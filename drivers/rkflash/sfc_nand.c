// SPDX-License-Identifier: GPL-2.0

/* Copyright (c) 2018 Rockchip Electronics Co. Ltd. */

#include <linux/bug.h>
#include <linux/delay.h>
#include <linux/kernel.h>
#include <linux/slab.h>

#include "rkflash_debug.h"
#include "rk_sftl.h"
#include "sfc_nand.h"

static u32 sfc_nand_get_ecc_status0(void);
static u32 sfc_nand_get_ecc_status1(void);
static u32 sfc_nand_get_ecc_status2(void);
static u32 sfc_nand_get_ecc_status3(void);
static u32 sfc_nand_get_ecc_status4(void);
static u32 sfc_nand_get_ecc_status5(void);
static u32 sfc_nand_get_ecc_status6(void);

static struct nand_info spi_nand_tbl[] = {
	/* TC58CVG0S0HxAIx */
	{ 0x98C2, 4, 0x40, 1, 1024, 0x00, 18, 0x8, 0, { 0x04, 0x08, 0xFF, 0xFF }, &sfc_nand_get_ecc_status0 },
	/* TC58CVG1S0HxAIx */
	{ 0x98CB, 4, 0x40, 2, 1024, 0x00, 19, 0x8, 0, { 0x04, 0x08, 0xFF, 0xFF }, &sfc_nand_get_ecc_status0 },
	/* TC58CVG2S0HRAIJ */
	{ 0x98ED, 8, 0x40, 1, 2048, 0x0C, 20, 0x8, 0, { 0x04, 0x0C, 0x08, 0x10 }, &sfc_nand_get_ecc_status0 },
	/* TC58CVG1S3HRAIJ */
	{ 0x98EB, 4, 0x40, 1, 2048, 0x0C, 19, 0x8, 0, { 0x04, 0x08, 0xFF, 0xFF }, &sfc_nand_get_ecc_status0 },
	/* TC58CVG0S3HRAIJ */
	{ 0x98E2, 4, 0x40, 1, 1024, 0x0C, 18, 0x8, 0, { 0x04, 0x08, 0xFF, 0xFF }, &sfc_nand_get_ecc_status0 },

	/* MX35LF1GE4AB */
	{ 0xC212, 4, 0x40, 1, 1024, 0x0C, 18, 0x4, 1, { 0x04, 0x08, 0xFF, 0xFF }, &sfc_nand_get_ecc_status1 },
	/* MX35LF2GE4AB */
	{ 0xC222, 4, 0x40, 2, 1024, 0x0C, 19, 0x4, 1, { 0x04, 0x08, 0xFF, 0xFF }, &sfc_nand_get_ecc_status1 },
	/* MX35LF2GE4AD */
	{ 0xC226, 4, 0x40, 1, 2048, 0x0C, 19, 0x8, 1, { 0x04, 0x08, 0xFF, 0xFF }, &sfc_nand_get_ecc_status0 },
	/* MX35LF4GE4AD */
	{ 0xC237, 8, 0x40, 1, 2048, 0x0C, 19, 0x8, 1, { 0x04, 0x08, 0x14, 0x18 }, &sfc_nand_get_ecc_status0 },
	/* MT29F1G01ZAC */
	{ 0x2C12, 4, 0x40, 1, 1024, 0x00, 18, 0x1, 1, { 0x04, 0x08, 0xFF, 0xFF }, &sfc_nand_get_ecc_status1 },

	/* GD5F1GQ4UAYIG */
	{ 0xC8F1, 4, 0x40, 1, 1024, 0x0C, 18, 0x8, 1, { 0x04, 0x08, 0xFF, 0xFF }, &sfc_nand_get_ecc_status0 },
	/* GD5F1GQ4RB9IGR */
	{ 0xC8D1, 4, 0x40, 1, 1024, 0x0C, 18, 0x8, 1, { 0x04, 0x08, 0xFF, 0xFF }, &sfc_nand_get_ecc_status3 },
	/* GD5F2GQ40BY2GR */
	{ 0xC8D2, 4, 0x40, 2, 1024, 0x0C, 19, 0x8, 1, { 0x04, 0x08, 0xFF, 0xFF }, &sfc_nand_get_ecc_status3 },
	/* GD5F1GQ5UEYIG */
	{ 0xC851, 4, 0x40, 1, 1024, 0x4C, 18, 0x4, 1, { 0x04, 0x14, 0xFF, 0xFF }, &sfc_nand_get_ecc_status2 },
	/* GD5F2GQ5UEYIG */
	{ 0xC852, 4, 0x40, 1, 2048, 0x4C, 19, 0x4, 1, { 0x04, 0x14, 0xFF, 0xFF }, &sfc_nand_get_ecc_status2 },
	/* GD5F1GQ4R */
	{ 0xC8C1, 4, 0x40, 1, 1024, 0x0C, 18, 0x8, 1, { 0x04, 0x08, 0xFF, 0xFF }, &sfc_nand_get_ecc_status3 },

