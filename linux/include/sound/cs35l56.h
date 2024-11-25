/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Common definitions for Cirrus Logic CS35L56 smart amp
 *
 * Copyright (C) 2023 Cirrus Logic, Inc. and
 *                    Cirrus Logic International Semiconductor Ltd.
 */

#ifndef __CS35L56_H
#define __CS35L56_H

#include <linux/firmware/cirrus/cs_dsp.h>
#include <linux/regulator/consumer.h>
#include <linux/regmap.h>
#include <sound/cs-amp-lib.h>

#define CS35L56_DEVID					0x0000000
#define CS35L56_REVID					0x0000004
#define CS35L56_RELID					0x000000C
#define CS35L56_OTPID					0x0000010
#define CS35L56_SFT_RESET				0x0000020
#define CS35L56_GLOBAL_ENABLES				0x0002014
#define CS35L56_BLOCK_ENABLES				0x0002018
#define CS35L56_BLOCK_ENABLES2				0x000201C
#define CS35L56_REFCLK_INPUT				0x0002C04
#define CS35L56_GLOBAL_SAMPLE_RATE			0x0002C0C
#define CS35L56_OTP_MEM_53				0x00300D4
#define CS35L56_OTP_MEM_54				0x00300D8
#define CS35L56_OTP_MEM_55				0x00300DC
#define CS35L56_ASP1_ENABLES1				0x0004800
#define CS35L56_ASP1_CONTROL1				0x0004804
#define CS35L56_ASP1_CONTROL2				0x0004808
#define CS35L56_ASP1_CONTROL3				0x000480C
#define CS35L56_ASP1_FRAME_CONTROL1			0x0004810
#define CS35L56_ASP1_FRAME_CONTROL5			0x0004820
#define CS35L56_ASP1_DATA_CONTROL1			0x0004830
#define CS35L56_ASP1_DATA_CONTROL5			0x0004840
#define CS35L56_DACPCM1_INPUT				0x0004C00
#define CS35L56_DACPCM2_INPUT				0x0004C08
#define CS35L56_ASP1TX1_INPUT				0x0004C20
#define CS35L56_ASP1TX2_INPUT				0x0004C24
#define CS35L56_ASP1TX3_INPUT				0x0004C28
#define CS35L56_ASP1TX4_INPUT				0x0004C2C
#define CS35L56_DSP1RX1_INPUT				0x0004C40
#define CS35L56_DSP1RX2_INPUT				0x0004C44
#define CS35L56_SWIRE_DP3_CH1_INPUT			0x0004C70
#define CS35L56_SWIRE_DP3_CH2_INPUT			0x0004C74
#define CS35L56_SWIRE_DP3_CH3_INPUT			0x0004C78
#define CS35L56_SWIRE_DP3_CH4_INPUT			0x0004C7C
#define CS35L56_IRQ1_CFG				0x000E000
#define CS35L56_IRQ1_STATUS				0x000E004
#define CS35L56_IRQ1_EINT_1				0x000E010
#define CS35L56_IRQ1_EINT_2				0x000E014
#define CS35L56_IRQ1_EINT_4				0x000E01C
#define CS35L56_IRQ1_EINT_8				0x000E02C
#define CS35L56_IRQ1_EINT_18				0x000E054
#define CS35L56_IRQ1_EINT_20				0x000E05C
#define CS35L56_IRQ1_MASK_1				0x000E090
#define CS35L56_IRQ1_MASK_2				0x000E094
#define CS35L56_IRQ1_MASK_4				0x000E09C
#define CS35L56_IRQ1_MASK_8				0x000E0AC
#define CS35L56_IRQ1_MASK_18				0x000E0D4
#define CS35L56_IRQ1_MASK_20				0x000E0DC
#define CS35L56_DSP_VIRTUAL1_MBOX_1			0x0011020
#define CS35L56_DSP_VIRTUAL1_MBOX_2			0x0011024
#define CS35L56_DSP_VIRTUAL1_MBOX_3			0x0011028
#define CS35L56_DSP_VIRTUAL1_MBOX_4			0x001102C
#define CS35L56_DSP_VIRTUAL1_MBOX_5			0x0011030
#define CS35L56_DSP_VIRTUAL1_MBOX_6			0x0011034
#define CS35L56_DSP_VIRTUAL1_MBOX_7			0x0011038
#define CS35L56_DSP_VIRTUAL1_MBOX_8			0x001103C
#define CS35L56_DSP_RESTRICT_STS1			0x00190F0
#define CS35L56_DSP1_XMEM_PACKED_0			0x2000000
#define CS35L56_DSP1_XMEM_PACKED_6143			0x2005FFC
#define CS35L56_DSP1_XMEM_UNPACKED32_0			0x2400000
#define CS35L56_DSP1_XMEM_UNPACKED32_4095		0x2403FFC
#define CS35L56_DSP1_SYS_INFO_ID			0x25E0000
#define CS35L56_DSP1_SYS_INFO_END			0x25E004C
#define CS35L56_DSP1_AHBM_WINDOW_DEBUG_0		0x25E2040
#define CS35L56_DSP1_AHBM_WINDOW_DEBUG_1		0x25E2044
#define CS35L56_DSP1_XMEM_UNPACKED24_0			0x2800000
#define CS35L56_DSP1_FW_VER				0x2800010
#define CS35L56_DSP1_HALO_STATE				0x28021E0
#define CS35L56_DSP1_PM_CUR_STATE			0x2804308
#define CS35L56_DSP1_XMEM_UNPACKED24_8191		0x2807FFC
#define CS35L56_DSP1_CORE_BASE				0x2B80000
#define CS35L56_DSP1_SCRATCH1				0x2B805C0
#define CS35L56_DSP1_SCRATCH2				0x2B805C8
#define CS35L56_DSP1_SCRATCH3				0x2B805D0
#define CS35L56_DSP1_SCRATCH4				0x2B805D8
#define CS35L56_DSP1_YMEM_PACKED_0			0x2C00000
#define CS35L56_DSP1_YMEM_PACKED_4604			0x2C047F0
#define CS35L56_DSP1_YMEM_UNPACKED32_0			0x3000000
#define CS35L56_DSP1_YMEM_UNPACKED32_3070		0x3002FF8
#define CS35L56_DSP1_YMEM_UNPACKED24_0			0x3400000
#define CS35L56_MAIN_RENDER_USER_MUTE			0x3400024
#define CS35L56_MAIN_RENDER_USER_VOLUME			0x340002C
#define CS35L56_MAIN_POSTURE_NUMBER			0x3400094
#define CS35L56_PROTECTION_STATUS			0x34000D8
#define CS35L56_TRANSDUCER_ACTUAL_PS			0x3400150
#define CS35L56_DSP1_YMEM_UNPACKED24_6141		0x3405FF4
#define CS35L56_DSP1_PMEM_0				0x3800000
#define CS35L56_DSP1_PMEM_5114				0x3804FE8

