/*
 * vgmutil.c
 *
 *  Created on: 2018/02/23
 *      Author: dsrphm
 */

#include <stdio.h>
#include <string.h>
#include "vgmutil.h"
#include "vgmheader.h"

int readvgmheader(FILE *fp, struct vgm_header_tag *vgm_header)
{
	// VGM�t�@�C�����o�[�W������ǂݍ��݁A
	// ���̃o�[�W�����ɉ����ĕK�v�ȃT�C�Y���w�b�_�Ƃ��ēǂݍ���
	uint32_t version;
	fpos_t offset;

	// �o�[�W������ǂݍ���
	fgetpos(fp, &offset);
	fseek(fp, POSVER, SEEK_SET);
	fread(&version, sizeof(version), 1, fp);
	// �w�b�_��ǂݍ���
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
	// �t�@�C����VGM�t�@�C�����ǂ������A�w�b�_�擪��"Vgm "���ۂ��Ń`�F�b�N
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


