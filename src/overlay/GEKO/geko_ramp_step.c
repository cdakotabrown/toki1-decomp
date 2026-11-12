/* @SymbolStart: 0x8013FBE0 */
/* @SymbolSize: 188 */
/* @PostProc: patch_insn_reg_field 0x74:rd:t0:v0 0x7c:rt:t0:v0 0x80:rs:t0:v0 */
/* geko_ramp_step -- GEKO overlay, Address: 0x8013FBE8, Size: 176 bytes (44 insns) */
#include "types.h"
#include "tokimeki/class_w.h"

extern void device_cmd_send(s32 a0);
extern void sub_sub_step_up(void);

extern s32 step_counter;
extern s8 geko_fill_on;
extern CLASS_W class_w[];

void geko_ramp_step(void)
{
    s32 t6 = step_counter;

    s32 ta;

    if (t6 == 0) {
        class_w[98].disp_flag = class_w[98].disp_flag | 0x80;
        class_w[98].scale_y = 0;
        class_w[98].scale_x = 0;
        device_cmd_send(1281);
    }

    ta = (s16)(class_w[98].scale_x + 128);
    class_w[98].scale_y = class_w[98].scale_y + 128;
    class_w[98].scale_x = ta;
    if (ta < 4096) {
    } else {
        class_w[98].disp_flag = class_w[98].disp_flag & 0xFF7F;
        geko_fill_on = 1;
        sub_sub_step_up();
    }
    step_counter = step_counter + 1;
}
