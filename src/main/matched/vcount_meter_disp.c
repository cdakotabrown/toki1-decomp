/* Address: 0x80044648 */
/* Size: 708 bytes (177 instructions) */
/* Region: Game (0x80041000-0x80086000) */
/* Compiler: IDO 5.2 */
/* @CompileEnv: IDO52_UGEN_TEMP_REG=12:t0,14:t2,15:t3,16:t4,17:t5,19:t7 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: remap_stack_slots --map '96:88,98:90,100:92,102:94,104:100,108:104,112:108,116:112,120:116' --keep-frame */
/* @PostProc: patch_insn_reg_field 0x1ec:rd:v0:t9 0x1f0:rt:v0:t9 0x1f4:rt:v0:t9 */
#include "types.h"

typedef struct {
    s16 x;
    s16 y;
    s16 w;
    s16 h;
} Rect;

extern s32 vblank_function_tbl0[];

extern void box_set(s32 id, Rect r, s32 colour);

void vcount_meter_disp(void)
{
    s32 q[4];
    Rect r;
    s32 i;
    s32 h;

    q[0] = vblank_function_tbl0[0] / 264;
    q[1] = vblank_function_tbl0[1] / 264;
    q[2] = vblank_function_tbl0[2] / 264;
    q[3] = vblank_function_tbl0[3] / 264;

    for (i = 0; i < 4; i++) {
        if (q[i] >= 3) {
            q[i] = 0x00800000;
        } else if (q[i] >= 2) {
            q[i] = 0x00808000;
        } else if (q[i] >= 1) {
            q[i] = 0x00008000;
        } else {
            q[i] = 128;
        }
    }

    r.w = 4;
    for (i = 0; i < 4; i++) {
        r.x = (s16)(140 + i * 4);
        h = (vblank_function_tbl0[i] * 16) / 264;
        r.h = (s16)h;
        r.y = (s16)(76 - h);
        box_set(i + 6, r, q[i]);
    }

    r.x = 136;
    r.h = 16;
    r.y = 60;
    box_set(i + 6, r, 0x00808080);
}