	/* W25N01GV */
	{ 0xEFAA, 4, 0x40, 1, 1024, 0x4C, 18, 0x1, 0, { 0x04, 0x14, 0x24, 0xFF }, &sfc_nand_get_ecc_status1 },
	/* W25N01GW */
	{ 0xEFBA, 4, 0x40, 1, 1024, 0x4C, 18, 0x1, 0, { 0x04, 0x14, 0x24, 0xFF }, &sfc_nand_get_ecc_status1 },

	/* HYF2GQ4UAACAE */
	{ 0xC952, 4, 0x40, 1, 2048, 0x4C, 19, 0xE, 1, { 0x04, 0x24, 0xFF, 0xFF }, &sfc_nand_get_ecc_status0 },
	/* HYF1GQ4UDACAE */
	{ 0xC921, 4, 0x40, 1, 1024, 0x4C, 18, 0x4, 1, { 0x04, 0x14, 0xFF, 0xFF }, &sfc_nand_get_ecc_status0 },
	/* HYF1GQ4UPACAE */
	{ 0xC9A1, 4, 0x40, 1, 1024, 0x4C, 18, 0x1, 1, { 0x04, 0x14, 0xFF, 0xFF }, &sfc_nand_get_ecc_status1 },
	/* HYF2GQ4UDACAE */
	{ 0xC922, 4, 0x40, 1, 2048, 0x4C, 19, 0x4, 1, { 0x04, 0x14, 0xFF, 0xFF }, &sfc_nand_get_ecc_status0 },
	/* HYF2GQ4UHCCAE */
	{ 0xC95A, 4, 0x40, 1, 2048, 0x4C, 19, 0xE, 1, { 0x04, 0x24, 0xFF, 0xFF }, &sfc_nand_get_ecc_status0 },
	/* HYF4GQ4UAACBE */
	{ 0xC9D4, 8, 0x40, 1, 2048, 0x4C, 20, 0x4, 1, { 0x20, 0x40, 0x24, 0x44 }, &sfc_nand_get_ecc_status0 },

	/* FS35ND01G-S1 */
	{ 0xCDB1, 4, 0x40, 1, 1024, 0x0C, 18, 0x4, 1, { 0x10, 0x14, 0xFF, 0xFF }, &sfc_nand_get_ecc_status5 },
	/* FS35ND02G-S2 */
	{ 0xCDA2, 4, 0x40, 1, 2048, 0x00, 19, 0x4, 0, { 0x10, 0x14, 0xFF, 0xFF }, &sfc_nand_get_ecc_status5 },
	/* FS35ND01G-S1Y2 */
	{ 0xCDEA, 4, 0x40, 1, 1024, 0x4C, 18, 0x4, 0, { 0x04, 0x08, 0xFF, 0xFF }, &sfc_nand_get_ecc_status1 },
	/* FS35ND02G-S3Y2 */
	{ 0xCDEB, 4, 0x40, 1, 2048, 0x4C, 19, 0x4, 0, { 0x04, 0x08, 0xFF, 0xFF }, &sfc_nand_get_ecc_status1 },
	/* FS35ND04G-S2Y2 1*4096 */
	{ 0xCDEC, 4, 0x40, 2, 2048, 0x4C, 20, 0x4, 0, { 0x04, 0x08, 0xFF, 0xFF }, &sfc_nand_get_ecc_status1 },

	/* DS35Q1GA-IB */
	{ 0xE571, 4, 0x40, 1, 1024, 0x0C, 18, 0x4, 1, { 0x04, 0x14, 0xFF, 0xFF }, &sfc_nand_get_ecc_status1 },
	/* DS35Q2GA-IB */
	{ 0xE572, 4, 0x40, 2, 1024, 0x0C, 19, 0x4, 1, { 0x04, 0x14, 0xFF, 0xFF }, &sfc_nand_get_ecc_status1 },
	/* DS35M1GA-1B */
	{ 0xE521, 4, 0x40, 1, 1024, 0x0C, 18, 0x4, 1, { 0x04, 0x14, 0xFF, 0xFF }, &sfc_nand_get_ecc_status1 },

	/* EM73C044VCC-H */
	{ 0xD522, 4, 0x40, 1, 1024, 0x0C, 18, 0x8, 1, { 0x04, 0x14, 0xFF, 0xFF }, &sfc_nand_get_ecc_status0 },
	/* EM73D044VCE-H */
	{ 0xD520, 4, 0x40, 1, 2048, 0x0C, 19, 0x8, 1, { 0x04, 0x14, 0xFF, 0xFF }, &sfc_nand_get_ecc_status0 },
	/* EM73E044SNA-G */
	{ 0xD503, 8, 0x40, 1, 2048, 0x4C, 20, 0x8, 1, { 0x04, 0x28, 0x08, 0x2C }, &sfc_nand_get_ecc_status0 },

