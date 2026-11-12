/* bunka_sd_fade_in - BUNKA_SD overlay, Address: 0x8013441C (true start */
/* @SymbolStart: 0x80134418 */
/* @SymbolSize: 144 */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* @CompileEnv: IDO52_COPYPROP_VAR=-4 */
/* @PostProc: swap_instructions --offset1 0x14 --offset2 0x18 */
/* @PostProc: swap_instructions --offset1 0x18 --offset2 0x1c */
/* @PostProc: swap_instructions --offset1 0x1c --offset2 0x20 */
/* @PostProc: swap_instructions --offset1 0x28 --offset2 0x2c */
/* @PostProc: swap_instructions --offset1 0x64 --offset2 0x68 */
/* @PostProc: patch_insn_reg_field 0x10:rt:a0:v1 0x20:rs:a0:v1 0x40:rt:a0:v1 0x44:rs:a0:v1 */
#include "types.h"
#include "tokimeki/class_w.h"

extern void sub_sub_step_up(void);

extern u8 scene_entry_b;
extern u8 srn_w[];
extern u8 D_80128B30_b[];
extern u8 D_80129C30_b[];
extern CLASS_W class_w[];

void bunka_sd_fade_in(void)
{
    s32 val;
    u8 *ptr;

    val = scene_entry_b;
    ptr = (&class_w[96].active);
    do {
        ptr += 68;
        *(ptr - 61) = val;
    } while (ptr < srn_w);

    scene_entry_b = val + 2;
    if (scene_entry_b < 129U) {
        return;
    }

    val = 128;
    ptr = D_80128B30_b;
    do {
        ptr += 272;
        *(ptr - 197) = val;
        *(ptr - 129) = val;
        *(ptr - 61) = val;
        *(ptr - 265) = val;
    } while (ptr != D_80129C30_b);

    scene_entry_b = val;
    sub_sub_step_up();
}
