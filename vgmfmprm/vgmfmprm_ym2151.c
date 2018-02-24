/*
 * vgmfmprm_ym2151.c
 *
 *  Created on: 2018/02/23
 *      Author: dsrphm
 */

#include <stdio.h>
#include "vgmfmprm.h"
#include "vgmutil.h"

#define CHIPNAME "YM2151"
#define CHS 8
#define REGS 46

extern OPTIONS g_flg;

static uint8_t fmprm[CHS][REGS] = {0};
static uint8_t regchg[CHS] = {0};

int vgmfmprm_ym2151(uint8_t aa, uint8_t dd)
{
	/* in mml2vgm
	'@ M No
	'@ AR DR SR RR SL TL KS ML DT1 DT2 AME <- ym2151reg[ch][0]-[10]
	'@ AR DR SR RR SL TL KS ML DT1 DT2 AME <- ym2151reg[ch][11]-[21]
	'@ AR DR SR RR SL TL KS ML DT1 DT2 AME <- ym2151reg[ch][22]-[32]
	'@ AR DR SR RR SL TL KS ML DT1 DT2 AME <- ym2151reg[ch][33]-[43]
	'@ AL FB  <- ym2151reg[ch][44]-[45]
	*/
	/* in YM2151 datasheet
	'@ M No
	'@ AR D1R D2R RR D1L TL KS MUL DT1 DT2 AME <- ym2151reg[ch][0]-[10]
	'@ AR D1R D2R RR D1L TL KS MUL DT1 DT2 AME <- ym2151reg[ch][11]-[21]
	'@ AR D1R D2R RR D1L TL KS MUL DT1 DT2 AME <- ym2151reg[ch][22]-[32]
	'@ AR D1R D2R RR D1L TL KS MUL DT1 DT2 AME <- ym2151reg[ch][33]-[43]
	'@ CON FL  <- ym2151reg[ch][44]-[45]
	*/
	uint8_t ch;
	uint8_t op;

	uint8_t opind[4] = {0, 2, 1, 3}; // aa: OP1 OP3 OP2 OP4

	switch (aa){
	case 0x08:
		// 音色関係のレジスタが変更され、Key-Onとなった時点で
		// その時の音色定義を出力する
		ch = dd & 0x07;
		if (regchg[ch]){
			if (dd & 0x78){ // 01111000
				formatM(CHIPNAME, ch, samples, 234, fmprm[ch]);
				regchg[ch] = 0;
			}
		}
//		if (g_flg.r){
//			printf("%08x %s[%d]reg: %02x %02x\n", fpos, CHIPNAME, ch + 1, aa, dd);
//		}
		break;
	case 0x20 ... 0x27:
		// AL=CON FB=FL
		ch = aa - 0x20;
		regchg[ch] = 1;
		// CON
		fmprm[ch][44] = (dd & 0x07) >> 0; // 00000111
		// FL
		fmprm[ch][45] = (dd & 0x38) >> 3; // 00111000
		if (g_flg.r){
			printf("%08x %s[%d]reg: %02x %02x\n", fpos, CHIPNAME, ch + 1, aa, dd);
		}
		break;
	case 0x40 ... 0x5f:
		// DT1 ML=MUL
		ch = (aa - 0x40) % CHS;
		op = opind[(aa - 0x40) / CHS];
		regchg[ch] = 1;
		// MUL
		fmprm[ch][7 + 11 * op] = (dd & 0x0f) >> 0; // 00001111
		// DT1
		fmprm[ch][8 + 11 * op] = (dd & 0x70) >> 4; // 01110000
		if (g_flg.r){
			printf("%08x %s[%d]reg: %02x %02x\n", fpos, CHIPNAME, ch + 1, aa, dd);
		}
		break;
	case 0x60 ... 0x7f:
		// TL
		ch = (aa - 0x60) % CHS;
		op = opind[(aa - 0x60) / CHS];
		regchg[ch] = 1;
		fmprm[ch][5 + 11 * op] = (dd & 0x7f) >> 0; // 01111111
		if (g_flg.r){
			printf("%08x %s[%d]reg: %02x %02x\n", fpos, CHIPNAME, ch + 1, aa, dd);
		}
		break;
	case 0x80 ... 0x9f:
		// KS AR
		ch = (aa - 0x80) % CHS;
		op = opind[(aa - 0x80) / CHS];
		regchg[ch] = 1;
		// KS
		fmprm[ch][6 + 11 * op] = (dd & 0xc0) >> 6; // 11000000
		// AR
		fmprm[ch][0 + 11 * op] = (dd & 0x1f) >> 0; // 00011111
		if (g_flg.r){
			printf("%08x %s[%d]reg: %02x %02x\n", fpos, CHIPNAME, ch + 1, aa, dd);
		}
		break;
	case 0xa0 ... 0xbf:
		// AME DR=D1R
		ch = (aa - 0xa0) % CHS;
		op = opind[(aa - 0xa0) / CHS];
		regchg[ch] = 1;
		// AME
		fmprm[ch][10 + 11 * op] = (dd & 0x80) >> 7; // 10000000
		// D1R
		fmprm[ch][1 + 11 * op] = (dd & 0x1f) >> 0; // 00011111
		if (g_flg.r){
			printf("%08x %s[%d]reg: %02x %02x\n", fpos, CHIPNAME, ch + 1, aa, dd);
		}
		break;
	case 0xc0 ... 0xdf:
		// DT2 SR=D2R
		ch = (aa - 0xc0) % CHS;
		op = opind[(aa - 0xc0) / CHS];
		regchg[ch] = 1;
		// DT2
		fmprm[ch][9 + 11 * op] = (dd & 0xc0) >> 6; // 11000000
		// D2R
		fmprm[ch][2 + 11 * op] = (dd & 0x1f) >> 0; // 00011111
		if (g_flg.r){
			printf("%08x %s[%d]reg: %02x %02x\n", fpos, CHIPNAME, ch + 1, aa, dd);
		}
		break;
	case 0xe0 ... 0xff:
		// SL=D1L RR
		ch = (aa - 0xe0) % CHS;
		op = opind[(aa - 0xe0) / CHS];
		regchg[ch] = 1;
		// D1L
		fmprm[ch][4 + 11 * op] = (dd & 0xf0) >> 4; // 11110000
		// RR
		fmprm[ch][3 + 11 * op] = (dd & 0x0f) >> 0; // 00001111
		if (g_flg.r){
			printf("%08x %s[%d]reg: %02x %02x\n", fpos, CHIPNAME, ch + 1, aa, dd);
		}
		break;
	default:
		break;
	}
	return 1;
}
