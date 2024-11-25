/* SPDX-License-Identifier: GPL-2.0-only OR MIT */
/*
 * Copyright 2024 NXP
 */

#ifndef __DT_BINDINGS_CLOCK_IMX95_H
#define __DT_BINDINGS_CLOCK_IMX95_H

#define IMX95_CLK_VPUBLK_WAVE			0
#define IMX95_CLK_VPUBLK_JPEG_ENC		1
#define IMX95_CLK_VPUBLK_JPEG_DEC		2

#define IMX95_CLK_CAMBLK_CSI2_FOR0		0
#define IMX95_CLK_CAMBLK_CSI2_FOR1		1
#define IMX95_CLK_CAMBLK_ISP_AXI		2
#define IMX95_CLK_CAMBLK_ISP_PIXEL		3
#define IMX95_CLK_CAMBLK_ISP			4

#define IMX95_CLK_DISPMIX_LVDS_PHY_DIV		0
#define IMX95_CLK_DISPMIX_LVDS_CH0_GATE		1
#define IMX95_CLK_DISPMIX_LVDS_CH1_GATE		2
#define IMX95_CLK_DISPMIX_PIX_DI0_GATE		3
#define IMX95_CLK_DISPMIX_PIX_DI1_GATE		4

#define IMX95_CLK_DISPMIX_ENG0_SEL		0
#define IMX95_CLK_DISPMIX_ENG1_SEL		1

#define IMX95_CLK_NETCMIX_ENETC0_RMII		0
#define IMX95_CLK_NETCMIX_ENETC1_RMII		1

#endif	/* __DT_BINDINGS_CLOCK_IMX95_H */
