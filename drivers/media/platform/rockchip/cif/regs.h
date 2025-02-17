/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Rockchip CIF Driver
 *
 * Copyright (C) 2018 Rockchip Electronics Co., Ltd.
 */

#ifndef _RKCIF_REGS_H
#define _RKCIF_REGS_H

struct cif_reg {
	u32 offset;
};

#define CIF_REG(_offset)		{ .offset = (_offset), }

enum cif_reg_index {
	/* dvp registers index */
	CIF_REG_DVP_CTRL = 0x0,
	CIF_REG_DVP_INTEN,
	CIF_REG_DVP_INTSTAT,
	CIF_REG_DVP_FOR,
	CIF_REG_DVP_LINE_NUM_ADDR,
	CIF_REG_DVP_DMA_IDLE_REQ,
	CIF_REG_DVP_MULTI_ID,
	CIF_REG_DVP_FRM0_ADDR_Y,
	CIF_REG_DVP_FRM0_ADDR_UV,
	CIF_REG_DVP_FRM1_ADDR_Y,
	CIF_REG_DVP_FRM1_ADDR_UV,
	CIF_REG_DVP_VIR_LINE_WIDTH,
	CIF_REG_DVP_SET_SIZE,
	CIF_REG_DVP_SCM_ADDR_Y,
	CIF_REG_DVP_SCM_ADDR_U,
	CIF_REG_DVP_SCM_ADDR_V,
	CIF_REG_DVP_WB_UP_FILTER,
	CIF_REG_DVP_WB_LOW_FILTER,
	CIF_REG_DVP_WBC_CNT,
	CIF_REG_DVP_LINE_INT_NUM,
	CIF_REG_DVP_LINE_CNT,
	CIF_REG_DVP_CROP,
	CIF_REG_DVP_SCL_CTRL,
	CIF_REG_DVP_SCL_DST,
	CIF_REG_DVP_SCL_FCT,
	CIF_REG_DVP_SCL_VALID_NUM,
	CIF_REG_DVP_LINE_LOOP_CTRL,
	CIF_REG_DVP_PATH_SEL,
	CIF_REG_DVP_FIFO_ENTRY,
	CIF_REG_DVP_FRAME_STATUS,
	CIF_REG_DVP_CUR_DST,
	CIF_REG_DVP_LAST_LINE,
	CIF_REG_DVP_LAST_PIX,
	/* mipi & lvds registers index */
	CIF_REG_MIPI_LVDS_ID0_CTRL0,
	CIF_REG_MIPI_LVDS_ID0_CTRL1,
	CIF_REG_MIPI_LVDS_ID1_CTRL0,
	CIF_REG_MIPI_LVDS_ID1_CTRL1,
	CIF_REG_MIPI_LVDS_ID2_CTRL0,
	CIF_REG_MIPI_LVDS_ID2_CTRL1,
	CIF_REG_MIPI_LVDS_ID3_CTRL0,
	CIF_REG_MIPI_LVDS_ID3_CTRL1,
	CIF_REG_MIPI_WATER_LINE,
	CIF_REG_MIPI_LVDS_CTRL,
	CIF_REG_MIPI_LVDS_FRAME0_ADDR_Y_ID0,
	CIF_REG_MIPI_LVDS_FRAME1_ADDR_Y_ID0,
	CIF_REG_MIPI_LVDS_FRAME0_ADDR_UV_ID0,
	CIF_REG_MIPI_LVDS_FRAME1_ADDR_UV_ID0,
	CIF_REG_MIPI_LVDS_FRAME0_VLW_Y_ID0,
	CIF_REG_MIPI_LVDS_FRAME1_VLW_Y_ID0,
	CIF_REG_MIPI_LVDS_FRAME0_VLW_UV_ID0,
	CIF_REG_MIPI_LVDS_FRAME1_VLW_UV_ID0,
	CIF_REG_MIPI_LVDS_FRAME0_ADDR_Y_ID1,
	CIF_REG_MIPI_LVDS_FRAME1_ADDR_Y_ID1,
	CIF_REG_MIPI_LVDS_FRAME0_ADDR_UV_ID1,
	CIF_REG_MIPI_LVDS_FRAME1_ADDR_UV_ID1,
	CIF_REG_MIPI_LVDS_FRAME0_VLW_Y_ID1,
	CIF_REG_MIPI_LVDS_FRAME1_VLW_Y_ID1,
	CIF_REG_MIPI_LVDS_FRAME0_VLW_UV_ID1,
	CIF_REG_MIPI_LVDS_FRAME1_VLW_UV_ID1,
	CIF_REG_MIPI_LVDS_FRAME0_ADDR_Y_ID2,
	CIF_REG_MIPI_LVDS_FRAME1_ADDR_Y_ID2,
	CIF_REG_MIPI_LVDS_FRAME0_ADDR_UV_ID2,
	CIF_REG_MIPI_LVDS_FRAME1_ADDR_UV_ID2,
	CIF_REG_MIPI_LVDS_FRAME0_VLW_Y_ID2,
	CIF_REG_MIPI_LVDS_FRAME1_VLW_Y_ID2,
	CIF_REG_MIPI_LVDS_FRAME0_VLW_UV_ID2,
	CIF_REG_MIPI_LVDS_FRAME1_VLW_UV_ID2,
	CIF_REG_MIPI_LVDS_FRAME0_ADDR_Y_ID3,
	CIF_REG_MIPI_LVDS_FRAME1_ADDR_Y_ID3,
	CIF_REG_MIPI_LVDS_FRAME0_ADDR_UV_ID3,
	CIF_REG_MIPI_LVDS_FRAME1_ADDR_UV_ID3,
	CIF_REG_MIPI_LVDS_FRAME0_VLW_Y_ID3,
	CIF_REG_MIPI_LVDS_FRAME1_VLW_Y_ID3,
	CIF_REG_MIPI_LVDS_FRAME0_VLW_UV_ID3,
	CIF_REG_MIPI_LVDS_FRAME1_VLW_UV_ID3,
	CIF_REG_MIPI_LVDS_INTEN,
	CIF_REG_MIPI_LVDS_INTSTAT,
	CIF_REG_MIPI_LVDS_LINE_INT_NUM_ID0_1,
	CIF_REG_MIPI_LVDS_LINE_INT_NUM_ID2_3,
	CIF_REG_MIPI_LVDS_LINE_LINE_CNT_ID0_1,
	CIF_REG_MIPI_LVDS_LINE_LINE_CNT_ID2_3,
	CIF_REG_MIPI_LVDS_ID0_CROP_START,
	CIF_REG_MIPI_LVDS_ID1_CROP_START,
	CIF_REG_MIPI_LVDS_ID2_CROP_START,
	CIF_REG_MIPI_LVDS_ID3_CROP_START,
	CIF_REG_LVDS_SAV_EAV_ACT0_ID0,
	CIF_REG_LVDS_SAV_EAV_BLK0_ID0,
	CIF_REG_LVDS_SAV_EAV_ACT1_ID0,
	CIF_REG_LVDS_SAV_EAV_BLK1_ID0,
	CIF_REG_LVDS_SAV_EAV_ACT0_ID1,
	CIF_REG_LVDS_SAV_EAV_BLK0_ID1,
	CIF_REG_LVDS_SAV_EAV_ACT1_ID1,
	CIF_REG_LVDS_SAV_EAV_BLK1_ID1,
	CIF_REG_LVDS_SAV_EAV_ACT0_ID2,
	CIF_REG_LVDS_SAV_EAV_BLK0_ID2,
	CIF_REG_LVDS_SAV_EAV_ACT1_ID2,
	CIF_REG_LVDS_SAV_EAV_BLK1_ID2,
	CIF_REG_LVDS_SAV_EAV_ACT0_ID3,
	CIF_REG_LVDS_SAV_EAV_BLK0_ID3,
	CIF_REG_LVDS_SAV_EAV_ACT1_ID3,
	CIF_REG_LVDS_SAV_EAV_BLK1_ID3,
	CIF_REG_Y_STAT_CONTROL,
	CIF_REG_Y_STAT_VALUE,
	CIF_REG_MMU_DTE_ADDR,
	CIF_REG_MMU_STATUS,
	CIF_REG_MMU_COMMAND,
	CIF_REG_MMU_PAGE_FAULT_ADDR,
	CIF_REG_MMU_ZAP_ONE_LINE,
	CIF_REG_MMU_INT_RAWSTAT,
	CIF_REG_MMU_INT_CLEAR,
	CIF_REG_MMU_INT_MASK,
	CIF_REG_MMU_INT_STATUS,
	CIF_REG_MMU_AUTO_GATING,
	CIF_REG_INDEX_MAX
};

