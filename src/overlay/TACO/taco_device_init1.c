/* Address: 0x80142EC0 (absorbed cross-prologue head) .. 0x80142F84 */
/* Size: 196 bytes (49 instructions) */
/* @SymbolStart: 0x80142EC0 */
/* @SymbolSize: 196 */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:6 */
/* @PostProc: patch_insn_reg_field 0x38:rt:t0:t8 0x94:rs:t0:t8 0x94:rt:t0:t8 0x9c:rs:t0:t8 0x54:rt:t6:v1 0x58:rs:t6:v1 0x58:rt:t6:v1 0x60:rs:t6:v1 0x60:rt:t7:t6 0x68:rt:t7:t6 0x7c:rt:t8:v1 0x80:rs:t8:v1 0x80:rt:t8:v1 0x88:rs:t8:v1 0x88:rt:t9:t7 0x90:rt:t9:t7 */
/* Address: 0x80142EC8 (true entry 0x80142EC0, 2 insns hoisted above .sym) */
#include "types.h"

extern u8 _debug_number_show_str0;
extern u8 step_mode;

extern s32  sub_sub_step_up();
extern void device_state_init();
extern s32  device_load_poll();
extern void card_step_init();
extern s32  season_bgm_set();

s32 taco_device_init1(void)
{
    s32 r;
    s32 sel;

    sel = step_mode;
    r = sel;
    switch (sel) {
    case 0:
        device_state_init();
        r = season_bgm_set();
        step_mode++;
        break;
    case 1:
        r = device_load_poll();
        if (r == 1) {
            step_mode++;
        }
        break;
    case 2:
        if (_debug_number_show_str0 == 0) {
            card_step_init();
        } else {
            sub_sub_step_up();
        }
        r = sub_sub_step_up();
        break;
    }
    return r;
}
