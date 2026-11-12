/* @SymbolStart: 0x801528DC */
/* @SymbolSize: 216 */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* @PostProc: swap_instructions --offset1 0x24 --offset2 0x30 */
/* @PostProc: swap_instructions --offset1 0x28 --offset2 0x34 */
/* @PostProc: swap_instructions --offset1 0x2c --offset2 0x38 */
/* @PostProc: swap_instructions --offset1 0x30 --offset2 0x34 */
/* @PostProc: swap_instructions --offset1 0x54 --offset2 0x58 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v1:v0 0x14:rs:v1:v0 0x14:rt:v1:v0 0x1c:rs:v1:v0 0x24:rs:v1:v0 0x34:rt:t6:v1 0x38:rt:t6:v1 0x48:rt:a1:a2 0x54:rs:a1:a2 0x54:rt:a1:a2 0x5c:rt:a2:a1 0x68:rt:a2:a1 0x78:rt:t7:t6 0x80:rt:t7:t6 0x88:rt:a1:a2 0x94:rt:v0:t7 0x9c:rs:v0:t7 0xa0:rt:v0:t7 */
/* rpg_bat_fade_out - RPG_BAT overlay, Address: 0x801528E4, Size: 208 bytes (52 */
#include "types.h"
#include "tokimeki/class_w.h"

extern s32 sub_step_up();
extern s32 back_clear_switch();
extern s32 class_clear_all();
extern s32 text_sys_init();
extern s32 cursor_init();

extern s32 step_state;
extern u8  scene_entry_c;
extern s32 rpg_bat_mode;
extern u8  screen_bri;
extern u8  class_w[];

void rpg_bat_fade_out(void)
{
    s32 v0;
    s32 v1;
    s32 a0;
    s32 a1;
    s32 t7;
    u8 *p;
    u8 *end;

    v0 = step_state;
    if (v0 == 0) {
        v0 = v0 + 1;
        step_state = v0;
        scene_entry_c = (u8)128;
    }
    v1  = scene_entry_c;
    a0  = rpg_bat_mode;
    a1  = 0x20000;
    p   = class_w;
    end = &((CLASS_W *)class_w)[96].active;

    do {
        p[0x1107] = (u8)v1;
        screen_bri = (u8)v1;
        if (a0 == a1) {
            if ((u32)v1 >= 97U) {
                ((CLASS_W *)class_w)[127].bright = (u8)96;
            }
        }
        p += 68;
    } while (p != end);

    v1 = v1 - 2;
    t7 = v1 & 0xFF;
    scene_entry_c = (u8)t7;
    if (t7 == 0) {
        text_sys_init();
        screen_bri = (u8)128;
        class_clear_all(0);
        cursor_init(0);
        back_clear_switch(1);
        sub_step_up();
    }
}
