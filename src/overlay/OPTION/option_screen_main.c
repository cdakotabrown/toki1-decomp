/* @SymbolStart: 0x801389C0 */
/* @SymbolSize: 132 */
/* option_screen_main - OPTION overlay, Address: 0x801389C0, Size: 132 bytes (33 insns) */
/* @PostProc: patch_insn_reg_field 0x18:rt:v0:v1 0x1c:rs:v0:v1 0x1c:rt:v0:v1 0x30:rs:v0:v1 0x3c:rs:v0:v1 */
#include "types.h"

extern s32 step_state;
extern u8 now_sub_step;
extern void option_screen_load();
extern void option_screen_dispatch();
extern void game_reset();
extern void update_anim_state();
extern void basyo_show();

void option_screen_main(void) {
    s32 counter;
    u32 sel;

    counter = step_state;
    sel = now_sub_step;
    step_state = counter + 1;
    switch (sel) {
        case 0:
            option_screen_load();
            break;
        case 1:
            option_screen_dispatch();
            break;
        default:
            game_reset();
            break;
    }
    update_anim_state(0);
    basyo_show();
}
