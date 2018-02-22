/*
 * vgmfmprm.h
 *
 *  Created on: 2018/02/23
 *      Author: dsrphm
 */

#ifndef VGMFMPRM_H_
#define VGMFMPRM_H_
#include <stdint.h>
typedef struct {
	char vgmFilename[256];
	uint8_t d;
	uint8_t r;
} OPTIONS;

int vgmfmprm_ym2151(uint8_t, uint8_t);
int vgmfmprm_ym2612(uint8_t, uint8_t, uint8_t);

#else
//extern OPTIONS g_flg;
extern int vgmfmprm_ym2151(uint8_t, uint8_t);
extern int vgmfmprm_ym2612(uint8_t, uint8_t, uint8_t);


#endif /* VGMFMPRM_H_ */