	/* XT26G02A */
	{ 0x0BE2, 4, 0x40, 1, 2048, 0x4C, 19, 0x1, 1, { 0x08, 0x0C, 0xFF, 0xFF }, &sfc_nand_get_ecc_status4 },
	/* XT26G01A */
	{ 0x0BE1, 4, 0x40, 1, 1024, 0x4C, 18, 0x1, 1, { 0x08, 0x0C, 0xFF, 0xFF }, &sfc_nand_get_ecc_status4 },
	/* XT26G04A */
	{ 0x0BE3, 4, 0x80, 1, 2048, 0x4C, 20, 0x1, 1, { 0x08, 0x0C, 0xFF, 0xFF }, &sfc_nand_get_ecc_status4 },
	/* XT26G01B */
	{ 0x0BF1, 4, 0x40, 1, 1024, 0x4C, 18, 0x1, 1, { 0x08, 0x0C, 0xFF, 0xFF }, &sfc_nand_get_ecc_status4 },
	/* XT26G02B */
	{ 0x0BF2, 4, 0x40, 1, 2048, 0x4C, 19, 0x1, 1, { 0x08, 0x0C, 0xFF, 0xFF }, &sfc_nand_get_ecc_status5 },
	/* XT26G02E */
	{ 0x2C24, 4, 0x40, 1, 2048, 0x4C, 19, 0x1, 1, { 0x20, 0x24, 0xFF, 0xFF }, &sfc_nand_get_ecc_status6 },

	/* IS37SML01G1 */
	{ 0xC821, 4, 0x40, 1, 1024, 0x00, 18, 0x1, 0, { 0x08, 0x0C, 0xFF, 0xFF }, &sfc_nand_get_ecc_status1 },
	/* F50L1G41LB */
	{ 0xC801, 4, 0x40, 1, 1024, 0x4C, 18, 0x1, 0, { 0x14, 0x24, 0xFF, 0xFF }, &sfc_nand_get_ecc_status1 },
	/* ATO25D1GA */
	{ 0x9B12, 4, 0x40, 1, 1024, 0x40, 18, 0x1, 1, { 0x14, 0x24, 0xFF, 0xFF }, &sfc_nand_get_ecc_status1 },
	/* FM25S01 */
	{ 0xA1A1, 4, 0x40, 1, 1024, 0x4C, 18, 0x1, 1, { 0x00, 0x04, 0xFF, 0xFF }, &sfc_nand_get_ecc_status1 },
};

static struct nand_info *p_nand_info;
static u32 gp_page_buf[SFC_NAND_PAGE_MAX_SIZE / 4];
static struct SFNAND_DEV sfc_nand_dev;

static struct nand_info *sfc_nand_get_info(u8 *nand_id)
{
	u32 i;
	u32 id = (nand_id[0] << 8) | (nand_id[1] << 0);

	for (i = 0; i < ARRAY_SIZE(spi_nand_tbl); i++) {
		if (spi_nand_tbl[i].id == id)
			return &spi_nand_tbl[i];
	}

	return NULL;
}

static int sfc_nand_write_en(void)
{
	int ret;
	struct rk_sfc_op op;

	op.sfcmd.d32 = 0;
	op.sfcmd.b.cmd = CMD_WRITE_EN;

	op.sfctrl.d32 = 0;

	ret = sfc_request(&op, 0, NULL, 0);
	return ret;
}

static int sfc_nand_rw_preset(void)
{
	int ret;
	struct rk_sfc_op op;

	op.sfcmd.d32 = 0;
	op.sfcmd.b.cmd = 0xff;
	op.sfcmd.b.cs = 2;

	op.sfctrl.d32 = 0;
	op.sfctrl.b.datalines = 2;
	op.sfctrl.b.cmdlines = 2;
	op.sfctrl.b.addrlines = 2;

	ret = sfc_request(&op, 0, NULL, 0);
	return ret;
}

static int sfc_nand_read_feature(u8 addr, u8 *data)
{
	int ret;
	struct rk_sfc_op op;

	op.sfcmd.d32 = 0;
	op.sfcmd.b.cmd = 0x0F;
	op.sfcmd.b.addrbits = SFC_ADDR_XBITS;

	op.sfctrl.d32 = 0;
	op.sfctrl.b.addrbits = 8;

	*data = 0;

	ret = sfc_request(&op, addr, data, 1);

	if (ret != SFC_OK)
		return ret;

	return SFC_OK;
}

