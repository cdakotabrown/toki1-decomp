/* @CompileEnv: IDO52_FORCE_WEB_COLOR=40:a0,83:v1 IDO52_COPYPROP_SITE=lod:2:deny,lod:6:deny */
/* @PostProc: swap_instructions --offset1 0x1c --offset2 0x24 */
/* @PostProc: swap_instructions --offset1 0x20 --offset2 0x28 */
/* @PostProc: swap_instructions --offset1 0x15c --offset2 0x188 */
/* @PostProc: patch_insn_reg_field 0x1c:rt:v0:v1 0x20:rs:v0:v1 */
/* @SymbolStart: 0x80136830 */
/* @SymbolSize: 452 */
#include "types.h"

extern void tt_cmd_once(s32);

extern s32 *tt_scroll_wk;
extern s16 *tt_anim_buf;
extern s32 tt_speed_prev_no;
extern s32 tt_speed_no;

void tt_speed_mark_set(void)
{
    s32 *p;
    s16 *q;
    s32 i;
    s32 prev;
    s32 cur;

    p = tt_scroll_wk;
    prev = tt_speed_no;
    cur = *p / 32768;
    tt_speed_prev_no = prev;
    tt_speed_no = cur;
    if (prev != cur) {
        tt_cmd_once(1553);
    }
    if (*p == 0) {
        q = tt_anim_buf;
        q[133] = 15370;
        q[183] = 15370;
        q[283] = 15370;
        q[333] = 15370;
        q[383] = 15370;
        q += 350;
        q[-117] = 15370;
    } else {
        q = tt_anim_buf + 100;
        for (i = 0; i < 6; i++) {
            q[33] = (*p / 32768 == i + 1) ? 15369 : 15370;
            q += 50;
        }
    }
}
