/*
 * vgmfmprm_ym2203.c
 *
 *  Created on: 2018/02/23
 *      Author: dksrphm
 */

#include <stdio.h>
#include <string.h>
#include "vgmfmprm.h"
#include "vgmutil.h"

#define CHIPNAME "YM2203"
#define CHS 3
#define REGS 46
#define TONES 255

extern OPTIONS g_flg;

static uint8_t fmprm[CHS][REGS] = {0};
static uint8_t tone[TONES][REGS] = {0};
static int regchg[CHS] = {0};
static int tones = 0;
static int curtone[CHS] = {-1};

static int serialize(uint8_t, uint8_t, uint8_t);

int vgmfmprm_ym2203(uint8_t aa, uint8_t dd)
{
	/* in mml2vgm
	'@ N No
	'@ AR DR SR RR SL TL KS ML DT AM SSGEG <- fmprm[ch][0]-[10]
	'@ AR DR SR RR SL TL KS ML DT AM SSGEG <- fmprm[ch][11]-[21]
	'@ AR DR SR RR SL TL KS ML DT AM SSGEG <- fmprm[ch][22]-[32]
	'@ AR DR SR RR SL TL KS ML DT AM SSGEG <- fmprm[ch][33]-[43]
	'@ AL FB <- fmprm[ch][44]-[45]
	*/
	uint8_t ch;
	uint8_t op;
	uint8_t opind[4] = {0, 2, 1, 3};
	int i;
	int cmp;

	ch = aa % 4;
	if (ch == 3){
		return 0;
	}
	switch (aa){
	case 0x28:
		ch = dd & 0x03;
		if (regchg[ch]){
			if (dd & 0xf0){ // 11110000
				if (TONES < tones){
					printf("%s: tones over %d.\n", CHIPNAME, TONES);
				} else {
					cmp = 1;
					for (i = 0; i < tones; i++){
						if (!memcmp(tone[i], fmprm[ch], sizeof(fmprm[ch]))){
							cmp = 0;
							break;
						}
					}
					if (cmp){
						formatN(CHIPNAME, ch, samples, tones, fmprm[ch]);
						curtone[ch] = tones;
						memcpy(tone[tones], fmprm[ch], sizeof(fmprm[ch]));
						tones++;
					} else {
						if (i != curtone[ch]){
							printf("%s[%d] samples:%d @%d\n", CHIPNAME, ch + 1, samples, i);
							curtone[ch] = i;
						}
					}
				}
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
		fmprm[ch][7 + 11 * op] = (dd & 0x0f) >> 0; //00001111
		// DT
		fmprm[ch][8 + 11 * op] = (dd & 0x70) >> 4; //01110000
		if (g_flg.r){
			printf("%08x %s[%d][%d]reg: %02x %02x\n", fpos, CHIPNAME, ch + 1, op + 1, aa, dd);
			serialize(aa, dd, ch);
		}
		break;
	case 0x40 ... 0x4f:
		op = opind[(aa - 0x40) / 4];
		regchg[ch] = 1;
		// TL
		fmprm[ch][5 + 11 * op] = (dd & 0x7f) >> 0; //01111111
		if (g_flg.r){
			printf("%08x %s[%d][%d]reg: %02x %02x\n", fpos, CHIPNAME, ch + 1, op + 1, aa, dd);
			serialize(aa, dd, ch);
		}
		break;
	case 0x50 ... 0x5f:
		// KS AR
		op = opind[(aa - 0x50) / 4];
		regchg[ch] = 1;
		// RS
		fmprm[ch][6 + 11 * op] = (dd & 0xc0) >> 6; //11000000
		// AR
		fmprm[ch][0 + 11 * op] = (dd & 0x1f) >> 0; //00011111
		if (g_flg.r){
			printf("%08x %s[%d][%d]reg: %02x %02x\n", fpos, CHIPNAME, ch + 1, op + 1, aa, dd);
			serialize(aa, dd, ch);
		}
		break;
	case 0x60 ... 0x6f:
		// DR
		op = opind[(aa - 0x60) / 4];
		regchg[ch] = 1;
		fmprm[ch][1 + 11 * op] = (dd & 0x1f) >> 0; //00011111
		if (g_flg.r){
			printf("%08x %s[%d][%d]reg: %02x %02x\n", fpos, CHIPNAME, ch + 1, op + 1, aa, dd);
			serialize(aa, dd, ch);
		}
		break;
	case 0x70 ... 0x7f:
		op = opind[(aa - 0x70) / 4];
		regchg[ch] = 1;
		// SR
		fmprm[ch][2 + 11 * op] = (dd & 0x1f) >> 0; //00011111
		if (g_flg.r){
			printf("%08x %s[%d][%d]reg: %02x %02x\n", fpos, CHIPNAME, ch + 1, op + 1, aa, dd);
			serialize(aa, dd, ch);
		}
		break;
	case 0x80 ... 0x8f:
		// SL RR
		op = opind[(aa - 0x80) / 4];
		regchg[ch] = 1;
		// SL
		fmprm[ch][4 + 11 * op] = (dd & 0xf0) >> 4; //11110000
		// RR
		fmprm[ch][3 + 11 * op] = (dd & 0x0f) >> 0; //00001111
		if (g_flg.r){
			printf("%08x %s[%d][%d]reg: %02x %02x\n", fpos, CHIPNAME, ch + 1, op + 1, aa, dd);
			serialize(aa, dd, ch);
		}
		break;
	case 0x90 ... 0x9f:
		// SSG-EG
		op = opind[(aa - 0x90) / 4];
		regchg[ch] = 1;
		fmprm[ch][10 + 11 * op] = (dd & 0x0f) >> 0; //00001111
		if (g_flg.r){
			printf("%08x %s[%d][%d]reg: %02x %02x\n", fpos, CHIPNAME, ch + 1, op + 1, aa, dd);
			serialize(aa, dd, ch);
		}
		break;
	case 0xb0 ... 0xb3:
		// FB AL
		// FB
		regchg[ch] = 1;
		fmprm[ch][45] = (dd & 0x38) >> 3; //00111000
		// AL
		fmprm[ch][44] = (dd & 0x07) >> 0; //00000111
		if (g_flg.r){
			printf("%08x %s[%d]reg: %02x %02x\n", fpos, CHIPNAME, ch + 1, aa, dd);
			serialize(aa, dd, ch);
		}
		break;
	default:
		break;
	}
	return 1;
}
int serialize(uint8_t aa, uint8_t dd, uint8_t ch)
{
	int i;
	printf("%08x,%02x,%02x,%s[%d]regs,", fpos, aa, dd, CHIPNAME, ch + 1);
	for (i = 0; i < REGS; i++){
		printf("%d,", fmprm[ch][i]);
	}
	printf("\n");
	return 1;
}