static int sfc_nand_write_feature(u32 addr, u8 status)
{
	int ret;
	struct rk_sfc_op op;

	sfc_nand_write_en();

	op.sfcmd.d32 = 0;
	op.sfcmd.b.cmd = 0x1F;
	op.sfcmd.b.addrbits = SFC_ADDR_XBITS;
	op.sfcmd.b.rw = SFC_WRITE;

	op.sfctrl.d32 = 0;
	op.sfctrl.b.addrbits = 8;

	ret = sfc_request(&op, addr, &status, 1);

	if (ret != SFC_OK)
		return ret;

	return ret;
}

static int sfc_nand_wait_busy(u8 *data, int timeout)
{
	int ret;
	int i;
	u8 status;

	*data = 0;

	for (i = 0; i < timeout; i++) {
		ret = sfc_nand_read_feature(0xC0, &status);

		if (ret != SFC_OK)
			return ret;

		*data = status;

		if (!(status & (1 << 0)))
			return SFC_OK;

		sfc_delay(1);
	}

	return SFC_NAND_WAIT_TIME_OUT;
}

/*
 * ecc default:
 * ecc bits: 0xC0[4,5]
 * 0b00, No bit errors were detected
 * 0b01, Bit errors were detected and corrected.
 * 0b10, Multiple bit errors were detected and not corrected.
 * 0b11, Bits errors were detected and corrected, bit error count
 *	exceed the bit flip detection threshold
 */
static u32 sfc_nand_get_ecc_status0(void)
{
	u32 ret;
	u32 i;
	u8 ecc;
	u8 status;
	u32 timeout = 1000 * 1000;

	for (i = 0; i < timeout; i++) {
		ret = sfc_nand_read_feature(0xC0, &status);

		if (ret != SFC_OK)
			return SFC_NAND_ECC_ERROR;

		if (!(status & (1 << 0)))
			break;

		sfc_delay(1);
	}

	ecc = (status >> 4) & 0x03;

	if (ecc <= 1)
		ret = SFC_NAND_ECC_OK;
	else if (ecc == 2)
		ret = (u32)SFC_NAND_ECC_ERROR;
	else
		ret = SFC_NAND_ECC_REFRESH;

	return ret;
}

/*
 * ecc spectial type1:
 * ecc bits: 0xC0[4,5]
 * 0b00, No bit errors were detected;
 * 0b01, Bits errors were detected and corrected, bit error count
 *	may reach the bit flip detection threshold;
 * 0b10, Multiple bit errors were detected and not corrected;
 * 0b11, Reserved.
 */
static u32 sfc_nand_get_ecc_status1(void)
{
	u32 ret;
	u32 i;
	u8 ecc;
	u8 status;
	u32 timeout = 1000 * 1000;

	for (i = 0; i < timeout; i++) {
		ret = sfc_nand_read_feature(0xC0, &status);

		if (ret != SFC_OK)
			return SFC_NAND_ECC_ERROR;

		if (!(status & (1 << 0)))
			break;

		sfc_delay(1);
	}

	ecc = (status >> 4) & 0x03;

	if (ecc == 0)
		ret = SFC_NAND_ECC_OK;
	else if (ecc == 1)
		ret = SFC_NAND_ECC_REFRESH;
	else
		ret = (u32)SFC_NAND_ECC_ERROR;

	return ret;
}

/*
 * ecc spectial type2:
 * ecc bits: 0xC0[4,5] 0xF0[4,5]
 * [0b0000, 0b0011], No bit errors were detected;
 * [0b0100, 0b0111], Bit errors were detected and corrected. Not
 *	reach Flipping Bits;
 * [0b1000, 0b1011], Multiple bit errors were detected and
 *	not corrected.
 * [0b1100, 0b1111], reserved.
 */
static u32 sfc_nand_get_ecc_status2(void)
{
	u32 ret;
	u32 i;
	u8 ecc;
	u8 status, status1;
	u32 timeout = 1000 * 1000;

	for (i = 0; i < timeout; i++) {
		ret = sfc_nand_read_feature(0xC0, &status);

		if (ret != SFC_OK)
			return SFC_NAND_ECC_ERROR;

		ret = sfc_nand_read_feature(0xF0, &status1);

		if (ret != SFC_OK)
			return SFC_NAND_ECC_ERROR;

		if (!(status & (1 << 0)))
			break;

		sfc_delay(1);
	}

	ecc = (status >> 4) & 0x03;
	ecc = (ecc << 2) | ((status1 >> 4) & 0x03);

	if (ecc < 7)
		ret = SFC_NAND_ECC_OK;
	else if (ecc == 7)
		ret = SFC_NAND_ECC_REFRESH;
	else
		ret = (u32)SFC_NAND_ECC_ERROR;

	return ret;
}

/*
 * ecc spectial type3:
 * ecc bits: 0xC0[4,5] 0xF0[4,5]
 * [0b0000, 0b0011], No bit errors were detected;
 * [0b0100, 0b0111], Bit errors were detected and corrected. Not
 *	reach Flipping Bits;
 * [0b1000, 0b1011], Multiple bit errors were detected and
 *	not corrected.
 * [0b1100, 0b1111], Bit error count equals the bit flip
 *	detectio nthreshold
 */