/* CIF Reg Offset */
#define CIF_CTRL			0x00
#define CIF_INTEN			0x04
#define CIF_INTSTAT			0x08
#define CIF_FOR				0x0c
#define CIF_LINE_NUM_ADDR		0x10
#define CIF_DMA_IDLE_REQ		0x10
#define CIF_FRM0_ADDR_Y			0x14
#define CIF_FRM0_ADDR_UV		0x18
#define CIF_FRM1_ADDR_Y			0x1c
#define CIF_FRM1_ADDR_UV		0x20
#define CIF_VIR_LINE_WIDTH		0x24
#define CIF_SET_SIZE			0x28
#define CIF_SCM_ADDR_Y			0x2c
#define CIF_LINE_INT_NUM		0x2c
#define CIF_SCM_ADDR_U			0x30
#define CIF_LINE_CNT			0x30
#define CIF_SCM_ADDR_V			0x34
#define CIF_WB_UP_FILTER		0x38
#define CIF_WB_LOW_FILTER		0x3c
#define CIF_WBC_CNT			0x40
#define CIF_CROP			0x44
#define RV1126_CIF_CROP			0x34
#define CIF_SCL_CTRL			0x48
#define CIF_PATH_SEL			0x48
#define CIF_SCL_DST			0x4c
#define CIF_SCL_FCT			0x50
#define CIF_SCL_VALID_NUM		0x54
#define CIF_FIFO_ENTRY			0x54
#define CIF_LINE_LOOP_CTR		0x58
#define CIF_FRAME_STATUS		0x60
#define RV1126_CIF_FRAME_STATUS		0x3c
#define CIF_CUR_DST			0x64
#define RV1126_CIF_CUR_DST		0x40
#define CIF_LAST_LINE			0x68
#define RV1126_CIF_LAST_LINE		0x44
#define CIF_LAST_PIX			0x6c
#define RV1126_CIF_LAST_PIX		0x48
#define CIF_MULTI_ID			0x10

