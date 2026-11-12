/* @SymbolStart: 0x801365c4 */
/* @SymbolSize: 108 */
/* @PostProc: patch_insn_reg_field 0x18:rt:v0:t7 0x1c:rd:v0:t7 0x1c:rs:v0:t7 0x20:rs:v0:t7 0x20:rt:v0:t7 0x28:rs:v0:t7 0x40:rt:t7:t8 0x44:rs:t7:t8 0x44:rt:t7:t8 0x4c:rs:t7:t8 */
/* kangei_choice_fade_out - KANGEI overlay, Address: 0x801365D8, Size: 88 bytes (22 insns) */
#include "types.h"

extern void fade_out_step(void);
extern void date_fade_out(void);
extern void kangei_fade(void);

extern u8 kangei_choice_no;
extern u8 kangei_choice_tbl;

extern u8 kangei_choice_rnd;

void
kangei_choice_fade_out(void)
{
    u8 flag;
    u8 idx;
    u8 sel;

    idx = kangei_choice_no;
    sel = *(&kangei_choice_tbl + (s32)idx);

    if (sel != 0) {
        fade_out_step();
        return;
    }

    flag = kangei_choice_rnd;
    if (flag != 0) {
        date_fade_out();
    } else {
        kangei_fade();
    }
}
