/* @CompileEnv: IDO52_AS1_NO_LA_PULL=2,3 */
/* @PostProc: patch_stack_complete --old 104 --new 192 */
/* @PostProc: remap_stack_slots --map '40:52,56:68,72:84,88:100' */
/* @PostProc: swap_instructions --offset1 0x54 --offset2 0x60 */
/* @PostProc: swap_instructions --offset1 0x58 --offset2 0x60 */
/* @PostProc: swap_instructions --offset1 0x5c --offset2 0x60 */
/* @PostProc: swap_instructions --offset1 0x68 --offset2 0x74 */
/* @PostProc: swap_instructions --offset1 0x6c --offset2 0x74 */
/* @PostProc: swap_instructions --offset1 0x70 --offset2 0x74 */
/* @PostProc: swap_instructions --offset1 0x138 --offset2 0x144 */
/* @PostProc: swap_instructions --offset1 0x13c --offset2 0x144 */
/* @PostProc: swap_instructions --offset1 0x140 --offset2 0x144 */
/* @PostProc: swap_instructions --offset1 0x14c --offset2 0x158 */
/* @PostProc: swap_instructions --offset1 0x150 --offset2 0x158 */
/* @PostProc: swap_instructions --offset1 0x154 --offset2 0x158 */
/* @PostProc: patch_insn_reg_field 0xe4:rt:t9:v0 0xe8:rs:t9:v0 0xe8:rt:t9:v0 0xf0:rs:t9:v0 0xf0:rt:t0:t9 0xf8:rt:t0:t9 0x110:rt:t1:t0 0x114:rs:t1:t0 0x114:rt:t1:t0 0x124:rs:t1:t0 0x124:rt:t2:t1 0x128:rt:t2:t1 0x138:rt:t3:t2 0x13c:rt:t5:t4 0x140:rt:t7:t6 0x144:rt:t9:t8 0x14c:rd:t4:t3 0x14c:rs:t3:t2 0x150:rd:t6:t5 0x150:rs:t5:t4 0x154:rd:t8:t7 0x154:rs:t7:t6 0x158:rd:t0:t9 0x158:rs:t9:t8 0x15c:rt:t0:t9 0x160:rt:t8:t7 0x164:rt:t6:t5 0x16c:rt:t4:t3 0x180:rt:t4:t3 0x184:rs:t4:t3 0x184:rt:t4:t3 0x190:rd:t1:t0 0x198:rd:t1:t0 0x19c:rs:t4:t3 0x1a0:rd:t2:t1 0x1a0:rt:t1:t0 0x1a4:rd:t3:t2 0x1a4:rt:t2:t1 0x1a8:rs:t3:t2 0x1ac:rd:t5:t4 0x1b0:rs:t5:t4 0x1b4:rd:t6:t5 0x1b4:rt:t5:t4 0x1b8:rs:t5:t4 0x1bc:rd:t6:t5 0x1c4:rt:t6:t5 0x1d8:rt:t7:v0 0x1e4:rt:t7:v0 0x1f0:rt:t8:t6 0x1f4:rt:t9:t7 0x1f8:rt:t9:t7 0x1fc:rt:t8:t6 */
/* @SymbolStart: 0x8013493C */
/* @SymbolSize: 560 */
/* Address: 0x80134944, Size: 552 bytes (138 instructions), + absorbed */
#include "types.h"
#include "tokimeki/class_w.h"

extern void icon_disp_switch();
extern void option_row_pos_set();
extern void menu_set();
extern void goto_sub_step();
extern void goto_sub_sub_step();
extern void option_row_slide();

extern u8 step_mode;
extern u8 class_w[];

void option_zoom(void)
{
    s16 sp100[8];
    s16 sp84[8];
    s16 sp68[8];
    s16 sp52[8];
    s32 i;

    if (step_mode == 0) {
        icon_disp_switch(0);
        ((CLASS_W *)class_w)[107].disp_flag = (u8)(((CLASS_W *)class_w)[107].disp_flag & ~0x80);
        for (i = 1; i < 9; i++) {
            class_w[i * 68 + 7279] =
                (u8)(class_w[i * 68 + 7279] & ~0x80);
        }

        ((CLASS_W *)class_w)[106].scale_x = (s16)((((CLASS_W *)class_w)[106].scale_x / 9) * 8);
        if (((CLASS_W *)class_w)[106].scale_x < 9) {
            step_mode = (u8)(step_mode + 1);
        }
    } else if (step_mode == 2) {
        icon_disp_switch(0);
        ((CLASS_W *)class_w)[107].disp_flag = (u8)(((CLASS_W *)class_w)[107].disp_flag & 0xFF7F);
        for (i = 1; i < 9; i++) {
            class_w[i * 68 + 7279] =
                (u8)(class_w[i * 68 + 7279] & ~0x80);
        }

        ((CLASS_W *)class_w)[106].scale_x = (s16)((((CLASS_W *)class_w)[106].scale_x * 9) / 8);
        ((CLASS_W *)class_w)[106].scale_y = (s16)((((CLASS_W *)class_w)[106].scale_y * 9) / 8);
        if (((CLASS_W *)class_w)[106].scale_x >= 16384) {
            step_mode = 1;
        }
    } else {
        option_row_pos_set(sp52);
        menu_set(0, 8, sp52, sp68, sp84, sp100);
        goto_sub_step(0);
        goto_sub_sub_step(2);
    }

    option_row_slide();
    ((CLASS_W *)class_w)[106].disp_flag = 0;
}