#define CIF_FETCH_Y_LAST_LINE(val)	((val) & 0x1fff)
/* Check if swap y and c in bt1120 mode */
#define CIF_FETCH_IS_Y_FIRST(val)	((val) & 0xf)
#define CIF_RAW_STORED_BIT_WIDTH	(16U)
#define CIF_YUV_STORED_BIT_WIDTH	(8U)

/* RK1808 & RV1126 CIF CSI & LVDS Registers Offset */
#define CIF_CSI_ID0_CTRL0		0x80
#define CIF_CSI_ID0_CTRL1		0x84
#define CIF_CSI_ID1_CTRL0		0x88
#define CIF_CSI_ID1_CTRL1		0x8c
#define CIF_CSI_ID2_CTRL0		0x90
#define CIF_CSI_ID2_CTRL1		0x94
#define CIF_CSI_ID3_CTRL0		0x98
#define CIF_CSI_ID3_CTRL1		0x9c
#define CIF_CSI_WATER_LINE		0xa0
#define CIF_CSI_MIPI_LVDS_CTRL		0xa0
#define CIF_CSI_FRM0_ADDR_Y_ID0		0xa4
#define CIF_CSI_FRM1_ADDR_Y_ID0		0xa8
#define CIF_CSI_FRM0_ADDR_UV_ID0	0xac
#define CIF_CSI_FRM1_ADDR_UV_ID0	0xb0
#define CIF_CSI_FRM0_VLW_Y_ID0		0xb4
#define CIF_CSI_FRM1_VLW_Y_ID0		0xb8
#define CIF_CSI_FRM0_VLW_UV_ID0		0xbc
#define CIF_CSI_FRM1_VLW_UV_ID0		0xc0
#define CIF_CSI_FRM0_ADDR_Y_ID1		0xc4
#define CIF_CSI_FRM1_ADDR_Y_ID1		0xc8
#define CIF_CSI_FRM0_ADDR_UV_ID1	0xcc
#define CIF_CSI_FRM1_ADDR_UV_ID1	0xd0
#define CIF_CSI_FRM0_VLW_Y_ID1		0xd4
#define CIF_CSI_FRM1_VLW_Y_ID1		0xd8
#define CIF_CSI_FRM0_VLW_UV_ID1		0xdc
#define CIF_CSI_FRM1_VLW_UV_ID1		0xe0
#define CIF_CSI_FRM0_ADDR_Y_ID2		0xe4
#define CIF_CSI_FRM1_ADDR_Y_ID2		0xe8
#define CIF_CSI_FRM0_ADDR_UV_ID2	0xec
#define CIF_CSI_FRM1_ADDR_UV_ID2	0xf0
#define CIF_CSI_FRM0_VLW_Y_ID2		0xf4
#define CIF_CSI_FRM1_VLW_Y_ID2		0xf8
#define CIF_CSI_FRM0_VLW_UV_ID2		0xfc
#define CIF_CSI_FRM1_VLW_UV_ID2		0x100
#define CIF_CSI_FRM0_ADDR_Y_ID3		0x104
#define CIF_CSI_FRM1_ADDR_Y_ID3		0x108
#define CIF_CSI_FRM0_ADDR_UV_ID3	0x10c
#define CIF_CSI_FRM1_ADDR_UV_ID3	0x110
#define CIF_CSI_FRM0_VLW_Y_ID3		0x114
#define CIF_CSI_FRM1_VLW_Y_ID3		0x118
#define CIF_CSI_FRM0_VLW_UV_ID3		0x11c
#define CIF_CSI_FRM1_VLW_UV_ID3		0x120
#define CIF_CSI_INTEN			0x124
#define CIF_CSI_INTSTAT			0x128
#define CIF_CSI_LINE_INT_NUM_ID0_1	0x12c
#define CIF_CSI_LINE_INT_NUM_ID2_3	0x130
#define CIF_CSI_LINE_CNT_ID0_1		0x134
#define CIF_CSI_LINE_CNT_ID2_3		0x138
#define CIF_CSI_ID0_CROP_START		0x13c
#define CIF_CSI_ID1_CROP_START		0x140
#define CIF_CSI_ID2_CROP_START		0x144
#define CIF_CSI_ID3_CROP_START		0x148
#define CIF_LVDS_SAV_EAV_ACT0_ID0	0x150
#define CIF_LVDS_SAV_EAV_BLK0_ID0	0x154
#define CIF_LVDS_SAV_EAV_ACT1_ID0	0x158
#define CIF_LVDS_SAV_EAV_BLK1_ID0	0x15c
#define CIF_LVDS_SAV_EAV_ACT0_ID1	0x160
#define CIF_LVDS_SAV_EAV_BLK0_ID1	0x164
#define CIF_LVDS_SAV_EAV_ACT1_ID1	0x168
#define CIF_LVDS_SAV_EAV_BLK1_ID1	0x16c
#define CIF_LVDS_SAV_EAV_ACT0_ID2	0x170
#define CIF_LVDS_SAV_EAV_BLK0_ID2	0x174
#define CIF_LVDS_SAV_EAV_ACT1_ID2	0x178
#define CIF_LVDS_SAV_EAV_BLK1_ID2	0x17c
#define CIF_LVDS_SAV_EAV_ACT0_ID3	0x180
#define CIF_LVDS_SAV_EAV_BLK0_ID3	0x184
#define CIF_LVDS_SAV_EAV_ACT1_ID3	0x188
#define CIF_LVDS_SAV_EAV_BLK1_ID3	0x18c
#define CIF_Y_STAT_CONTROL		0x190
#define CIF_Y_STAT_VALUE		0x194
#define CIF_MMU_DTE_ADDR		0x800
#define CIF_MMU_STATUS			0x804
#define CIF_MMU_COMMAND			0x808
#define CIF_MMU_PAGE_FAULT_ADDR		0x80c
#define CIF_MMU_ZAP_ONE_LINE		0x810
#define CIF_MMU_INT_RAWSTAT		0x814
#define CIF_MMU_INT_CLEAR		0x818
#define CIF_MMU_INT_MASK		0x81c
#define CIF_MMU_INT_STATUS		0x820
#define CIF_MMU_AUTO_GATING		0x824

