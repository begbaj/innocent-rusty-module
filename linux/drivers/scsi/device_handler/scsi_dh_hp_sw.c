// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Basic HP/COMPAQ MSA 1000 support. This is only needed if your HW cannot be
 * upgraded.
 *
 * Copyright (C) 2006 Red Hat, Inc.  All rights reserved.
 * Copyright (C) 2006 Mike Christie
 * Copyright (C) 2008 Hannes Reinecke <hare@suse.de>
 */

#include <linux/slab.h>
#include <linux/module.h>
#include <scsi/scsi.h>
#include <scsi/scsi_dbg.h>
#include <scsi/scsi_eh.h>
#include <scsi/scsi_dh.h>

#define HP_SW_NAME			"hp_sw"

#define HP_SW_TIMEOUT			(60 * HZ)
#define HP_SW_RETRIES			3

#define HP_SW_PATH_UNINITIALIZED	-1
#define HP_SW_PATH_ACTIVE		0
#define HP_SW_PATH_PASSIVE		1

struct hp_sw_dh_data {
	int path_state;
	int retries;
	int retry_cnt;
	struct scsi_device *sdev;
};

static int hp_sw_start_stop(struct hp_sw_dh_data *);

/*
 * tur_done - Handle TEST UNIT READY return status
 * @sdev: sdev the command has been sent to
 * @errors: blk error code
 *
 * Returns SCSI_DH_DEV_OFFLINED if the sdev is on the passive path
 */
static int tur_done(struct scsi_device *sdev, struct hp_sw_dh_data *h,
		    struct scsi_sense_hdr *sshdr)
{
	int ret = SCSI_DH_IO;

	switch (sshdr->sense_key) {
	case NOT_READY:
		if (sshdr->asc == 0x04 && sshdr->ascq == 2) {
			/*
			 * LUN not ready - Initialization command required
			 *
			 * This is the passive path
			 */
			h->path_state = HP_SW_PATH_PASSIVE;
			ret = SCSI_DH_OK;
			break;
		}
		fallthrough;
	default:
		sdev_printk(KERN_WARNING, sdev,
			   "%s: sending tur failed, sense %x/%x/%x\n",
			   HP_SW_NAME, sshdr->sense_key, sshdr->asc,
			   sshdr->ascq);
		break;
	}
	return ret;
}

/*
 * hp_sw_tur - Send TEST UNIT READY
 * @sdev: sdev command should be sent to
 *
 * Use the TEST UNIT READY command to determine
 * the path state.
 */
static int hp_sw_tur(struct scsi_device *sdev, struct hp_sw_dh_data *h)
{
	unsigned char cmd[6] = { TEST_UNIT_READY };
	struct scsi_sense_hdr sshdr;
	int ret, res;
	blk_opf_t opf = REQ_OP_DRV_IN | REQ_FAILFAST_DEV |
				REQ_FAILFAST_TRANSPORT | REQ_FAILFAST_DRIVER;
	struct scsi_failure failure_defs[] = {
		{
			.sense = UNIT_ATTENTION,
			.asc = SCMD_FAILURE_ASC_ANY,
			.ascq = SCMD_FAILURE_ASCQ_ANY,
			.allowed = SCMD_FAILURE_NO_LIMIT,
			.result = SAM_STAT_CHECK_CONDITION,
		},
		{}
	};
	struct scsi_failures failures = {
		.failure_definitions = failure_defs,
	};
	const struct scsi_exec_args exec_args = {
		.sshdr = &sshdr,
		.failures = &failures,
	};

	res = scsi_execute_cmd(sdev, cmd, opf, NULL, 0, HP_SW_TIMEOUT,
			       HP_SW_RETRIES, &exec_args);
	if (res > 0 && scsi_sense_valid(&sshdr)) {
		ret = tur_done(sdev, h, &sshdr);
	} else if (res == 0) {
		h->path_state = HP_SW_PATH_ACTIVE;
		ret = SCSI_DH_OK;
	} else {
		sdev_printk(KERN_WARNING, sdev,
			    "%s: sending tur failed with %x\n",
			    HP_SW_NAME, res);
		ret = SCSI_DH_IO;
	}

	return ret;
}

/*
 * hp_sw_start_stop - Send START STOP UNIT command
 * @sdev: sdev command should be sent to
 *
 * Sending START STOP UNIT activates the SP.
 */