/* DEVID */
#define CS35L56_DEVID_MASK				0x00FFFFFF

/* REVID */
#define CS35L56_AREVID_MASK				0x000000F0
#define CS35L56_MTLREVID_MASK				0x0000000F
#define CS35L56_REVID_B0				0x000000B0

/* ASP_ENABLES1 */
#define CS35L56_ASP_RX2_EN_SHIFT			17
#define CS35L56_ASP_RX1_EN_SHIFT			16
#define CS35L56_ASP_TX4_EN_SHIFT			3
#define CS35L56_ASP_TX3_EN_SHIFT			2
#define CS35L56_ASP_TX2_EN_SHIFT			1
#define CS35L56_ASP_TX1_EN_SHIFT			0

/* ASP_CONTROL1 */
#define CS35L56_ASP_BCLK_FREQ_MASK			0x0000003F
#define CS35L56_ASP_BCLK_FREQ_SHIFT			0

/* ASP_CONTROL2 */
#define CS35L56_ASP_RX_WIDTH_MASK			0xFF000000
#define CS35L56_ASP_RX_WIDTH_SHIFT			24
#define CS35L56_ASP_TX_WIDTH_MASK			0x00FF0000
#define CS35L56_ASP_TX_WIDTH_SHIFT			16
#define CS35L56_ASP_FMT_MASK				0x00000700
#define CS35L56_ASP_FMT_SHIFT				8
#define CS35L56_ASP_BCLK_INV_MASK			0x00000040
#define CS35L56_ASP_FSYNC_INV_MASK			0x00000004

/* ASP_CONTROL3 */
#define CS35L56_ASP1_DOUT_HIZ_CTRL_MASK			0x00000003

/* ASP_DATA_CONTROL1 */
#define CS35L56_ASP_TX_WL_MASK				0x0000003F

/* ASP_DATA_CONTROL5 */
#define CS35L56_ASP_RX_WL_MASK				0x0000003F