/* The key register bit description */

/* CIF_CTRL Reg */
#define DISABLE_CAPTURE			(0x0 << 0)
#define ENABLE_CAPTURE			(0x1 << 0)
#define MODE_ONEFRAME			(0x0 << 1)
#define MODE_PINGPONG			(0x1 << 1)
#define MODE_LINELOOP			(0x2 << 1)
#define AXI_BURST_16			(0xF << 12)

/* CIF_INTEN */
#define INTEN_DISABLE			(0x0 << 0)
#define FRAME_END_EN			(0x1 << 0)
#define BUS_ERR_EN			(0x1 << 6)
#define SCL_ERR_EN			(0x1 << 7)
#define PST_INF_FRAME_END_EN		(0x1 << 9)

/* CIF INTSTAT */
#define INTSTAT_CLS			(0x3FF)
#define FRAME_END			(0x01 << 0)
#define LINE_ERR			(0x1 << 2)
#define PIX_ERR				(0x1 << 3)
#define IFIFO_OVERFLOW			(0x1 << 4)
#define DFIFO_OVERFLOW			(0x1 << 5)
#define BUS_ERR				(0x1 << 6)
#define PST_INF_FRAME_END		(0x01 << 9)
#define FRAME_END_CLR			(0x01 << 0)
#define PST_INF_FRAME_END_CLR		(0x01 << 9)
#define INTSTAT_ERR			(0xFC)
#define DVP_ALL_OVERFLOW		(IFIFO_OVERFLOW | DFIFO_OVERFLOW)