static u32 sfc_nand_get_ecc_status3(void)
{
	u32 ret;
	u32 i;
	u8 ecc;
	u8 status, status1;
	u32 timeout = 1000 * 1000;

	for (i = 0; i < timeout; i++) {
		ret = sfc_nand_read_feature(0xC0, &status);

		if (ret != SFC_OK)
			return SFC_NAND_ECC_ERROR;

		ret = sfc_nand_read_feature(0xF0, &status1);

		if (ret != SFC_OK)
			return SFC_NAND_ECC_ERROR;

		if (!(status & (1 << 0)))
			break;

		sfc_delay(1);
	}

	ecc = (status >> 4) & 0x03;
	ecc = (ecc << 2) | ((status1 >> 4) & 0x03);

	if (ecc < 7)
		ret = SFC_NAND_ECC_OK;
	else if (ecc == 7 || ecc >= 12)
		ret = SFC_NAND_ECC_REFRESH;
	else
		ret = (u32)SFC_NAND_ECC_ERROR;

	return ret;
}

/*
 * ecc spectial type4:
 * ecc bits: 0xC0[2,5]
 * [0b0000], No bit errors were detected;
 * [0b0001, 0b0111], Bit errors were detected and corrected. Not
 *	reach Flipping Bits;
 * [0b1000], Multiple bit errors were detected and
 *	not corrected.
 * [0b1100], Bit error count equals the bit flip
 *	detectionthreshold
 * else, reserved
 */
static u32 sfc_nand_get_ecc_status4(void)
{
	u32 ret;
	u32 i;
	u8 ecc;
	u8 status;
	u32 timeout = 1000 * 1000;

	for (i = 0; i < timeout; i++) {
		ret = sfc_nand_read_feature(0xC0, &status);

		if (ret != SFC_OK)
			return SFC_NAND_ECC_ERROR;

		if (!(status & (1 << 0)))
			break;

		sfc_delay(1);
	}

	ecc = (status >> 2) & 0x0f;

	if (ecc < 7)
		ret = SFC_NAND_ECC_OK;
	else if (ecc == 7 || ecc == 12)
		ret = SFC_NAND_ECC_REFRESH;
	else
		ret = (u32)SFC_NAND_ECC_ERROR;

	return ret;
}

/*
 * ecc spectial type5:
 * ecc bits: 0xC0[4,6]
 * [0b000], No bit errors were detected;
 * [0b001, 0b011], Bit errors were detected and corrected. Not
 *	reach Flipping Bits;
 * [0b100], Bit error count equals the bit flip
 *	detectionthreshold
 * [0b101, 0b110], Reserved;
 * [0b111], Multiple bit errors were detected and
 *	not corrected.
 */
static u32 sfc_nand_get_ecc_status5(void)
{
	u32 ret;
	u32 i;
	u8 ecc;
	u8 status;
	u32 timeout = 1000 * 1000;

	for (i = 0; i < timeout; i++) {
		ret = sfc_nand_read_feature(0xC0, &status);

		if (ret != SFC_OK)
			return SFC_NAND_ECC_ERROR;

		if (!(status & (1 << 0)))
			break;

		sfc_delay(1);
	}

	ecc = (status >> 4) & 0x07;

	if (ecc < 4)
		ret = SFC_NAND_ECC_OK;
	else if (ecc == 4)
		ret = SFC_NAND_ECC_REFRESH;
	else
		ret = (u32)SFC_NAND_ECC_ERROR;

	return ret;
}

/*
 * ecc spectial type6:
 * ecc bits: 0xC0[4,6]
 * [0b000], No bit errors were detected;
 * [0b001], 1-3 Bit errors were detected and corrected. Not
 *	reach Flipping Bits;
 * [0b010], Multiple bit errors were detected and
 *	not corrected.
 * [0b011], 4-6 Bit errors were detected and corrected. Not
 *	reach Flipping Bits;
 * [0b101], Bit error count equals the bit flip
 *	detectionthreshold
 * others, Reserved.
 */
static u32 sfc_nand_get_ecc_status6(void)
{
	u32 ret;
	u32 i;
	u8 ecc;
	u8 status;
	u32 timeout = 1000 * 1000;

	for (i = 0; i < timeout; i++) {
		ret = sfc_nand_read_feature(0xC0, &status);

		if (ret != SFC_OK)
			return SFC_NAND_ECC_ERROR;

		if (!(status & (1 << 0)))
			break;

		sfc_delay(1);
	}

	ecc = (status >> 4) & 0x07;

	if (ecc == 0 || ecc == 1 || ecc == 3)
		ret = SFC_NAND_ECC_OK;
	else if (ecc == 5)
		ret = SFC_NAND_ECC_REFRESH;
	else
		ret = (u32)SFC_NAND_ECC_ERROR;

	return ret;
}

