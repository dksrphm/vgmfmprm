/*
 * vgmheader.h
 *
 *  Created on: 2018/02/22
 *      Author: dsrphm
 */

#ifndef VGMHEADER_H_
#define VGMHEADER_H_

#include <stdint.h>

struct vgm_header_tag{
        char idstr[4];
        uint32_t eofofst;
        uint32_t version;
        uint32_t snclock;

        uint32_t ym2413clk;
        uint32_t gd3ofst;
        uint32_t totalsmp;
        uint32_t loopofst;

        uint32_t loopsmp;
        uint32_t rate;
        uint16_t snfb;
        uint8_t  snw;
        uint8_t  snf;
        uint32_t ym2612clk;

        uint32_t ym2151clk;
        uint32_t dataofst;
        uint32_t segapcmclk;
        uint32_t segapcmif;

        uint32_t rf5c68clk;
        uint32_t ym2203clk;
        uint32_t ym2608clk;
        uint32_t ym2610bclk;

        uint32_t ym3812clk;
        uint32_t ym3526clk;
        uint32_t y8950clk;
        uint32_t ymf262clk;

        uint32_t ymf278bclk;
        uint32_t ymf271clk;
        uint32_t ymz280bclk;
        uint32_t rf5c164clk;

        uint32_t pwmclk;
        uint32_t ay8910clk;
        uint8_t  ay8910type;
        uint8_t  ay8910flg;
        uint8_t  ym2203ayflg;
        uint8_t  ym2608ayflg;
        uint8_t  volmod;
        uint8_t  vgm_res1;
        uint8_t  loopbase;
        uint8_t  loopmod;

        uint32_t gbdmgclk;
        uint32_t nesapuclk;
        uint32_t multipcmclk;
        uint32_t upd7759clk;

        uint32_t okim6258clk;
        uint8_t  okim6258flg;
        uint8_t  k054539flg;
        uint8_t  c140type;
        uint8_t  vgm_res2;
        uint32_t okim6295clk;
        uint32_t k051649clk;

        uint32_t k054539clk;
        uint32_t huc6280clk;
        uint32_t c140clk;
        uint32_t k053260clk;

        uint32_t pokeyclk;
        uint32_t qsndclk;
        uint32_t scspclk;
        uint32_t extrahdrofs;

        uint32_t wonderswanclk;
        uint32_t vsuclk;
        uint32_t saa1099vsuclk;
        uint32_t es5503clk;

        uint32_t es5506clk;
        uint8_t  es5505chns;
        uint8_t  es5506chns;
        uint8_t  c352div;
        uint8_t  vgm_res3;
        uint32_t x1clk;
        uint32_t c352clk;

        uint32_t ga20clk;
        unsigned char vgm_res4[12];
        unsigned char vgm_res5[16];
};

#endif /* VGMHEADER_H_ */
