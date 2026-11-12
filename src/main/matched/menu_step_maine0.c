/* Address: 0x8005BD68, Size: 164 bytes (41 instructions) */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:6 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: patch_insn_reg_field 0x20:rt:t8:t7 0x60:rt:t6:v1 0x64:rs:t6:v1 0x64:rt:t6:v1 0x6C:rs:t6:v1 0x6C:rt:t7:t6 0x74:rt:t7:t6 0x78:rs:t8:t7 0x78:rt:t8:t7 0x80:rs:t8:t7 0x80:rt:t9:t8 0x84:rs:t9:t8 */
#include "types.h"
extern u8  step_mode;
extern s32 pad_trig;
extern s32 menu_step_maine0_str0;
extern void device_cmd_send(s32 a);
extern void k_sub_reset(void);
extern void set_kanji_string(s32 a, s32 b, s32 c, s32 *d, s32 e);
extern s32 k_sub_disp_start(s32 a);
extern s32 goto_sub_sub_step(s32 a);

s32 menu_step_maine0(void) {
    s32 m;
    s32 ret;

    m = step_mode;
    ret = m;
    switch (m) {
    case 0:
        device_cmd_send(1282);
        k_sub_reset();
        set_kanji_string(-128, 50, 1, &menu_step_maine0_str0, 0);
        ret = k_sub_disp_start(1);
        step_mode = step_mode + 1;
        break;
    case 1:
        if ((pad_trig & 0x860) != 0) {
            ret = goto_sub_sub_step(6);
        }
        break;
    }
    return ret;
}
