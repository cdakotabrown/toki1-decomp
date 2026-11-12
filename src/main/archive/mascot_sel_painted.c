/* Address: 0x800605F8 */
/* Size: 324 bytes (81 instructions) */
/* None produce 81 instructions. Requires ASM-only match or future tool. */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: custom_script custom_func_800605F8.py */
#include "types.h"

extern s16 cursor_cw_x;
extern s16 cursor_cw_y;
extern s8 menu_csr;
extern s32 pad_trig;
extern u8 mascot_flag[];
extern u8 reserved_date[];

extern void menu_check(s32, s16, s16);
extern void menu_bar_show(s32);
extern void func_80050030(s32);
extern void k_disp_inc(void);
extern void mascot_init(s32);
extern void goto_sub_sub_step(s32);
extern void sub_sub_step_up(void);

void mascot_sel(void) {
    s32 charIdx;
    s32 flags;

    menu_check(1, cursor_cw_x, cursor_cw_y);
    menu_bar_show(1);
    func_80050030(1);
    k_disp_inc();

    charIdx = menu_csr;
    if (charIdx != -1) {
        mascot_init(0);
        charIdx = menu_csr;
        if (mascot_flag[charIdx] || reserved_date[charIdx]) {
            mascot_init(charIdx + 1);
        }
    }

    flags = pad_trig;
    charIdx = menu_csr;

    if (flags & 0x40) {
        goto_sub_sub_step(0);
        return;
    }

    if ((flags & 0x20) == 0) {
        return;
    }

    if (charIdx == -1) {
        return;
    }

    if (charIdx == 13) {
        goto_sub_sub_step(0);
        return;
    }

    if (mascot_flag[charIdx] || reserved_date[charIdx]) {
        mascot_init(0);
        charIdx = menu_csr;
        mascot_init(charIdx + 1);
        sub_sub_step_up();
    }
}