/* FRAME STATUS */
#define FRAME_STAT_CLS			0x00
/* write 0 to clear frame 0 */
#define FRM0_STAT_CLS			0xfffffffe
#define FRAME_NUM_SHIFT			(16U)
#define FRAME_NUM_MASK			(0xffff << FRAME_NUM_SHIFT)
#define CIF_GET_FRAME_ID(val)		(((val) & FRAME_NUM_MASK) >> FRAME_NUM_SHIFT)

/* CIF FORMAT */
#define VSY_HIGH_ACTIVE			(0x01 << 0)
#define VSY_LOW_ACTIVE			(0x00 << 0)
#define HSY_LOW_ACTIVE			(0x01 << 1)
#define HSY_HIGH_ACTIVE			(0x00 << 1)
#define INPUT_MODE_YUV			(0x00 << 2)
#define INPUT_MODE_PAL			(0x02 << 2)
#define INPUT_MODE_NTSC			(0x03 << 2)
#define INPUT_MODE_BT1120		(0x07 << 2)
#define INPUT_MODE_RAW			(0x04 << 2)
#define INPUT_MODE_JPEG			(0x05 << 2)
#define INPUT_MODE_MIPI			(0x06 << 2)
#define YUV_INPUT_ORDER_UYVY		(0x00 << 5)
#define YUV_INPUT_ORDER_YVYU		(0x01 << 5)
#define YUV_INPUT_ORDER_VYUY		(0x10 << 5)
#define YUV_INPUT_ORDER_YUYV		(0x03 << 5)
#define YUV_INPUT_422			(0x00 << 7)
#define YUV_INPUT_420			(0x01 << 7)
#define INPUT_420_ORDER_EVEN		(0x00 << 8)
#define INPUT_420_ORDER_ODD		(0x01 << 8)
#define CCIR_INPUT_ORDER_ODD		(0x00 << 9)
#define CCIR_INPUT_ORDER_EVEN		(0x01 << 9)
#define RAW_DATA_WIDTH_8		(0x00 << 11)
#define RAW_DATA_WIDTH_10		(0x01 << 11)
#define RAW_DATA_WIDTH_12		(0x02 << 11)
#define MIPI_MODE_32BITS_BYPASS		(0x00 << 13)
#define MIPI_MODE_RGB			(0x01 << 13)
#define MIPI_MODE_YUV			(0x02 << 13)
#define YUV_OUTPUT_422			(0x00 << 16)
#define YUV_OUTPUT_420			(0x01 << 16)
#define OUTPUT_420_ORDER_EVEN		(0x00 << 17)
#define OUTPUT_420_ORDER_ODD		(0x01 << 17)
#define RAWD_DATA_LITTLE_ENDIAN		(0x00 << 18)
#define RAWD_DATA_BIG_ENDIAN		(0x01 << 18)
#define UV_STORAGE_ORDER_UVUV		(0x00 << 19)
#define UV_STORAGE_ORDER_VUVU		(0x01 << 19)
#define BT1120_CLOCK_SINGLE_EDGES	(0x00 << 24)
#define BT1120_CLOCK_DOUBLE_EDGES	(0x01 << 24)
#define BT1120_TRANSMIT_INTERFACE	(0x00 << 25)
#define BT1120_TRANSMIT_PROGRESS	(0x01 << 25)
#define BT1120_YC_SWAP			(0x01 << 26)

