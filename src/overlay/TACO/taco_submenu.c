/* @SymbolStart: 0x80136998 */
/* @SymbolSize: 440 */
/* @RodataBase: 0x8015D798 */
/* Address: 0x80136998 (cross-prologue head) / 0x801369A0 (prologue) */
/* Size: 440 bytes (110 insns incl. the 2-insn head) */
#include "types.h"

extern s32 pad_trig;
extern volatile s32 taco_submenu_sel;

extern u8 taco_rensya_flag[];
extern u8 taco_turn_btn[];
extern u8 taco_shot_btn[];
extern u8 taco_burst_btn[];

extern void sub_sub_step_up(void);
extern void sd_cmd_limit_send(s32, s32);
extern void cursor_step(u8 *, s32);

void taco_submenu(void)
{
    s32 pad;
    s32 sel;

    pad = pad_trig;
    if ((pad & 0x5000) != 0) {
        sd_cmd_limit_send(0, 1280);
        pad = pad_trig;
    }

    if ((pad & 0x1000) != 0) {
        sel = taco_submenu_sel - 1;
        taco_submenu_sel = sel;
    }
    sel = taco_submenu_sel;
    if ((pad & 0x4000) != 0) {
        sel = sel + 1;
    }
    if (sel < 0) {
        sel = 4;
    }
    if (sel >= 5) {
        sel = 0;
    }
    taco_submenu_sel = sel;

    if ((pad & 0xA020) != 0) {
        taco_submenu_sel = sel;
        if (sel != 4) {
            sd_cmd_limit_send(0, 1281);
            pad = pad_trig;
        }
        sel = taco_submenu_sel;
        switch ((u32)sel) {
        case 0:
            cursor_step(taco_rensya_flag, 2);
            pad = pad_trig;
            break;
        case 1:
            cursor_step(taco_turn_btn, 3);
            pad = pad_trig;
            break;
        case 2:
            cursor_step(taco_shot_btn, 3);
            pad = pad_trig;
            break;
        case 3:
            cursor_step(taco_burst_btn, 3);
            pad = pad_trig;
            break;
        case 4:
            if ((pad & 0x20) != 0) {
                sd_cmd_limit_send(0, 1282);
                sub_sub_step_up();
                pad = pad_trig;
            }
            break;
        }
    }

    if ((pad & 0x840) != 0) {
        sd_cmd_limit_send(0, 1282);
        sub_sub_step_up();
    }
}
