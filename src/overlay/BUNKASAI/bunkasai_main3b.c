/* @PostProc: patch_insn_reg_field   0x10:rt:v0:v1 0x18:rs:v0:v1 0x18:rt:v0:v1 0x1c:rt:t7:t8 0x30:rs:t7:t8 0x30:rt:t8:t9 0x34:rt:t8:t9 0x40:rs:v0:v1 0x4c:rs:v0:v1 0x50:rt:t9:v0 0x6c:rs:t9:v0 */
/* @SymbolStart: 0x8013B190 */
/* @SymbolSize: 204 */
/* Address: 0x8013B1B8 */
/* Size: 164 bytes (41 insns) */
#include "types.h"
extern volatile s32 bunka_bukatu;
extern volatile s32 bunka_girl;
extern u8 girl2_flag;

extern s32 goto_sub_step(s32);
extern s32 game_reset(void);

#define now_year (*(volatile u8 *)0x800EE79E)

void bunkasai_main3b(void)
{
    u8 val;
    s32 v1;
    u8 x;

    v1 = bunka_bukatu;
    x = girl2_flag;
    bunka_girl = 2;
    girl2_flag = x | 4;

switch (v1) {
    case 2:
        goto_sub_step(60);
        break;
    case 3:
        val = now_year;
        switch (val) {
        case 95: goto_sub_step(61); break;
        case 96: goto_sub_step(63); break;
        case 97: goto_sub_step(64); break;
        }
        break;
    default:
        game_reset();
        break;
    }
}
