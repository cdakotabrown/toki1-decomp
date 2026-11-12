/* Address: 0x8013F444 (cross-prologue head absorbed from 0x8013F430) */
/* Size: 96 bytes (24 instructions incl. hoisted head) */
/* @SymbolStart: 0x8013F430 */
/* @SymbolSize: 96 */
/* @PostProc: patch_insn_reg_field 0x18:rt:v0:t7 0x1c:rd:v0:t7 0x1c:rs:v0:t7 0x20:rs:v0:t7 0x20:rt:v0:t7 0x2c:rs:v0:t7 0x34:rt:t7:t8 0x38:rs:t7:t8 0x38:rt:t7:t8 0x40:rs:t7:t8 */
#include "types.h"

extern void sub_sub_step_up(void);
extern void shougatu_menu_serifu_wait(void);
extern u8 shougatu_menu_slot;
extern u8 shougatu_menu_girl_tbl;
extern u8 now_year;

void shougatu_girl11_y96_serifu_check(void)
{
    s32 sel;

    sel = (&shougatu_menu_girl_tbl)[shougatu_menu_slot];
    if (sel == 11) {
        if (now_year == 96) {
            shougatu_menu_serifu_wait();
            return;
        }
    }
    sub_sub_step_up();
}