u32 sfc_nand_erase_block(u8 cs, u32 addr)
{
	int ret;
	struct rk_sfc_op op;
	u8 status;

	rkflash_print_dio("%s %x\n", __func__, addr);
	op.sfcmd.d32 = 0;
	op.sfcmd.b.cmd = 0xd8;
	op.sfcmd.b.addrbits = SFC_ADDR_24BITS;
	op.sfcmd.b.rw = SFC_WRITE;

	op.sfctrl.d32 = 0;

	sfc_nand_write_en();
	ret = sfc_request(&op, addr, NULL, 0);

	if (ret != SFC_OK)
		return ret;

	ret = sfc_nand_wait_busy(&status, 1000 * 1000);

	if (status & (1 << 2))
		return SFC_NAND_PROG_ERASE_ERROR;

	return ret;
}

u32 sfc_nand_prog_page_raw(u8 cs, u32 addr, u32 *p_page_buf)
{
	int ret;
	u32 plane;
	struct rk_sfc_op op;
	u8 status;
	u32 page_size = SFC_NAND_SECTOR_FULL_SIZE * p_nand_info->sec_per_page;

	rkflash_print_dio("%s %x %x\n", __func__, addr, p_page_buf[0]);
	sfc_nand_write_en();

	if (sfc_nand_dev.prog_lines == DATA_LINES_X4 &&
	    p_nand_info->feature & FEA_SOFT_QOP_BIT &&
	    sfc_get_version() < SFC_VER_3)
		sfc_nand_rw_preset();

	op.sfcmd.d32 = 0;
	op.sfcmd.b.cmd = sfc_nand_dev.page_prog_cmd;
	op.sfcmd.b.addrbits = SFC_ADDR_XBITS;
	op.sfcmd.b.rw = SFC_WRITE;

	op.sfctrl.d32 = 0;
	op.sfctrl.b.datalines = sfc_nand_dev.prog_lines;
	op.sfctrl.b.addrbits = 16;
	plane = p_nand_info->plane_per_die == 2 ? ((addr >> 6) & 0x1) << 12 : 0;
	sfc_request(&op, plane, p_page_buf, page_size);

	op.sfcmd.d32 = 0;
	op.sfcmd.b.cmd = 0x10;
	op.sfcmd.b.addrbits = SFC_ADDR_24BITS;
	op.sfcmd.b.rw = SFC_WRITE;

	op.sfctrl.d32 = 0;
	ret = sfc_request(&op, addr, p_page_buf, 0);

	if (ret != SFC_OK)
		return ret;

	ret = sfc_nand_wait_busy(&status, 1000 * 1000);

	if (status & (1 << 3))
		return SFC_NAND_PROG_ERASE_ERROR;

	return ret;
}

u32 sfc_nand_prog_page(u8 cs, u32 addr, u32 *p_data, u32 *p_spare)
{
	int ret;
	u32 sec_per_page = p_nand_info->sec_per_page;
	u32 data_size = sec_per_page * SFC_NAND_SECTOR_SIZE;
	struct nand_mega_area *meta = &p_nand_info->meta;

	memcpy(gp_page_buf, p_data, data_size);
	memset(&gp_page_buf[data_size / 4], 0xff, sec_per_page * 16);
	gp_page_buf[(data_size + meta->off0) / 4] = p_spare[0];
	gp_page_buf[(data_size + meta->off1) / 4] = p_spare[1];

	if (sec_per_page == 8) {
		gp_page_buf[(data_size + meta->off2) / 4] = p_spare[2];
		gp_page_buf[(data_size + meta->off3) / 4] = p_spare[3];
	}

	ret = sfc_nand_prog_page_raw(cs, addr, gp_page_buf);

	return ret;
}

u32 sfc_nand_read(u32 row, u32 *p_page_buf, u32 column, u32 len)
{
	int ret;
	u32 plane;
	struct rk_sfc_op op;
	u32 ecc_result;
	u8 status;

	op.sfcmd.d32 = 0;
	op.sfcmd.b.cmd = 0x13;
	op.sfcmd.b.rw = SFC_WRITE;
	op.sfcmd.b.addrbits = SFC_ADDR_24BITS;

	op.sfctrl.d32 = 0;

	sfc_request(&op, row, p_page_buf, 0);

	if (sfc_nand_dev.read_lines == DATA_LINES_X4 &&
	    p_nand_info->feature & FEA_SOFT_QOP_BIT &&
	    sfc_get_version() < SFC_VER_3)
		sfc_nand_rw_preset();

	sfc_nand_wait_busy(&status, 1000 * 1000);
	ecc_result = p_nand_info->ecc_status();

	op.sfcmd.d32 = 0;
	op.sfcmd.b.cmd = sfc_nand_dev.page_read_cmd;
	op.sfcmd.b.addrbits = SFC_ADDR_XBITS;
	op.sfcmd.b.dummybits = 8;

	op.sfctrl.d32 = 0;
	op.sfctrl.b.datalines = sfc_nand_dev.read_lines;
	op.sfctrl.b.addrbits = 16;

	plane = p_nand_info->plane_per_die == 2 ? ((row >> 6) & 0x1) << 12 : 0;
	ret = sfc_request(&op, plane | column, p_page_buf, len);
	rkflash_print_dio("%s %x %x\n", __func__, row, p_page_buf[0]);

	if (ret != SFC_OK)
		return SFC_NAND_HW_ERROR;

	return ecc_result;
}

