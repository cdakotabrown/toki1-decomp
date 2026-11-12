/* @CompileEnv: IDO52_CONST_INREG_VALUES=-128,252,16 */
/* @PostProc: swap_instructions --offset1 0x5c --offset2 0x68 */
/* @PostProc: swap_instructions --offset1 0x60 --offset2 0x6c */
/* @PostProc: swap_instructions --offset1 0x64 --offset2 0x68 */
/* @PostProc: swap_instructions --offset1 0x68 --offset2 0x6c */
/* @PostProc: patch_insn_reg_field 0x14:rt:v0:t6 0x18:rt:v1:t2 0x1c:rt:t0:t3 0x24:rt:t6:t7 0x28:rt:t7:t8 0x2c:rt:t8:t9 0x30:rt:t9:t4 0x34:rt:t1:t5 0x38:rt:v0:t6 0x3c:rt:v0:t6 0x40:rt:t6:t7 0x44:rt:t7:t8 0x48:rt:t8:t9 0x4c:rt:v1:t2 0x50:rt:v1:t2 0x54:rt:t0:t3 0x58:rt:t0:t3 0x5c:rt:t1:t5 0x60:rt:t9:t4 0x64:rt:t0:t3 0x68:rt:v1:t2 0x6c:rt:v0:t6 */
/* Address: 0x80141B34, 148 bytes (37 insns) */
#include "types.h"

extern void menu_set(s32 a0, s32 a1, s16 *offset, s16 *pos, s16 *color, s16 *size);
extern void cursor_init(s32 a0);
extern void set_flag_b3_high_bit(s32 a0);

void taiiku_menu_init(void)
{
    s32 pad;
    s16 offset[4];
    s16 pos[4];
    s16 color[4];
    s16 size[4];

    offset[1] = -128;
    offset[0] = -128;
    pos[0] = 48;
    pos[1] = 64;
    pos[2] = 80;
    color[1] = 252;
    color[0] = 252;
    size[1] = 16;
    size[0] = 16;
    size[2] = 16;
    color[2] = 252;
    offset[2] = -128;

    menu_set(0, 3, offset, pos, color, size);
    cursor_init(0);
    set_flag_b3_high_bit(1);
}