/* ASPTXn_INPUT */
#define CS35L56_ASP_TXn_SRC_MASK			0x0000007F

/* SWIRETX[1..7]_SRC SDWTXn INPUT */
#define CS35L56_SWIRETXn_SRC_MASK			0x0000007F

/* IRQ1_STATUS */
#define CS35L56_IRQ1_STS_MASK				0x00000001

/* IRQ1_EINT_1 */
#define CS35L56_AMP_SHORT_ERR_EINT1_MASK		0x80000000

/* IRQ1_EINT_2 */
#define CS35L56_DSP_VIRTUAL2_MBOX_WR_EINT1_MASK		0x00200000

/* IRQ1_EINT_4 */
#define CS35L56_OTP_BOOT_DONE_MASK			0x00000002

/* IRQ1_EINT_8 */
#define CS35L56_TEMP_ERR_EINT1_MASK			0x80000000

/* Mixer input sources */
#define CS35L56_INPUT_SRC_NONE				0x00
#define CS35L56_INPUT_SRC_ASP1RX1			0x08
#define CS35L56_INPUT_SRC_ASP1RX2			0x09
#define CS35L56_INPUT_SRC_VMON				0x18
#define CS35L56_INPUT_SRC_IMON				0x19
#define CS35L56_INPUT_SRC_ERR_VOL			0x20
#define CS35L56_INPUT_SRC_CLASSH			0x21
#define CS35L56_INPUT_SRC_VDDBMON			0x28
#define CS35L56_INPUT_SRC_VBSTMON			0x29
#define CS35L56_INPUT_SRC_DSP1TX1			0x32
#define CS35L56_INPUT_SRC_DSP1TX2			0x33
#define CS35L56_INPUT_SRC_DSP1TX3			0x34
#define CS35L56_INPUT_SRC_DSP1TX4			0x35
#define CS35L56_INPUT_SRC_DSP1TX5			0x36
#define CS35L56_INPUT_SRC_DSP1TX6			0x37
#define CS35L56_INPUT_SRC_DSP1TX7			0x38
#define CS35L56_INPUT_SRC_DSP1TX8			0x39
#define CS35L56_INPUT_SRC_TEMPMON			0x3A
#define CS35L56_INPUT_SRC_INTERPOLATOR			0x40
#define CS35L56_INPUT_SRC_SWIRE_DP1_CHANNEL1		0x44
#define CS35L56_INPUT_SRC_SWIRE_DP1_CHANNEL2		0x45
#define CS35L56_INPUT_MASK				0x7F

#define CS35L56_NUM_INPUT_SRC				21

/* ASP formats */
#define CS35L56_ASP_FMT_DSP_A				0
#define CS35L56_ASP_FMT_I2S				2

/* ASP HiZ modes */
#define CS35L56_ASP_UNUSED_HIZ_OFF_HIZ			3

/* MAIN_RENDER_ACTUAL_PS */
#define CS35L56_PS0					0
#define CS35L56_PS3					3

/* CS35L56_DSP_RESTRICT_STS1 */
#define CS35L56_RESTRICTED_MASK				0x7

/* CS35L56_MAIN_RENDER_USER_MUTE */
#define CS35L56_MAIN_RENDER_USER_MUTE_MASK		1

/* CS35L56_MAIN_RENDER_USER_VOLUME */
#define CS35L56_MAIN_RENDER_USER_VOLUME_MIN		-400
#define CS35L56_MAIN_RENDER_USER_VOLUME_MAX		48
#define CS35L56_MAIN_RENDER_USER_VOLUME_MASK		0x0000FFC0
#define CS35L56_MAIN_RENDER_USER_VOLUME_SHIFT		6
#define CS35L56_MAIN_RENDER_USER_VOLUME_SIGNBIT		9

/* CS35L56_MAIN_POSTURE_NUMBER */
#define CS35L56_MAIN_POSTURE_MIN			0
#define CS35L56_MAIN_POSTURE_MAX			255
#define CS35L56_MAIN_POSTURE_MASK			CS35L56_MAIN_POSTURE_MAX

/* CS35L56_PROTECTION_STATUS */
#define CS35L56_FIRMWARE_MISSING			BIT(0)

/* Software Values */
#define CS35L56_HALO_STATE_SHUTDOWN			1
#define CS35L56_HALO_STATE_BOOT_DONE			2

