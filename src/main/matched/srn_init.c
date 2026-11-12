/* Address: 0x8004E788 */
/* Size: 252 bytes (63 instructions) */
/* Region: Game (0x80041000-0x80085FFF), IDO 5.2 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

typedef struct {
    u16  h0;
    u16  h2;
    u16  h4;
    u16  h6;
    u32 *w8;
    u8   b12;
    u8   b13;
    u8   b14;
    u8   b15;
    u8   b16;
    u8   b17;
    u8   b18;
    u8   b19;
    u8   b20;
    u8   b21;
    u16  h22;
    u16  h24;
    u16  h26;
    s32  w28;
    s32  w32;
    void *w36;
} Rec40;

extern Rec40 srn_w[];
extern u8 mod_w[];

void srn_init(s32 idx, u32 *src, s32 link) {
    Rec40 *p = &srn_w[idx];

    p->b13 = (src[0] & 0xFF00) >> 8;
    p->h2 = 0;
    p->h6 = 0;
    p->b12 = 0;
    p->w8 = src + 5;
    p->b14 = (src[1] & 0xFF000000) >> 24;
    p->b15 = (src[1] & 0x00FF0000) >> 16;
    p->b16 = (src[1] & 0xFF00) >> 8;
    p->b17 = src[1] & 0xFF;
    p->b18 = (src[2] & 0xFF000000) >> 24;
    p->b19 = (src[2] & 0x00FF0000) >> 16;
    p->b20 = (src[2] & 0xFF00) >> 8;
    p->b21 = src[2] & 0xFF;
    p->h22 = 0;
    p->h24 = 480;
    p->w28 = src[4];
    p->w32 = src[3];
    p->w36 = &mod_w[link * 20];
}
