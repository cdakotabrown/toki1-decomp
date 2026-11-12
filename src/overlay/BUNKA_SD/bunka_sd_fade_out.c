/* bunka_sd_fade_out - BUNKA_SD overlay, Address: 0x801344AC */
/* @SymbolStart: 0x801344A8 */
/* @SymbolSize: 148 */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* @CompileEnv: IDO52_COPYPROP_VAR=-4 */
/* @PostProc: swap_instructions --offset1 0x14 --offset2 0x18 */
/* @PostProc: swap_instructions --offset1 0x18 --offset2 0x1c */
/* @PostProc: swap_instructions --offset1 0x1c --offset2 0x20 */
/* @PostProc: swap_instructions --offset1 0x28 --offset2 0x2c */
/* @PostProc: swap_instructions --offset1 0x68 --offset2 0x6c */
/* @PostProc: patch_insn_reg_field 0x10:rt:a0:v1 0x20:rs:a0:v1 0x4c:rt:a0:v1 0x50:rs:a0:v1 */
#include "types.h"
#include "tokimeki/class_w.h"

extern void sub_sub_step_up(void);

extern u8 scene_entry_b;
extern u8 srn_w[];
extern u8 cw96_active_a[];
extern u8 srn_w_a[];
extern CLASS_W class_w[];

void bunka_sd_fade_out(void)
{
    s32 val;
    u8 *p;

    val = scene_entry_b;
    p = (&class_w[96].active);
    do {
        p += 68;
        p[-61] = val;
    } while (p < srn_w);

    if (val == 0) {
        val = 2;
    }
    scene_entry_b = val - 2;
    if (scene_entry_b == 0) {
        val = 0;
        p = cw96_active_a;
        do {
            p += 272;
            p[-197] = val;
            p[-129] = val;
            p[-61] = val;
            p[-265] = val;
        } while (p != srn_w_a);
        scene_entry_b = val;
        sub_sub_step_up();
    }
}