#define CS35L56_MBOX_CMD_AUDIO_PLAY			0x0B000001
#define CS35L56_MBOX_CMD_AUDIO_PAUSE			0x0B000002
#define CS35L56_MBOX_CMD_AUDIO_REINIT			0x0B000003
#define CS35L56_MBOX_CMD_HIBERNATE_NOW			0x02000001
#define CS35L56_MBOX_CMD_WAKEUP				0x02000002
#define CS35L56_MBOX_CMD_PREVENT_AUTO_HIBERNATE		0x02000003
#define CS35L56_MBOX_CMD_ALLOW_AUTO_HIBERNATE		0x02000004
#define CS35L56_MBOX_CMD_SHUTDOWN			0x02000005
#define CS35L56_MBOX_CMD_SYSTEM_RESET			0x02000007

#define CS35L56_MBOX_TIMEOUT_US				5000
#define CS35L56_MBOX_POLL_US				250

#define CS35L56_PS0_POLL_US				500
#define CS35L56_PS0_TIMEOUT_US				50000
#define CS35L56_PS3_POLL_US				500
#define CS35L56_PS3_TIMEOUT_US				300000

#define CS35L56_CONTROL_PORT_READY_US			2200
#define CS35L56_HALO_STATE_POLL_US			1000
#define CS35L56_HALO_STATE_TIMEOUT_US			250000
#define CS35L56_RESET_PULSE_MIN_US			1100
#define CS35L56_WAKE_HOLD_TIME_US			1000

#define CS35L56_SDW1_PLAYBACK_PORT			1
#define CS35L56_SDW1_CAPTURE_PORT			3

#define CS35L56_NUM_BULK_SUPPLIES			3
#define CS35L56_NUM_DSP_REGIONS				5

struct cs35l56_base {
	struct device *dev;
	struct regmap *regmap;
	int irq;
	struct mutex irq_lock;
	u8 type;
	u8 rev;
	bool init_done;
	bool fw_patched;
	bool secured;
	bool can_hibernate;
	bool cal_data_valid;
	s8 cal_index;
	struct cirrus_amp_cal_data cal_data;
	struct gpio_desc *reset_gpio;
};

/* Temporary to avoid a build break with the HDA driver */
static inline int cs35l56_force_sync_asp1_registers_from_cache(struct cs35l56_base *cs35l56_base)
{
	return 0;
}

static inline bool cs35l56_is_otp_register(unsigned int reg)
{
	return (reg >> 16) == 3;
}

extern const struct regmap_config cs35l56_regmap_i2c;
extern const struct regmap_config cs35l56_regmap_spi;
extern const struct regmap_config cs35l56_regmap_sdw;

extern const struct cirrus_amp_cal_controls cs35l56_calibration_controls;

extern const char * const cs35l56_tx_input_texts[CS35L56_NUM_INPUT_SRC];
extern const unsigned int cs35l56_tx_input_values[CS35L56_NUM_INPUT_SRC];

int cs35l56_set_patch(struct cs35l56_base *cs35l56_base);
int cs35l56_mbox_send(struct cs35l56_base *cs35l56_base, unsigned int command);
int cs35l56_firmware_shutdown(struct cs35l56_base *cs35l56_base);
int cs35l56_wait_for_firmware_boot(struct cs35l56_base *cs35l56_base);
void cs35l56_wait_control_port_ready(void);
void cs35l56_wait_min_reset_pulse(void);
void cs35l56_system_reset(struct cs35l56_base *cs35l56_base, bool is_soundwire);
int cs35l56_irq_request(struct cs35l56_base *cs35l56_base, int irq);
irqreturn_t cs35l56_irq(int irq, void *data);
int cs35l56_is_fw_reload_needed(struct cs35l56_base *cs35l56_base);
int cs35l56_runtime_suspend_common(struct cs35l56_base *cs35l56_base);
int cs35l56_runtime_resume_common(struct cs35l56_base *cs35l56_base, bool is_soundwire);
void cs35l56_init_cs_dsp(struct cs35l56_base *cs35l56_base, struct cs_dsp *cs_dsp);
int cs35l56_get_calibration(struct cs35l56_base *cs35l56_base);
int cs35l56_read_prot_status(struct cs35l56_base *cs35l56_base,
			     bool *fw_missing, unsigned int *fw_version);
int cs35l56_hw_init(struct cs35l56_base *cs35l56_base);
int cs35l56_get_speaker_id(struct cs35l56_base *cs35l56_base);
int cs35l56_get_bclk_freq_id(unsigned int freq);
void cs35l56_fill_supply_names(struct regulator_bulk_data *data);

#endif /* ifndef __CS35L56_H */