/* CIF_SCL_CTRL */
#define ENABLE_SCL_DOWN			(0x01 << 0)
#define DISABLE_SCL_DOWN		(0x00 << 0)
#define ENABLE_SCL_UP			(0x01 << 1)
#define DISABLE_SCL_UP			(0x00 << 1)
#define ENABLE_YUV_16BIT_BYPASS		(0x01 << 4)
#define DISABLE_YUV_16BIT_BYPASS	(0x00 << 4)
#define ENABLE_RAW_16BIT_BYPASS		(0x01 << 5)
#define DISABLE_RAW_16BIT_BYPASS	(0x00 << 5)
#define ENABLE_32BIT_BYPASS		(0x01 << 6)
#define DISABLE_32BIT_BYPASS		(0x00 << 6)

/* CIF_INTSTAT */
#define CIF_F0_READY			(0x01 << 0)
#define CIF_F1_READY			(0x01 << 1)

/* CIF CROP */
#define CIF_CROP_Y_SHIFT		16
#define CIF_CROP_X_SHIFT		0

/* CIF_CSI_ID_CTRL0 */
#define CSI_DISABLE_CAPTURE		(0x0 << 0)
#define CSI_ENABLE_CAPTURE		(0x1 << 0)
#define CSI_WRDDR_TYPE_RAW8		(0x0 << 1)
#define CSI_WRDDR_TYPE_RAW10		(0x1 << 1)
#define CSI_WRDDR_TYPE_RAW12		(0x2 << 1)
#define CSI_WRDDR_TYPE_RGB888		(0x3 << 1)
#define CSI_WRDDR_TYPE_YUV422		(0x4 << 1)
#define CSI_WRDDR_TYPE_YUV420SP		(0x5 << 1)
#define CSI_WRDDR_TYPE_YUV400		(0x6 << 1)
#define CSI_DISABLE_COMMAND_MODE	(0x0 << 4)
#define CSI_ENABLE_COMMAND_MODE		(0x1 << 4)
#define CSI_DISABLE_CROP		(0x0 << 5)
#define CSI_ENABLE_CROP			(0x1 << 5)
#define CSI_ENABLE_MIPI_COMPACT		(0x1 << 6)

#define LVDS_ENABLE_CAPTURE		(0x1 << 16)
#define LVDS_MODE(mode)			(((mode) & 0x7) << 17)
#define LVDS_LANES_ENABLED(lanes)	\
	({ \
		unsigned int mask; \
		switch (lanes) { \
		case 1: \
			mask = 0x1 << 20; \
			break; \
		case 2: \
			mask = 0x3 << 20; \
			break; \
		case 3: \
			mask = 0x7 << 20; \
			break; \
		case 4: \
			mask = 0xf << 20; \
			break; \
		default: \
			mask = 0x1 << 20; \
			break; \
		} \
		mask; \
	})

#define LVDS_MAIN_LANE(index)		(((index) & 0x3) << 24)
#define LVDS_FID(id)			(((id) & 0x3) << 26)
#define LVDS_HDR_FRAME_X2		(0x0 << 28)
#define LVDS_HDR_FRAME_X3		(0x1 << 28)
#define LVDS_COMPACT			(0x1 << 29)

