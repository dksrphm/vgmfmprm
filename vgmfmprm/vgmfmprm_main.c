/*
 * vgmfmprm_main.c
 *
 *  Created on: 2018/02/23
 *      Author: dsrphm
 */

#include <stdio.h>
#include "vgmfmprm.h"
#include "vgmutil.h"

uint32_t samples = 0;
uint32_t fpos;

extern OPTIONS g_flg;

int vgmfmprm_main(FILE *fp, struct vgm_header_tag *vgm_header)
{
	fpos_t ft;
	uint32_t fendpos;
	uint8_t cmd;
	uint8_t aa;
	uint8_t dd;
	uint8_t aa1;
	uint8_t aa2;
	uint8_t port;

	uint8_t eosd;
	uint8_t datatype;
	uint32_t datasize;
	uint32_t flgEnd = 0;

	fendpos = calcEndPos(vgm_header);
	if (g_flg.d){
		printf("fendpos: %x\n", fendpos);
	}
	seektostream(fp, vgm_header);

	fgetpos(fp, &ft);
	fpos = ft;
	fread(&cmd, sizeof(cmd), 1, fp);
	do{
		switch (cmd){
		case 0x4f:
			// 0x4F dd    : Game Gear PSG stereo, write dd to port 0x06
			fread(&dd, sizeof(dd), 1, fp);
			if (g_flg.d){
				printf("%08x %02x %02x: GGPSG: write %02x to port 0x06.\n", (uint32_t)fpos, cmd, dd, dd);
			}
			//vgm_fprm_ym2612(aa, dd);
			break;
		case 0x50:
			// 0x4F dd    : Game Gear PSG stereo, write dd to port 0x06
			fread(&dd, sizeof(dd), 1, fp);
			if (g_flg.d){
				printf("%08x %02x %02x: PSG: write value %02x.\n", (uint32_t)fpos, cmd, dd, dd);
			}
			//vgm_fprm_ym2612(aa, dd);
			break;
		case 0x61:
			fread(&aa, sizeof(aa), 1, fp);
			fread(&dd, sizeof(dd), 1, fp);
			samples = samples + aa + dd * 256;
			if (g_flg.d){
				printf("%08x %02x %02x %02x: VGM: wait %d samples.\n", (uint32_t)fpos, cmd, aa, dd, aa + dd * 0x100);
			}
			break;
		case 0x62:
			samples += 732;
			if (g_flg.d){
				printf("%08x %02x: VGM: wait 732 samples.\n", (uint32_t)fpos, cmd);
			}
			break;
		case 0x63:
			samples += 882;
			if (g_flg.d){
				printf("%08x %02x: VGM: wait 882 samples.\n", (uint32_t)fpos, cmd);
			}
			break;
		case 0x66:
			if (g_flg.d){
				printf("%08x %02x: VGM: end of sound data.\n", (uint32_t)fpos, cmd);
			}
			break;
		case 0x67:
			fread(&eosd, sizeof(eosd), 1, fp);
			fread(&datatype, sizeof(datatype), 1, fp);
			fread(&datasize, sizeof(datasize), 1, fp);
			if (fseek(fp, datasize, SEEK_CUR)){
				printf("seek_error: %08x %02x %02x %02x %08x data block\n", (uint32_t)fpos, cmd, eosd, datatype, datasize);
				flgEnd = 1;
			}
			if (g_flg.d){
				printf("%08x %02x %02x %02x %08x: VGM: data block\n", (uint32_t)fpos, cmd, eosd, datatype, datasize);
			}
			break;
		case 0x52:
		case 0x53:
			// YM2612
			port = cmd - 0x52;
			fread(&aa, sizeof(aa), 1, fp);
			fread(&dd, sizeof(dd), 1, fp);
			if (g_flg.d){
				printf("%08x %02x %02x %02x: YM2612: port %d addr %02x data %02x\n",
					(uint32_t)fpos, cmd, aa, dd, port, aa, dd);
			}
			vgmfmprm_ym2612(port, aa, dd);
			break;
		case 0x54:
			// YM2151
			fread(&aa, sizeof(aa), 1, fp);
			fread(&dd, sizeof(dd), 1, fp);
			if (g_flg.d){
				printf("%08x %02x %02x %02x: YM2151: \n", (uint32_t)fpos, cmd, aa, dd);
			}
			vgmfmprm_ym2151(aa, dd);
			break;
		case 0x55:
			// YM2203
			fread(&aa, sizeof(aa), 1, fp);
			fread(&dd, sizeof(dd), 1, fp);
			if (g_flg.d){
				printf("%08x %02x %02x %02x: YM2203: \n", (uint32_t)fpos, cmd, aa, dd);
			}
			vgmfmprm_ym2203(aa, dd);
			break;
		case 0x70 ... 0x7f:
			samples = samples + cmd - 0x70 + 1;
			if (g_flg.d){
				printf("%08x %02x: VGM: wait %d samples.\n", (uint32_t)fpos, cmd, cmd - 0x70 + 1);
			}
			break;
		case 0x80 ... 0x8f:
			samples = samples + cmd - 0x80;
			if (g_flg.d){
				printf("%08x %02x: YM2612: port 0 address 2A write from the data bank then wait %d samples.\n", (uint32_t)fpos, cmd, cmd - 0x80);
			}
			break;
		case 0xc0:
			fread(&aa1, sizeof(aa1), 1, fp);
			fread(&aa2, sizeof(aa2), 1, fp);
			fread(&dd, sizeof(dd), 1, fp);
			if (g_flg.d){
				printf("%08x %02x %02x %02x %02x: SegaPCM: write %02x to memory %04x.\n",
					(uint32_t)fpos, cmd, aa1, aa2, dd, dd, aa1 + aa2 * 0x100);
			}
			break;
		case 0xe0:
			fread(&datasize, sizeof(datasize), 1, fp);
			if (g_flg.d){
				printf("%08x %02x %08x: VGM: seek to offset %08x\n", (uint32_t)fpos, cmd, datasize, datasize);
			}
			break;
		default:
			printf("Not implemented: %08x %02x\n", (uint32_t)fpos, cmd);
		}
		fgetpos(fp, &ft);
		fpos = ft;
		fread(&cmd, sizeof(cmd), 1, fp);
	} while (fpos < fendpos && fp != NULL && !flgEnd);
	return 1;
}

