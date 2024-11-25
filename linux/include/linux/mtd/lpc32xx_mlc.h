/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Platform data for LPC32xx SoC MLC NAND controller
 *
 * Copyright © 2012 Roland Stigge
 */

#ifndef __LINUX_MTD_LPC32XX_MLC_H
#define __LINUX_MTD_LPC32XX_MLC_H

#include <linux/dmaengine.h>

struct lpc32xx_mlc_platform_data {
	dma_filter_fn dma_filter;
};

#endif  /* __LINUX_MTD_LPC32XX_MLC_H */
