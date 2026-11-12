/* @PostProc: patch_insn_reg_field   0x10:rt:v0:v1 0x18:rs:v0:v1 0x18:rt:v0:v1 0x1c:rt:t7:t8 0x30:rs:t7:t8 0x30:rt:t8:t9 0x34:rt:t8:t9 0x40:rs:v0:v1 0x4c:rs:v0:v1 0x5c:rt:t9:v0 0x60:rs:t9:v0 */
/* @SymbolStart: 0x80146030 */
/* @SymbolSize: 208 */
/* Address: 0x80146058 */
/* Size: 168 bytes (42 insns) */
#include "types.h"
extern volatile s32 bunka_bukatu;
extern volatile s32 bunka_girl;
extern u8 girl3_flag;

extern s32 goto_sub_step(s32);
extern s32 game_reset(void);

#define now_year (*(volatile u8 *)0x800EE79E)

void bunkasai_girl3_home_cl45_select(void)
{
    u8 val;
    s32 v1;
    u8 x;

    v1 = bunka_bukatu;
    x = girl3_flag;
    bunka_girl = 3;
    girl3_flag = x | 4;

switch (v1) {
    case 4:
        val = now_year;
        switch (val) {
        case 95: goto_sub_step(81); break;
        case 96: goto_sub_step(82); break;
        case 97: goto_sub_step(83); break;
        }
        break;
    case 5:
        goto_sub_step(84);
        break;
    default:
        game_reset();
        break;
    }
}