static int hp_sw_start_stop(struct hp_sw_dh_data *h)
{
	unsigned char cmd[6] = { START_STOP, 0, 0, 0, 1, 0 };
	struct scsi_sense_hdr sshdr;
	struct scsi_device *sdev = h->sdev;
	int res, rc;
	blk_opf_t opf = REQ_OP_DRV_IN | REQ_FAILFAST_DEV |
				REQ_FAILFAST_TRANSPORT | REQ_FAILFAST_DRIVER;
	struct scsi_failure failure_defs[] = {
		{
			/*
			 * LUN not ready - manual intervention required
			 *
			 * Switch-over in progress, retry.
			 */
			.sense = NOT_READY,
			.asc = 0x04,
			.ascq = 0x03,
			.allowed = HP_SW_RETRIES,
			.result = SAM_STAT_CHECK_CONDITION,
		},
		{}
	};
	struct scsi_failures failures = {
		.failure_definitions = failure_defs,
	};
	const struct scsi_exec_args exec_args = {
		.sshdr = &sshdr,
		.failures = &failures,
	};

	res = scsi_execute_cmd(sdev, cmd, opf, NULL, 0, HP_SW_TIMEOUT,
			       HP_SW_RETRIES, &exec_args);
	if (!res) {
		return SCSI_DH_OK;
	} else if (res < 0 || !scsi_sense_valid(&sshdr)) {
		sdev_printk(KERN_WARNING, sdev,
			    "%s: sending start_stop_unit failed, "
			    "no sense available\n", HP_SW_NAME);
		return SCSI_DH_IO;
	}

	switch (sshdr.sense_key) {
	case NOT_READY:
		if (sshdr.asc == 0x04 && sshdr.ascq == 3) {
			rc = SCSI_DH_RETRY;
			break;
		}
		fallthrough;
	default:
		sdev_printk(KERN_WARNING, sdev,
			    "%s: sending start_stop_unit failed, "
			    "sense %x/%x/%x\n", HP_SW_NAME,
			    sshdr.sense_key, sshdr.asc, sshdr.ascq);
		rc = SCSI_DH_IO;
	}

	return rc;
}

static blk_status_t hp_sw_prep_fn(struct scsi_device *sdev, struct request *req)
{
	struct hp_sw_dh_data *h = sdev->handler_data;

	if (h->path_state != HP_SW_PATH_ACTIVE) {
		req->rq_flags |= RQF_QUIET;
		return BLK_STS_IOERR;
	}

	return BLK_STS_OK;
}

/*
 * hp_sw_activate - Activate a path
 * @sdev: sdev on the path to be activated
 *
 * The HP Active/Passive firmware is pretty simple;
 * the passive path reports NOT READY with sense codes
 * 0x04/0x02; a START STOP UNIT command will then
 * activate the passive path (and deactivate the
 * previously active one).
 */
static int hp_sw_activate(struct scsi_device *sdev,
				activate_complete fn, void *data)
{
	int ret = SCSI_DH_OK;
	struct hp_sw_dh_data *h = sdev->handler_data;

	ret = hp_sw_tur(sdev, h);

	if (ret == SCSI_DH_OK && h->path_state == HP_SW_PATH_PASSIVE)
		ret = hp_sw_start_stop(h);

	if (fn)
		fn(data, ret);
	return 0;
}

static int hp_sw_bus_attach(struct scsi_device *sdev)
{
	struct hp_sw_dh_data *h;
	int ret;

	h = kzalloc(sizeof(*h), GFP_KERNEL);
	if (!h)
		return SCSI_DH_NOMEM;
	h->path_state = HP_SW_PATH_UNINITIALIZED;
	h->retries = HP_SW_RETRIES;
	h->sdev = sdev;

	ret = hp_sw_tur(sdev, h);
	if (ret != SCSI_DH_OK)
		goto failed;
	if (h->path_state == HP_SW_PATH_UNINITIALIZED) {
		ret = SCSI_DH_NOSYS;
		goto failed;
	}

	sdev_printk(KERN_INFO, sdev, "%s: attached to %s path\n",
		    HP_SW_NAME, h->path_state == HP_SW_PATH_ACTIVE?
		    "active":"passive");

	sdev->handler_data = h;
	return SCSI_DH_OK;
failed:
	kfree(h);
	return ret;
}

static void hp_sw_bus_detach( struct scsi_device *sdev )
{
	kfree(sdev->handler_data);
	sdev->handler_data = NULL;
}

static struct scsi_device_handler hp_sw_dh = {
	.name		= HP_SW_NAME,
	.module		= THIS_MODULE,
	.attach		= hp_sw_bus_attach,
	.detach		= hp_sw_bus_detach,
	.activate	= hp_sw_activate,
	.prep_fn	= hp_sw_prep_fn,
};

static int __init hp_sw_init(void)
{
	return scsi_register_device_handler(&hp_sw_dh);
}

static void __exit hp_sw_exit(void)
{
	scsi_unregister_device_handler(&hp_sw_dh);
}

module_init(hp_sw_init);
module_exit(hp_sw_exit);

MODULE_DESCRIPTION("HP Active/Passive driver");
MODULE_AUTHOR("Mike Christie <michaelc@cs.wisc.edu");
MODULE_LICENSE("GPL");
