/* Address: 0x801355AC */
/* Size: 132 bytes (33 insns) */
/* @SymbolSize: 132 */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* @CompileEnv: IDO52_COPYPROP_VAR=-4 */
/* @PostProc: swap_instructions --offset1 0x58 --offset2 0x5c */
/* @PostProc: patch_insn_reg_field 0x20:rt:a0:v1 0x24:rs:a0:v1 0x24:rt:a0:v1 0x2c:rt:a0:v1 0x34:rt:a0:v1 0x38:rs:a0:v1 0x38:rt:a0:v1 0x3c:rs:a0:v1 */
#include "types.h"
#include "tokimeki/class_w.h"

extern void basyo_show(void);
extern void sub_sub_step_up(void);

extern u8 scene_entry_b;
extern u8 cal_cw_bright;
extern u8 srn_w[];
extern CLASS_W class_w[];

void bunka_sd_wait5(void)
{
    s32 val;
    u8 *ptr;

    basyo_show();

    val = scene_entry_b;
    cal_cw_bright = val;
    class_w[98].bright = val;
    val -= 2;
    scene_entry_b = val;
    if (scene_entry_b == 0) {
        val = 0;
        ptr = (&class_w[96].active);
        do {
            ptr += 272;
            *(ptr - 197) = val;
            *(ptr - 129) = val;
            *(ptr - 61) = val;
            *(ptr - 265) = val;
        } while (ptr != srn_w);
        scene_entry_b = val;
        sub_sub_step_up();
    }
}
