/*
 * vgmfmprm.c
 *
 *  Created on: 2018/02/22
 *      Author: dsrphm
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <libgen.h>
#include "vgmheader.h"
#include "vgmutil.h"
#include "vgmfmprm.h"

OPTIONS g_flg = {0};

int main(int argc, char *argv[])
{
	struct vgm_header_tag vgm_header = {0};

	int opt;
	char vgmFilename[256];
	FILE *fp;

	while ((opt = getopt(argc, argv, "dr")) != -1) {
		switch (opt) {
		case 'd':
			// 色々出力するオプション
			g_flg.d = 1;
			break;
		case 'r':
			// レジスタの変更情報を出力するオプション
			g_flg.r = 1;
			break;
		default:
			printf("error! \'%c\' \'%c\'\n", opt, optopt);
			return 1;
		}
	}
	if (g_flg.d){
		printf("d = %d\n", g_flg.d);
		printf("r = %d\n", g_flg.r);
		for (int i = optind; i < argc; i++) {
			printf("arg = %s\n", argv[i]);
		}
	}

	if (argc == optind + 1){
			strcpy(vgmFilename, argv[optind]);
	} else {
		printf("Usage: %s [-d] [-r] vgm_file_name\n", basename(argv[0]));
		printf("	-d: print debug info.\n	-r: print tone register info.\n");
		return 1;
	}
	// パラメータに指定されたファイルを開く
	printf("VGM file name: %s\n", vgmFilename);
	if (!(fp = fopen(vgmFilename, "rb"))){
		printf("File open error: %s\n", vgmFilename);
		fclose(fp);
		return 1;
	}

	if (!chkvgm(fp)){
		printf("File format error: %s\n", vgmFilename);
		fclose(fp);
		return 1;
	}
	// ヘッダを構造体へ読み込む
	readvgmheader(fp, &vgm_header);
	if (g_flg.d){
		printf("version: %x\n", vgm_header.version);
	}

	vgmfmprm_main(fp, &vgm_header);

	fclose(fp);
	return 0;
}
