/* @SymbolStart: 0x8013836C */
/* @SymbolSize: 100 */
/* @PostProc: patch_insn_reg_field 0x3c:rt:t1:t0 0x40:rt:t1:t0 0x44:rt:t2:t1 0x48:rs:t2:t1 0x48:rt:t2:t1 0x50:rs:t2:t1 0x50:rt:t3:t2 0x58:rt:t3:t2 */
/* kangei_birthday_step - KANGEI overlay, Address: 0x80138380, Size: 80 bytes (20 insns) */
#include "types.h"

extern void sub_sub_step_up(void);

extern s32 p_birthday;
extern u8 now_day;
extern u8 now_month;
extern u8 now_sub_sub_step;

void kangei_birthday_step(void)
{
    s32 v0_in;
    u8 t6_in;
    u8 val;

    v0_in = p_birthday;
    t6_in = now_day;
    if ((s32)((u32)(v0_in << 23) >> 27) == t6_in) {
        if ((u8)(v0_in & 0xF) == now_month) {
            val = now_sub_sub_step;
            now_sub_sub_step = (u8)(val + 12);
            return;
        }
    }
    sub_sub_step_up();
}