/* CIF_CSI_INTEN */
#define CSI_FRAME1_START_INTEN(id)	(0x1 << ((id) * 2 + 1))
#define CSI_FRAME0_END_INTEN(id)	(0x1 << ((id) * 2 + 8))
#define CSI_FRAME1_END_INTEN(id)	(0x1 << ((id) * 2 + 9))
#define CSI_DMA_Y_FIFO_OVERFLOW_INTEN	(0x1 << 16)
#define CSI_DMA_UV_FIFO_OVERFLOW_INTEN	(0x1 << 17)
#define CSI_CONFIG_FIFO_OVERFLOW_INTEN	(0x1 << 18)
#define CSI_BANDWIDTH_LACK_INTEN	(0x1 << 19)
#define CSI_RX_FIFO_OVERFLOW_INTEN	(0x1 << 20)
#define CSI_ALL_FRAME_START_INTEN	(0xff << 0)
#define CSI_ALL_FRAME_END_INTEN		(0xff << 8)
#define CSI_ALL_ERROR_INTEN		(0x1f << 16)

#define CSI_START_INTEN(id)		(0x3 << ((id) * 2))
#define CSI_DMA_END_INTEN(id)		(0x3 << ((id) * 2 + 8))
#define CSI_LINE_INTEN(id)		(0x1 << ((id) + 21))

#define CSI_START_INTSTAT(id)		(0x3 << ((id) * 2))
#define CSI_DMA_END_INTSTAT(id)		(0x3 << ((id) * 2 + 8))
#define CSI_LINE_INTSTAT(id)		(0x1 << ((id) + 21))

/* CIF_CSI_INTSTAT */
#define CSI_FRAME0_START_ID0		(0x1 << 0)
#define CSI_FRAME1_START_ID0		(0x1 << 1)
#define CSI_FRAME0_START_ID1		(0x1 << 2)
#define CSI_FRAME1_START_ID1		(0x1 << 3)
#define CSI_FRAME0_START_ID2		(0x1 << 4)
#define CSI_FRAME1_START_ID2		(0x1 << 5)
#define CSI_FRAME0_START_ID3		(0x1 << 6)
#define CSI_FRAME1_START_ID3		(0x1 << 7)
#define CSI_FRAME0_END_ID0		(0x1 << 8)
#define CSI_FRAME1_END_ID0		(0x1 << 9)
#define CSI_FRAME0_END_ID1		(0x1 << 10)
#define CSI_FRAME1_END_ID1		(0x1 << 11)
#define CSI_FRAME0_END_ID2		(0x1 << 12)
#define CSI_FRAME1_END_ID2		(0x1 << 13)
#define CSI_FRAME0_END_ID3		(0x1 << 14)
#define CSI_FRAME1_END_ID3		(0x1 << 15)
#define CSI_DMA_Y_FIFO_OVERFLOW		(0x1 << 16)
#define CSI_DMA_UV_FIFO_OVERFLOW	(0x1 << 17)
#define CSI_CONFIG_FIFO_OVERFLOW	(0x1 << 18)
#define CSI_BANDWIDTH_LACK		(0x1 << 19)
#define CSI_RX_FIFO_OVERFLOW		(0x1 << 20)
#define CSI_LINE_ID0_INTST		(0x1 << 21)
#define CSI_LINE_ID1_INTST		(0x1 << 22)
#define CSI_LINE_ID2_INTST		(0x1 << 23)
#define CSI_LINE_ID3_INTST		(0x1 << 24)
#define CSI_DMA_LVDS_ID2_FIFO_OVERFLOW	(0x1 << 25)
#define CSI_DMA_LVDS_ID3_FIFO_OVERFLOW	(0x1 << 26)

#define CSI_FRAME_END_ID0	(CSI_FRAME0_END_ID0 |\
				 CSI_FRAME1_END_ID0)
#define CSI_FRAME_END_ID1	(CSI_FRAME0_END_ID1 |\
				 CSI_FRAME1_END_ID1)
#define CSI_FRAME_END_ID2	(CSI_FRAME0_END_ID2 |\
				 CSI_FRAME1_END_ID2)
#define CSI_FRAME_END_ID3	(CSI_FRAME0_END_ID3 |\
				 CSI_FRAME1_END_ID3)
#define CSI_FIFO_OVERFLOW	(CSI_DMA_Y_FIFO_OVERFLOW |	\
				 CSI_DMA_UV_FIFO_OVERFLOW |	\
				 CSI_CONFIG_FIFO_OVERFLOW |	\
				 CSI_RX_FIFO_OVERFLOW |	\
				 CSI_DMA_LVDS_ID2_FIFO_OVERFLOW |	\
				 CSI_DMA_LVDS_ID3_FIFO_OVERFLOW)
