/* Function: GsSetDrawBuffOffset (file name only -- see NAME below) */
/* Address: 0x80099370 */
/* Size: 264 bytes (66 instructions) */
/* Region: Library (0x80086000-0x800BFFFF) */
/* @SymbolStart: 0x80099370 */
/* @SymbolSize: 264 */
/* @Compiler: gcc260 */
/* @CompileFlags: -O2 -G0 -mips1 --load-dest-temp --konami-lib-sched */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

typedef struct {
    RECT rect;
    s16 x;
    s16 y;
} ClrArea;

extern s16 PSDGPU;
extern s16 PSDIDX;
extern s16 scr_half_w;
extern s16 scr_half_h;
extern s16 PSDOFSX[2];
extern s16 PSDOFSY[2];
extern s16 GsORGOFSX;
extern s16 GsORGOFSY;
extern ClrArea GsDRAWENV;

extern void PutDrawEnv(RECT *r);
extern void SetGeomOffset(s32 x, s32 y);

void GsSetDrawBuffOffset(void)
{
    s32 x;
    s32 y;

    if (PSDGPU != 0) {
        GsORGOFSX = GsORGOFSY = 0;
        GsDRAWENV.x = scr_half_w + PSDOFSX[PSDIDX];
        GsDRAWENV.y = scr_half_h + PSDOFSY[PSDIDX];
        PutDrawEnv(&GsDRAWENV.rect);
    } else {
        x = scr_half_w + PSDOFSX[PSDIDX == 0];
        y = scr_half_h + PSDOFSY[PSDIDX == 0];
        SetGeomOffset(x, y);
        GsORGOFSX = x;
        GsORGOFSY = y;
    }
}
