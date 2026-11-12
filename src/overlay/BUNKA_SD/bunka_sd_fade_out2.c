/* @SymbolStart: 0x80134850 */
/* @SymbolSize: 232 */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* @CompileEnv: IDO52_COPYPROP_VAR=-12 */
/* @PostProc: swap_instructions --offset1 0x78 --offset2 0x7c */
/* @PostProc: swap_instructions --offset1 0x7c --offset2 0x80 */
/* @PostProc: swap_instructions --offset1 0x84 --offset2 0x88 */
/* @PostProc: swap_instructions --offset1 0xbc --offset2 0xc0 */
/* @PostProc: patch_insn_reg_field 0x28:rt:a1:v1 0x34:rs:a1:v1 0x44:rd:a0:v0 0x48:rs:a0:v0 0x50:rs:a0:v0 0x58:rd:a0:v0 0x5c:rs:a0:v0 0x74:rt:a0:v1 0x80:rs:a0:v1 0x9c:rt:a0:v1 0xa0:rs:a0:v1 */
/* bunka_sd_fade_out2 -- BUNKA_SD overlay, Address: 0x80134858 (true start 0x80134850: */
#include "types.h"
#include "tokimeki/class_w.h"

extern s32 bunka_sd_kind;
extern u8 scene_entry_c;
extern u8 scene_entry_b;
extern u8 class_w[];
extern u8 srn_w[];
extern void sub_sub_step_up(void);

void bunka_sd_fade_out2(void)
{
    u8 ci;
    s32 row;
    s32 count;
    u8 *p;
    u8 *q;

    if (bunka_sd_kind == 3) {
        ci = scene_entry_c;
        ((CLASS_W *)class_w)[100].anim_flag = 64;
        row = (s32)((u32)ci / 14U);
        if ((u32)row >= 2U) {
            ci = 0;
            row = 0;
        }
        ((CLASS_W *)class_w)[100].frame = (s16)(row + 162);
        scene_entry_c = ci + 1;
    }

    count = scene_entry_b;
    p = class_w;
    do {
        p += 68;
        *(p + 6467) = count;
    } while (p < (&((CLASS_W *)class_w)[5].active));

    scene_entry_b = count - 2;
    if (scene_entry_b == 0) {
        count = 0;
        q = (&((CLASS_W *)class_w)[96].active);
        do {
            q += 272;
            *(q - 197) = count;
            *(q - 129) = count;
            *(q - 61) = count;
            *(q - 265) = count;
        } while (q != srn_w);
        scene_entry_b = count;
        sub_sub_step_up();
    }
}
