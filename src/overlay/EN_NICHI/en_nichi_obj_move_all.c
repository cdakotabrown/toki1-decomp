/* @SymbolSize: 176 */
/* en_nichi_obj_move_all - EN_NICHI overlay, Address: 0x80133AA0, Size: 208 bytes (52 insns) */
/* @PostProc: patch_insn_reg_field 0x3c:rt:v0:t6 */
/* @PostProc: patch_insn_reg_field 0x40:rt:t7:t8 */
/* @PostProc: patch_insn_reg_field 0x44:rt:t6:t7 */
/* @PostProc: patch_insn_reg_field 0x44:rs:v0:t6 */
/* @PostProc: patch_insn_reg_field 0x48:rs:t6:t7 */
/* @PostProc: patch_insn_reg_field 0x50:rs:t7:t8 */
/* @PostProc: patch_insn_reg_field 0x50:rt:t7:t8 */
/* @PostProc: patch_insn_reg_field 0x58:rt:t7:t8 */
/* @PostProc: patch_insn_reg_field 0x60:rt:v0:t9 */
/* @PostProc: patch_insn_reg_field 0x68:rs:v0:t9 */
#include "types.h"

extern void en_nichi_obj_react();
extern void en_nichi_obj_move();
extern void en_nichi_obj_move_select();

extern u8 cw152_prio;
extern u32 en_nichi_obj_wk[];

void en_nichi_obj_move_all(void)
{
    s32 s0;
    u32 t7;
    u32 t9;
    u32 *s1;
    u8 t8;
    u32 t6_tmp;

    s1 = en_nichi_obj_wk;
    s0 = 0;
    do {
        t6_tmp = *s1;
        t7 = t6_tmp & 0x80U;
        if (t7 != 0) {
            goto loop_inc;
        }
        t8 = cw152_prio;
        if (t8 != 7) {
            goto call_8013497C;
        }
        t9 = s1[1];
        if (t9 == 0) {
            goto call_8013497C;
        }
        en_nichi_obj_react(s0);
        en_nichi_obj_move(s0);
        goto loop_inc;
call_8013497C:
        en_nichi_obj_move_select(s0);
        en_nichi_obj_move(s0);
loop_inc:
        s0++;
        s1 = (u32 *)((u8 *)s1 + 48);
    } while (s0 != 10);
}