/* CIF_MIPI_LVDS_CTRL */
#define CIF_MIPI_LVDS_SW_DMA_IDLE		(0x1 << 16)
#define CIF_MIPI_LVDS_SW_PRESS_VALUE(val)	(((val) & 0x3) << 13)
#define CIF_MIPI_LVDS_SW_PRESS_ENABLE		(0x1 << 12)
#define CIF_MIPI_LVDS_SW_LVDS_WIDTH_8BITS	(0x0 << 9)
#define CIF_MIPI_LVDS_SW_LVDS_WIDTH_10BITS	(0x1 << 9)
#define CIF_MIPI_LVDS_SW_LVDS_WIDTH_12BITS	(0x2 << 9)
#define CIF_MIPI_LVDS_SW_SEL_LVDS		(0x1 << 8)
#define CIF_MIPI_LVDS_SW_HURRY_VALUE(val)	(((val) & 0x3) << 5)
#define CIF_MIPI_LVDS_SW_HURRY_ENABLE		(0x1 << 4)
#define CIF_MIPI_LVDS_SW_WATER_LINE_75		(0x0 << 1)
#define CIF_MIPI_LVDS_SW_WATER_LINE_50		(0x1 << 1)
#define CIF_MIPI_LVDS_SW_WATER_LINE_25		(0x2 << 1)
#define CIF_MIPI_LVDS_SW_WATER_LINE_00		(0x3 << 1)
#define CIF_MIPI_LVDS_SW_WATER_LINE_ENABLE	(0x1 << 0)
#define CIF_MIPI_LVDS_SW_DMA_IDLE_RK1808	(0x1 << 24)
#define CIF_MIPI_LVDS_SW_HURRY_VALUE_RK1808(val)	(((val) & 0x3) << 17)
#define CIF_MIPI_LVDS_SW_HURRY_ENABLE_RK1808	(0x1 << 16)
#define CIF_MIPI_LVDS_SW_WATER_LINE_75_RK1808	(0x0 << 0)
#define CIF_MIPI_LVDS_SW_WATER_LINE_50_RK1808	(0x1 << 0)
#define CIF_MIPI_LVDS_SW_WATER_LINE_25_RK1808	(0x2 << 0)
#define CIF_MIPI_LVDS_SW_WATER_LINE_00_RK1808	(0x3 << 0)
#define CIF_MIPI_LVDS_SW_WATER_LINE_ENABLE_RK1808	(0x1 << 4)

/* CSI Host Registers Define */
#define CSIHOST_N_LANES		0x04
#define CSIHOST_PHY_RSTZ	0x0c
#define CSIHOST_RESETN		0x10
#define CSIHOST_ERR1		0x20
#define CSIHOST_ERR2		0x24
#define CSIHOST_MSK1		0x28
#define CSIHOST_MSK2		0x2c
#define CSIHOST_CONTROL		0x40

#define SW_CPHY_EN(x)		((x) << 0)
#define SW_DSI_EN(x)		((x) << 4)
#define SW_DATATYPE_FS(x)	((x) << 8)
#define SW_DATATYPE_FE(x)	((x) << 14)
#define SW_DATATYPE_LS(x)	((x) << 20)
#define SW_DATATYPE_LE(x)	((x) << 26)

#define SW_FRM_END_ID0(x)	(((x) & CSI_FRAME_END_ID0) >> 8)
#define SW_FRM_END_ID1(x)	(((x) & CSI_FRAME_END_ID1) >> 10)
#define SW_FRM_END_ID2(x)	(((x) & CSI_FRAME_END_ID2) >> 12)
#define SW_FRM_END_ID3(x)	(((x) & CSI_FRAME_END_ID3) >> 14)

/* CIF LVDS SAV EAV Define */
#define SW_LVDS_EAV_ACT(code)	(((code) & 0xfff) << 16)
#define SW_LVDS_SAV_ACT(code)	(((code) & 0xfff) << 0)
#define SW_LVDS_EAV_BLK(code)	(((code) & 0xfff) << 16)
#define SW_LVDS_SAV_BLK(code)	(((code) & 0xfff) << 0)

#endif
