/* @SymbolStart: 0x8013D35C */
/* @SymbolSize: 320 */
/* @PostProc: patch_stack_complete --old 40 --new 48 */
/* @PostProc: remap_stack_slots --from 36 --to 44 */
#include "types.h"

extern void taiiku_run_reset(void);
extern s32 taiiku_tama_push_1;
extern s32 taiiku_tama_vel;
extern s32 taiiku_tama_xf;
extern s32 taiiku_tama_fall;
struct { s32 w; s16 x; s16 y; } taiiku_tama_yf = {0, 0, 0};
extern s16 taiiku_tama_ghold;
extern u8  taiiku_tama_push[];

void runner_move(void)
{
    s32 t;
    s32 v0;
    s16 *q;

    if (taiiku_tama_push_1 <= 0) {
        taiiku_tama_yf.w = taiiku_tama_yf.w + 0x18000;
    } else {
        taiiku_tama_yf.w = taiiku_tama_yf.w - 0x10000;
    }
    taiiku_tama_xf = taiiku_tama_xf + taiiku_tama_vel;

    v0 = *(volatile s16 *)(taiiku_tama_push + 22);
    if (v0 >= 97) {
        taiiku_tama_fall = 1;
        *(volatile s16 *)(taiiku_tama_push + 22) = 96;
        taiiku_tama_ghold = 0;
        taiiku_run_reset();
    } else if (v0 < -96) {
        taiiku_tama_fall = 2;
        *(volatile s16 *)(taiiku_tama_push + 22) = -96;
        taiiku_tama_ghold = 0;
        taiiku_run_reset();
    }

    v0 = *(volatile s16 *)(taiiku_tama_push + 26);
    if (v0 < 0) {
        *(volatile s16 *)(taiiku_tama_push + 26) = 0;
        v0 = *(volatile s16 *)(taiiku_tama_push + 26);
    }
    if (v0 >= 128) {
        *(volatile s16 *)(taiiku_tama_push + 26) = 128;
    }

    q = (s16 *)&t;
    t = taiiku_tama_xf << 2;
    taiiku_tama_yf.x = q[1];
    t = taiiku_tama_yf.w << 1;
    taiiku_tama_yf.y = q[1];
}
