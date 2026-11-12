/* Address: 0x8009D2E0 */
/* Size: 140 bytes (35 instructions) */
/* Region: Library (0x80086000+) */
/* @SymbolStart: 0x8009D2E0 */
/* @SymbolSize: 140 */
/* @Compiler: gcc260 */
/* @CompileFlags: --fill-prev --load-dest-temp */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: swap_instructions --offset1 0x0 --offset2 0x8 */
/* @PostProc: swap_instructions --offset1 0x4 --offset2 0xc */
/* @PostProc: swap_instructions --offset1 0xc --offset2 0x20 */
/* @PostProc: swap_instructions --offset1 0x10 --offset2 0x50 */
/* @PostProc: swap_instructions --offset1 0x14 --offset2 0x44 */
/* @PostProc: swap_instructions --offset1 0x18 --offset2 0x20 */
/* @PostProc: swap_instructions --offset1 0x1c --offset2 0x48 */
/* @PostProc: swap_instructions --offset1 0x20 --offset2 0x54 */
/* @PostProc: swap_instructions --offset1 0x24 --offset2 0x50 */
/* @PostProc: swap_instructions --offset1 0x28 --offset2 0x50 */
/* @PostProc: swap_instructions --offset1 0x2c --offset2 0x44 */
/* @PostProc: swap_instructions --offset1 0x30 --offset2 0x50 */
/* @PostProc: swap_instructions --offset1 0x38 --offset2 0x3c */
/* @PostProc: swap_instructions --offset1 0x44 --offset2 0x58 */
/* @PostProc: swap_instructions --offset1 0x48 --offset2 0x5c */
/* @PostProc: swap_instructions --offset1 0x4c --offset2 0x60 */
/* @PostProc: swap_instructions --offset1 0x50 --offset2 0x60 */
/* @PostProc: swap_instructions --offset1 0x54 --offset2 0x58 */
/* @PostProc: swap_instructions --offset1 0x68 --offset2 0x6c */
/* @PostProc: swap_instructions --offset1 0x6c --offset2 0x70 */
/* @PostProc: patch_insn_reg_field --patch 0x24:rt:v1:v0 --patch 0x2c:rt:v1:v0 --patch 0x2c:rs:v1:v0 --patch 0x3c:rt:v1:v0 */
#include "types.h"

extern s16 PSDIDX;
extern s16 CLIP2;
extern s16 libgs_clip_base_y;
extern s16 libgs_clip_base_w;
extern s16 libgs_clip_base_h;
extern s16 PSDOFSX[];
extern s16 PSDOFSY[];
extern s16 GsDRAWENV;
extern s16 libgs_drawenv_clip_y;
extern s16 libgs_drawenv_clip_w;
extern s16 libgs_drawenv_clip_h;
extern void PutDrawEnv(RECT *);

void GsSetDrawBuffClip(void)
{
    RECT *r;
    s32 idx;
    s32 xb;
    s32 xo;
    s32 yb;
    s32 yo;

    idx = PSDIDX;
    libgs_drawenv_clip_w = libgs_clip_base_w;
    libgs_drawenv_clip_h = libgs_clip_base_h;
    xb = CLIP2;
    yo = PSDOFSY[idx];
    xo = PSDOFSX[idx];
    yb = libgs_clip_base_y;
    r = (RECT *)&GsDRAWENV;
    r->x = xb + xo;
    libgs_drawenv_clip_y = yb + yo;
    PutDrawEnv(r);
}
