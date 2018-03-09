/*
 * vgmutil.c
 *
 *  Created on: 2018/02/23
 *      Author: dksrphm
 */

#include <stdio.h>
#include <string.h>
#include "vgmutil.h"
#include "vgmheader.h"

int readvgmheader(FILE *, struct vgm_header_tag *);
int chkvgm(FILE *);
int seektostream(FILE *, struct vgm_header_tag *);
int calcEndPos(struct vgm_header_tag *);
int formatN(char *, int, uint32_t, int, uint8_t *);
int formatM(char *, int, uint32_t, int, uint8_t *);

int readvgmheader(FILE *fp, struct vgm_header_tag *vgm_header)
{
	// read header from vgm file.
	// 1.read VERSION from header.
	// 2.determine version.
	// 3.read suitable size from header.
	uint32_t version;
	fpos_t offset;

	fgetpos(fp, &offset);
	fseek(fp, POSVER, SEEK_SET);
	fread(&version, sizeof(version), 1, fp);

	rewind(fp);
	if (version <= VER150){
		fread(vgm_header, LEN150, 1, fp);
	} else if (version <= VER160){
		fread(vgm_header, LEN160, 1, fp);
	} else if (version <= VER161){
		fread(vgm_header, LEN161, 1, fp);
	} else {
		fread(vgm_header, sizeof(vgm_header), 1, fp);
	}

	fsetpos(fp, &offset);
	return 1;
}

int chkvgm(FILE *fp)
{
	// Check file is vgm format file or not.
	char idstr[4];
	fpos_t offset;
	int rc = 0;

	fgetpos(fp, &offset);
	rewind(fp);
	fread(&idstr, 4, 1, fp);
	if (strncmp(idstr, "Vgm ", 4)){
		//NG
		rc = 0;
	} else {
		rc = 1;
	}
	fsetpos(fp, &offset);
	return rc;
}

int calcEndPos(struct vgm_header_tag *vgm_header)
{
	uint32_t endPos;
	if (vgm_header->gd3ofst == 0){
		endPos = vgm_header->eofofst + 0x04;
	} else {
		endPos = vgm_header->gd3ofst + 0x14;
	}
	return endPos;
}

int seektostream(FILE *fp, struct vgm_header_tag *vgm_header)
{
	if (vgm_header->version <= VER150){
		rewind(fp);
		fseek(fp, 0x40, SEEK_SET);
	} else {
		rewind(fp);
		fseek(fp, 0x34 + vgm_header->dataofst, SEEK_SET);
	}
	return 1;
}
int formatN(char *CHIPNAME, int ch, uint32_t samples, int tonenum, uint8_t fmprm[46])
{
	// Output tone parameters in 'N' format.(for YM2608, 2610B, ...)
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

	printf("%s[%d] samples:%d @%d\n", CHIPNAME, ch + 1, samples, tonenum);
	printf("'@ N %d\n", tonenum);
	printf("    AR  DR  SR  RR  SL  TL  KS  ML  DT  AM SSGEG\n");
	for (op = 0; op < 4; op++){
		printf("'@ ");
		for (reg = 0; reg < 11; reg++){
			printf("%03d,", fmprm[op * 11 + reg]);
		}
		printf("\n");
	}
	printf("   ALG  FB\n");
	printf("'@ %03d,%03d\n\n", fmprm[44], fmprm[45]);
	return 1;
}
int formatM(char *CHIPNAME, int ch, uint32_t samples, int tonenum, uint8_t fmprm[46])
{
	// Output tone parameters in 'M' format.(for YM2151)
	/* in mml2vgm
	'@ M No
	'@ AR DR SR RR SL TL KS ML DT1 DT2 AME <- ym2151reg[ch][0]-[10]
	'@ AR DR SR RR SL TL KS ML DT1 DT2 AME <- ym2151reg[ch][11]-[21]
	'@ AR DR SR RR SL TL KS ML DT1 DT2 AME <- ym2151reg[ch][22]-[32]
	'@ AR DR SR RR SL TL KS ML DT1 DT2 AME <- ym2151reg[ch][33]-[43]
	'@ AL FB <- ym2151reg[ch][44]-[45]
	*/
	uint8_t op;
	uint8_t reg;

	printf("%s[%d] samples:%d @%d\n", CHIPNAME, ch + 1, samples, tonenum);
	printf("'@ M %d\n", tonenum);
	printf("    AR  DR  SR  RR  SL  TL  KS  ML DT1 DT2 AME\n");
	for (op = 0; op < 4; op++){
		printf("'@ ");
		for (reg = 0; reg < 11; reg++){
			printf("%03d,", fmprm[op * 11 + reg]);
		}
		printf("\n");
	}
	printf("   ALG  FB\n");
	printf("'@ %03d,%03d\n\n", fmprm[44], fmprm[45]);
	return 1;
}