u32 sfc_nand_read_page_raw(u8 cs, u32 addr, u32 *p_page_buf)
{
	u32 page_size = SFC_NAND_SECTOR_FULL_SIZE * p_nand_info->sec_per_page;

	return sfc_nand_read(addr, gp_page_buf, 0, page_size);
}

u32 sfc_nand_read_page(u8 cs, u32 addr, u32 *p_data, u32 *p_spare)
{
	u32 ret;
	u32 sec_per_page = p_nand_info->sec_per_page;
	u32 data_size = sec_per_page * SFC_NAND_SECTOR_SIZE;
	struct nand_mega_area *meta = &p_nand_info->meta;

	ret = sfc_nand_read_page_raw(cs, addr, gp_page_buf);
	memcpy(p_data, gp_page_buf, data_size);
	p_spare[0] = gp_page_buf[(data_size + meta->off0) / 4];
	p_spare[1] = gp_page_buf[(data_size + meta->off1) / 4];

	if (p_nand_info->sec_per_page == 8) {
		p_spare[2] = gp_page_buf[(data_size + meta->off2) / 4];
		p_spare[3] = gp_page_buf[(data_size + meta->off3) / 4];
	}

	if (ret == SFC_NAND_HW_ERROR)
		ret = SFC_NAND_ECC_ERROR;

	if (ret != SFC_NAND_ECC_OK) {
		rkflash_print_error("%s[0x%x], ret=0x%x\n", __func__, addr, ret);

		if (p_data)
			rkflash_print_hex("data:", p_data, 4, 8);

		if (p_spare)
			rkflash_print_hex("spare:", p_spare, 4, 2);
	}

	return ret;
}

u32 sfc_nand_check_bad_block(u8 cs, u32 addr)
{
	u32 ret;
	u32 data_size = p_nand_info->sec_per_page * SFC_NAND_SECTOR_SIZE;
	u32 marker = 0;

	ret = sfc_nand_read(addr, &marker, data_size, 2);

	/* unify with mtd framework */
	if (ret == SFC_NAND_ECC_ERROR)
		rkflash_print_error("%s page= %x ret= %x spare= %x\n",
				    __func__, addr, ret, marker);

	/* Original bad block */
	if ((u16)marker != 0xffff)
		return true;

	return false;
}

u32 sfc_nand_mark_bad_block(u8 cs, u32 addr)
{
	u32 ret;
	u32 data_size = p_nand_info->sec_per_page * SFC_NAND_SECTOR_SIZE;

	ret = sfc_nand_read_page_raw(cs, addr, gp_page_buf);

	if (ret)
		return SFC_NAND_HW_ERROR;

	gp_page_buf[data_size / 4] = 0x0;
	ret = sfc_nand_prog_page_raw(cs, addr, gp_page_buf);

	if (ret)
		return SFC_NAND_HW_ERROR;

	return ret;
}

int sfc_nand_read_id(u8 *data)
{
	int ret;
	struct rk_sfc_op op;

	op.sfcmd.d32 = 0;
	op.sfcmd.b.cmd = CMD_READ_JEDECID;
	op.sfcmd.b.addrbits = SFC_ADDR_XBITS;

	op.sfctrl.d32 = 0;
	op.sfctrl.b.addrbits = 8;

	ret = sfc_request(&op, 0, data, 3);

	return ret;
}

/*
 * Read the 1st page's 1st byte of a phy_blk
 * If not FF, it's bad blk
 */
static int sfc_nand_get_bad_block_list(u16 *table, u32 die)
{
	u32 bad_cnt, page;
	u32 blk_per_die;
	u16 blk;

	rkflash_print_info("%s\n", __func__);

	bad_cnt = 0;
	blk_per_die = p_nand_info->plane_per_die *
		      p_nand_info->blk_per_plane;

	for (blk = 0; blk < blk_per_die; blk++) {
		page = (blk + blk_per_die * die) *
		       p_nand_info->page_per_blk;

		if (sfc_nand_check_bad_block(die, page)) {
			table[bad_cnt++] = blk;
			rkflash_print_error("die[%d], bad_blk[%d]\n", die, blk);
		}
	}

	return (int)bad_cnt;
}

