/*
 * vgmutil.h
 *
 *  Created on: 2018/02/23
 *      Author: dksrphm
 */

#ifndef VGMUTIL_H_
#define VGMUTIL_H_

#include <stdio.h>
#include "vgmheader.h"
int readvgmheader(FILE *, struct vgm_header_tag *);
int chkvgm(FILE *);
int vgmfmprm_main(FILE *, struct vgm_header_tag *);
int seektostream(FILE *, struct vgm_header_tag *);
int calcEndPos(struct vgm_header_tag *);
int formatN(char *, int, uint32_t, int, uint8_t *);
int formatM(char *, int, uint32_t, int, uint8_t *);

#define POSVER 0x08
#define VER150 0x150
#define VER160 0x160
#define VER161 0x161
#define LEN150 0x40
#define LEN160 0x80
#define LEN161 0xc0

#else
extern int readvgmheader(FILE *, struct vgm_header_tag *);
extern int chkvgm(FILE *);
extern int vgmfmprm_main(FILE *, struct vgm_header_tag *);
extern int seektostream(FILE *, struct vgm_header_tag *);
extern int calcEndPos(struct vgm_header_tag *);
extern int formatN(char *, int, uint32_t, int, uint8_t *);
extern int formatM(char *, int, uint32_t, int, uint8_t *);

#endif /* VGMUTIL_H_ */
