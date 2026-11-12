/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: patch_stack_complete --old 48 --new 56 */
/* @PostProc: remap_stack_slots --from 44 --to 48 --from 40 --to 44 */
/* @SymbolStart: 0x80138B68 */
/* @SymbolSize: 336 */
/* name_ent_fuji_confirm_input - NAME_ENT overlay, Address: 0x80138B78, Size: 320 bytes (80 insns) */
#include "types.h"

extern s16 cursor_cw_x;
extern s16 cursor_cw_y;
extern u32 pad_trig;
extern s16 cw97_x;

extern void step_directional_scroll_once();
extern void device_cmd_send();
extern void sub_sub_step_up();
extern void goto_sub_sub_step();

void name_ent_fuji_confirm_input(void)
{
    s32 cursorX;
    s32 cursorY;
    u32 input;

    cursorX = cursor_cw_x;
    cursorY = cursor_cw_y;
    step_directional_scroll_once();

    if (cursorX >= -47 && cursorX < 16) {
        if (cursorY >= 81 && cursorY < 96) {
            cw97_x = -10;
        }
    }

    if (cursorX >= 17 && cursorX < 80) {
        if (cursorY >= 81 && cursorY < 96) {
            cw97_x = 46;
        }
    }

    input = pad_trig;

    if (input & 0x0020) {

        if (cursorX >= -47 && cursorX < 16) {
            if (cursorY >= 81 && cursorY < 96) {
                device_cmd_send(1281);
                sub_sub_step_up();
            }
        }

        if (cursorX >= 17 && cursorX < 80) {
            if (cursorY >= 81 && cursorY < 96) {
                goto_sub_sub_step(0);
            }
        }
        return;
    }

    if (input & 0x0800) {
        device_cmd_send(1281);
        sub_sub_step_up();
        return;
    }

    if (input & 0x0040) {
        goto_sub_sub_step(0);
    }
}