void sfc_nand_ftl_ops_init(void)
{
	/* para init */
	g_nand_phy_info.nand_type	= 1;
	g_nand_phy_info.die_num		= 1;
	g_nand_phy_info.plane_per_die	= p_nand_info->plane_per_die;
	g_nand_phy_info.blk_per_plane	= p_nand_info->blk_per_plane;
	g_nand_phy_info.page_per_blk	= p_nand_info->page_per_blk;
	g_nand_phy_info.page_per_slc_blk = p_nand_info->page_per_blk;
	g_nand_phy_info.byte_per_sec	= SFC_NAND_SECTOR_SIZE;
	g_nand_phy_info.sec_per_page	= p_nand_info->sec_per_page;
	g_nand_phy_info.sec_per_blk	= p_nand_info->sec_per_page *
					  p_nand_info->page_per_blk;
	g_nand_phy_info.reserved_blk	= 8;
	g_nand_phy_info.blk_per_die	= p_nand_info->plane_per_die *
					  p_nand_info->blk_per_plane;
	g_nand_phy_info.ecc_bits	= p_nand_info->max_ecc_bits;

	/* driver register */
	g_nand_ops.get_bad_blk_list	= sfc_nand_get_bad_block_list;
	g_nand_ops.erase_blk		= sfc_nand_erase_block;
	g_nand_ops.prog_page		= sfc_nand_prog_page;
	g_nand_ops.read_page		= sfc_nand_read_page;
	g_nand_ops.bch_sel		= NULL;
}

static int sfc_nand_enable_QE(void)
{
	int ret = SFC_OK;
	u8 status;

	ret = sfc_nand_read_feature(0xB0, &status);

	if (ret != SFC_OK)
		return ret;

	if (status & 1)   /* is QE bit set */
		return SFC_OK;

	status |= 1;

	return sfc_nand_write_feature(0xB0, status);
}

u32 sfc_nand_init(void)
{
	u8 status, id_byte[8];

	sfc_nand_read_id(id_byte);
	rkflash_print_error("sfc_nand id: %x %x %x\n",
			    id_byte[0], id_byte[1], id_byte[2]);

	if (id_byte[0] == 0xFF || id_byte[0] == 0x00)
		return (u32)FTL_NO_FLASH;

	p_nand_info = sfc_nand_get_info(id_byte);

	if (!p_nand_info)
		return (u32)FTL_UNSUPPORTED_FLASH;

	sfc_nand_dev.manufacturer = id_byte[0];
	sfc_nand_dev.mem_type = id_byte[1];
	sfc_nand_dev.capacity = p_nand_info->density;
	sfc_nand_dev.block_size = p_nand_info->page_per_blk * p_nand_info->sec_per_page;
	sfc_nand_dev.page_size = p_nand_info->sec_per_page;

	/* disable block lock */
	sfc_nand_write_feature(0xA0, 0);
	sfc_nand_dev.read_lines = DATA_LINES_X1;
	sfc_nand_dev.prog_lines = DATA_LINES_X1;
	sfc_nand_dev.page_read_cmd = 0x03;
	sfc_nand_dev.page_prog_cmd = 0x02;

	if (p_nand_info->feature & FEA_4BIT_READ) {
		if ((p_nand_info->has_qe_bits && sfc_nand_enable_QE() == SFC_OK) ||
		    !p_nand_info->has_qe_bits) {
			sfc_nand_dev.read_lines = DATA_LINES_X4;
			sfc_nand_dev.page_read_cmd = 0x6b;
		}
	}

	if (p_nand_info->feature & FEA_4BIT_PROG &&
	    sfc_nand_dev.read_lines == DATA_LINES_X4) {
		sfc_nand_dev.prog_lines = DATA_LINES_X4;
		sfc_nand_dev.page_prog_cmd = 0x32;
	}

	sfc_nand_read_feature(0xA0, &status);
	rkflash_print_info("sfc_nand A0 = 0x%x\n", status);
	sfc_nand_read_feature(0xB0, &status);
	rkflash_print_info("sfc_nand B0 = 0x%x\n", status);
	rkflash_print_info("read_lines = %x\n", sfc_nand_dev.read_lines);
	rkflash_print_info("prog_lines = %x\n", sfc_nand_dev.prog_lines);
	rkflash_print_info("page_read_cmd = %x\n", sfc_nand_dev.page_read_cmd);
	rkflash_print_info("page_prog_cmd = %x\n", sfc_nand_dev.page_prog_cmd);

	return SFC_OK;
}

void sfc_nand_deinit(void)
{
	/* to-do */
}

struct SFNAND_DEV *sfc_nand_get_private_dev(void)
{
	return &sfc_nand_dev;
}

struct nand_info *sfc_nand_get_nand_info(void)
{
	return p_nand_info;
}
