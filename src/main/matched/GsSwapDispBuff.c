/* Function: GsSwapDispBuff  (was recorded as func_8009CAC8 -- that address is */
/* Address: 0x8009CAB0 */
/* Size: 172 bytes (43 instructions) */
/* Region: Library (0x80086000+) */
/* Compiler: Psy-Q GCC 2.6.0 -O1 */
/* @SymbolStart: 0x8009CAB0 */
/* @SymbolSize: 172 */
/* @Compiler: gcc260 */
/* @CompileFlags: -O1 --load-dest-temp --konami-lib-sched */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern void PutDispEnv(s16 *);
extern void SetDispMask(s32 arg0);
extern void GsSetDrawBuffClip(void);
extern void GsSetDrawBuffOffset(void);

extern s16 GsDISPENV;
extern s16 libgs_dispenv_disp_y;
extern u16 PSDOFSX[];
extern u16 PSDOFSY[];
extern s16 PSDIDX;
extern s16 GsDSTACK[];

void GsSwapDispBuff(void) {
    s32 i;
    s16 idx;
    s16 *p;

    idx = PSDIDX;
    p = &GsDISPENV;
    *p = PSDOFSX[idx];
    libgs_dispenv_disp_y = PSDOFSY[idx];

    PutDispEnv(p);
    SetDispMask(1);

    if (PSDIDX == 0) {
        PSDIDX = 1;
    } else {
        PSDIDX = 0;
    }

    GsSetDrawBuffClip();
    GsSetDrawBuffOffset();

    for (i = 1240; i >= 0; i -= 40) {
        *(s16 *)((char *)GsDSTACK + i) = 0;
    }
}
