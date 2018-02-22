/*
 * vgmfmprm_ym2203.c
 *
 *  Created on: 2018/02/23
 *      Author: dsrphm
 */

#include <stdio.h>
#include "vgmfmprm.h"
#include "vgmutil.h"

#define CHIPNAME "YM2203"
#define CHS 3
#define REGS 46

static uint8_t ym2203reg[CHS][REGS] = {0};
static uint8_t regchg[CHS] = {0};

extern OPTIONS g_flg;
extern uint32_t fpos;

int format_ym2203(uint8_t);
int serialize_ym2203(uint8_t, uint8_t, uint8_t);

int vgmfmprm_ym2203(uint8_t aa, uint8_t dd)
{
	/* in mml2vgm
	'@ N No
	'@ AR DR SR RR SL TL KS ML DT AM SSGEG <- ym2203reg[ch][0]-[10]
	'@ AR DR SR RR SL TL KS ML DT AM SSGEG <- ym2203reg[ch][11]-[21]
	'@ AR DR SR RR SL TL KS ML DT AM SSGEG <- ym2203reg[ch][22]-[32]
	'@ AR DR SR RR SL TL KS ML DT AM SSGEG <- ym2203reg[ch][33]-[43]
	'@ AL FB <- ym2203reg[ch][44]-[45]
	*/
	uint8_t ch;
	uint8_t op;
	uint8_t opind[4] = {0, 2, 1, 3};

	ch = aa % 4;
	if (ch == 3){
		return 0;
	}
	switch (aa){
	case 0x28:
		ch = dd & 0x03;
		if (regchg[ch]){
			if (dd & 0xf0){ // 11110000
				format_ym2203(ch);
				regchg[ch] = 0;
			}
		}
		if (g_flg.r){
			printf("%08x %s[%d]reg: %02x %02x\n", fpos, CHIPNAME, ch + 1, aa, dd);
		}
		break;
	case 0x30 ... 0x3e:
		// DT ML
		op = opind[(aa - 0x30) / 4];
		regchg[ch] = 1;
		// MUL
		ym2203reg[ch][7 + 11 * op] = (dd & 0x0f) >> 0; //00001111
		// DT
		ym2203reg[ch][8 + 11 * op] = (dd & 0x70) >> 4; //01110000
		if (g_flg.r){
			printf("%08x %s[%d][%d]reg: %02x %02x\n", fpos, CHIPNAME, ch + 1, op + 1, aa, dd);
			serialize_ym2203(aa, dd, ch);
		}
		break;
	case 0x40 ... 0x4f:
		op = opind[(aa - 0x40) / 4];
		regchg[ch] = 1;
		// TL
		ym2203reg[ch][5 + 11 * op] = (dd & 0x7f) >> 0; //01111111
		if (g_flg.r){
			printf("%08x %s[%d][%d]reg: %02x %02x\n", fpos, CHIPNAME, ch + 1, op + 1, aa, dd);
			serialize_ym2203(aa, dd, ch);
		}
		break;
	case 0x50 ... 0x5f:
		// KS AR
		op = opind[(aa - 0x50) / 4];
		regchg[ch] = 1;
		// RS
		ym2203reg[ch][6 + 11 * op] = (dd & 0xc0) >> 6; //11000000
		// AR
		ym2203reg[ch][0 + 11 * op] = (dd & 0x1f) >> 0; //00011111
		if (g_flg.r){
			printf("%08x %s[%d][%d]reg: %02x %02x\n", fpos, CHIPNAME, ch + 1, op + 1, aa, dd);
			serialize_ym2203(aa, dd, ch);
		}
		break;
	case 0x60 ... 0x6f:
		// DR
		op = opind[(aa - 0x60) / 4];
		regchg[ch] = 1;
		ym2203reg[ch][1 + 11 * op] = (dd & 0x1f) >> 0; //00011111
		if (g_flg.r){
			printf("%08x %s[%d][%d]reg: %02x %02x\n", fpos, CHIPNAME, ch + 1, op + 1, aa, dd);
			serialize_ym2203(aa, dd, ch);
		}
		break;
	case 0x70 ... 0x7f:
		op = opind[(aa - 0x70) / 4];
		regchg[ch] = 1;
		// SR
		ym2203reg[ch][2 + 11 * op] = (dd & 0x1f) >> 0; //00011111
		if (g_flg.r){
			printf("%08x %s[%d][%d]reg: %02x %02x\n", fpos, CHIPNAME, ch + 1, op + 1, aa, dd);
			serialize_ym2203(aa, dd, ch);
		}
		break;
	case 0x80 ... 0x8f:
		// SL RR
		op = opind[(aa - 0x80) / 4];
		regchg[ch] = 1;
		// SL
		ym2203reg[ch][4 + 11 * op] = (dd & 0xf0) >> 4; //11110000
		// RR
		ym2203reg[ch][3 + 11 * op] = (dd & 0x0f) >> 0; //00001111
		if (g_flg.r){
			printf("%08x %s[%d][%d]reg: %02x %02x\n", fpos, CHIPNAME, ch + 1, op + 1, aa, dd);
			serialize_ym2203(aa, dd, ch);
		}
		break;
	case 0x90 ... 0x9f:
		// SSG-EG
		op = opind[(aa - 0x90) / 4];
		regchg[ch] = 1;
		ym2203reg[ch][10 + 11 * op] = (dd & 0x0f) >> 0; //00001111
		if (g_flg.r){
			printf("%08x %s[%d][%d]reg: %02x %02x\n", fpos, CHIPNAME, ch + 1, op + 1, aa, dd);
			serialize_ym2203(aa, dd, ch);
		}
		break;
	case 0xb0 ... 0xb3:
		// FB AL
		// FB
		regchg[ch] = 1;
		ym2203reg[ch][45] = (dd & 0x38) >> 3; //00111000
		// AL
		ym2203reg[ch][44] = (dd & 0x07) >> 0; //00000111
		if (g_flg.r){
			printf("%08x %s[%d]reg: %02x %02x\n", fpos, CHIPNAME, ch + 1, aa, dd);
			serialize_ym2203(aa, dd, ch);
		}
		break;
	default:
		break;
	}



	return 1;
}
int format_ym2203(uint8_t ch)
{
	/* in mml2vgm
	'@ N No
	'@ AR DR SR RR SL TL KS ML DT AM SSGEG <- ym2203reg[ch][0]-[10]
	'@ AR DR SR RR SL TL KS ML DT AM SSGEG <- ym2203reg[ch][11]-[21]
	'@ AR DR SR RR SL TL KS ML DT AM SSGEG <- ym2203reg[ch][22]-[32]
	'@ AR DR SR RR SL TL KS ML DT AM SSGEG <- ym2203reg[ch][33]-[43]
	'@ AL FB <- ym2203reg[ch][44]-[45]
	*/
	uint8_t op;
	uint8_t reg;

	printf("%s[%d]:\n", CHIPNAME, ch + 1);
	printf("'@ N No\n");
	printf("    AR  DR  SR  RR  SL  TL  KS  ML  DT  AM SSGEG\n");
	for (op = 0; op < 4; op++){
		printf("'@ ");
		for (reg = 0; reg < 11; reg++){
			printf("%03d,", ym2203reg[ch][op * 11 + reg]);
		}
		printf("\n");
	}
	printf("   ALG  FB\n");
	printf("'@ %03d,%03d\n\n", ym2203reg[ch][44], ym2203reg[ch][45]);
	return 1;
}
int serialize_ym2203(uint8_t aa, uint8_t dd, uint8_t ch)
{
	return 1;
}