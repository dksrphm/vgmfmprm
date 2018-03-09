/*
 * vgmfmprm.h
 *
 *  Created on: 2018/02/23
 *      Author: dksrphm
 */

#ifndef VGMFMPRM_H_
#define VGMFMPRM_H_

#include <stdint.h>
#include "vgmheader.h"

typedef struct {
	char vgmFilename[256];
	uint8_t d;
	uint8_t r;
} OPTIONS;

extern uint32_t samples;
extern uint32_t fpos;

extern int vgmfmprm_ym2151(uint8_t, uint8_t);
extern int vgmfmprm_ym2203(uint8_t, uint8_t);
extern int vgmfmprm_ym2608(uint8_t, uint8_t, uint8_t);
extern int vgmfmprm_ym2610b(uint8_t, uint8_t, uint8_t);
extern int vgmfmprm_ym2612(uint8_t, uint8_t, uint8_t);
extern int vgmfmprm_main(FILE *, struct vgm_header_tag *);

#else

#endif /* VGMFMPRM_H_ */
