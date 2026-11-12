/* @SymbolStart: 0x80140100 */
/* @SymbolSize: 276 */
/* @PostProc: swap_instructions --offset1 0x10 --offset2 0x18 */
/* @PostProc: swap_instructions --offset1 0x14 --offset2 0x1c */
/* @PostProc: swap_instructions --offset1 0x1c --offset2 0x20 */
/* @PostProc: patch_insn_reg_field 0x10:rt:t6:v0 0x14:rs:t6:v0 0x40:rt:t7:t6 0x44:rt:t7:t6 0x50:rt:t8:t7 0x54:rs:t8:t7 0x54:rt:t8:t7 0x5c:rs:t8:t7 0x5c:rt:t9:t8 0x60:rt:t9:t8 0x74:rt:t0:t9 0x7c:rt:t0:t9 0x84:rt:t1:t0 0x88:rt:t1:t0 0x90:rt:t2:t1 0x94:rt:t2:t1 0xa0:rt:t3:t2 0xa4:rs:t3:t2 0xa4:rt:t3:t2 0xac:rs:t3:t2 0xac:rt:t4:t3 0xb0:rt:t4:t3 0xc4:rt:t5:t4 0xcc:rt:t5:t4 0xd4:rt:t6:t5 0xd8:rt:t6:t5 0xe0:rt:t7:t6 0xe4:rt:t7:t6 0xf0:rt:t8:t7 0xf4:rs:t8:t7 0xf4:rt:t8:t7 0xfc:rs:t8:t7 0xfc:rt:t9:t8 0x100:rt:t9:t8 */
/* Address: 0x80140108 (real entry 0x80140100 - cross-prologue hoisted */
/* Size: 276 bytes (69 instructions) with the absorbed head. */
#include "types.h"

extern s32 sub_sub_step_up();
extern void goto_sub_step();
extern u8 ev_flag06;
extern u8 ev_flag34;
extern u8 ev_flag42;
extern u8 now_girl;
extern s32 work_tarao;
extern s32 event_girl_no;
extern s32 script_on;

#define SEL (*(u8 *)0x801460A4)

s32 shougatu_girl_step_goto(void)
{
    s32 v0 = SEL;

    script_on = 0;
    if (v0 == 0) {
        now_girl = 0;
        event_girl_no = 0;
        work_tarao = 7;
        goto_sub_step(6);
        ev_flag06 = ev_flag06 + 1;
        return 0;
    }
    if (v0 == 7) {
        now_girl = 7;
        event_girl_no = 7;
        work_tarao = 7;
        goto_sub_step(7);
        ev_flag34 = ev_flag34 + 1;
        return 0;
    }
    if (v0 == 9) {
        now_girl = 9;
        event_girl_no = 9;
        work_tarao = 7;
        goto_sub_step(8);
        ev_flag42 = ev_flag42 + 1;
        return 0;
    }
    return sub_sub_step_up();
}
