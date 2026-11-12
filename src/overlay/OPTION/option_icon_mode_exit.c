/* @CompileFlags: -Wo,-loopunroll,0 */
/* @CompileEnv: IDO52_AS1_NO_LA_PULL=4,5 */
/* @PostProc: patch_stack_complete --old 112 --new 192 */
/* @PostProc: remap_stack_slots --map '48:56,64:72,80:88,96:104' */
/* @PostProc: swap_instructions --offset1 0x44 --offset2 0x48 */
/* @PostProc: swap_instructions --offset1 0xec --offset2 0xf0 */
/* @PostProc: patch_insn_reg_field 0x48:rt:v1:a0 0x4c:rs:v1:a0 0x4c:rt:v1:a0 0x54:rt:a0:v1 0x60:rt:a0:v1 0x64:rt:v1:a0 0xc4:rt:t1:v0 0xc8:rs:t1:v0 0xc8:rt:t1:v0 0xd0:rs:t1:v0 0xd0:rt:t2:t1 0xd8:rt:t2:t1 0xf0:rt:v1:a0 0xf4:rs:v1:a0 0xf4:rt:v1:a0 0xfc:rt:a0:v1 0x100:rt:t3:t2 0x108:rd:t4:t3 0x108:rs:t3:t2 0x108:rt:a0:v1 0x10c:rt:v1:a0 0x110:rt:t4:t3 0x124:rt:t8:t7 0x128:rs:t8:t7 0x128:rt:t8:t7 0x134:rd:t5:t4 0x13c:rd:t5:t4 0x140:rs:t8:t7 0x144:rd:t6:t5 0x144:rt:t5:t4 0x148:rd:t7:t6 0x148:rt:t6:t5 0x14c:rs:t7:t6 0x150:rd:t9:t8 0x154:rs:t9:t8 0x158:rd:t0:t9 0x158:rt:t9:t8 0x15c:rs:t9:t8 0x160:rd:t0:t9 0x168:rt:t0:t9 0x17c:rt:t1:v0 0x188:rt:t1:v0 0x194:rt:t2:t0 0x198:rt:t3:t1 0x19c:rt:t3:t1 0x1a0:rt:t2:t0 */
/* Address: 0x801342A8, Size: 468 bytes (117 instructions) */
#include "types.h"
#include "tokimeki/class_w.h"

extern void icon_init();
extern void icon_disp_switch();
extern void option_row_pos_set();
extern void menu_set();
extern void goto_sub_step();
extern void goto_sub_sub_step();
extern void option_row_slide();

extern u8 step_mode;
extern u8 class_w[];

void option_icon_mode_exit(void)
{
    s16 sp104[8];
    s16 sp88[8];
    s16 sp72[8];
    s16 sp56[8];
    u8 *p;
    u8 *pend;

    icon_init();
    icon_disp_switch(0);

    if (step_mode == 0) {
        icon_disp_switch(0);
        pend = &class_w[204];
        p = &class_w[0];
        do {
            p[7279] = (u8)(p[7279] & ~0x80);
            p += 68;
        } while (p != pend);

        ((CLASS_W *)class_w)[106].scale_x = (s16)((((CLASS_W *)class_w)[106].scale_x / 9) * 8);
        if (((CLASS_W *)class_w)[106].scale_x < 9) {
            step_mode = (u8)(step_mode + 1);
        }
    } else if (step_mode == 2) {
        icon_disp_switch(0);
        pend = &class_w[204];
        p = &class_w[0];
        do {
            p[7279] = (u8)(p[7279] & ~0x80);
            p += 68;
        } while (p != pend);

        ((CLASS_W *)class_w)[106].scale_x = (s16)((((CLASS_W *)class_w)[106].scale_x * 9) / 8);
        ((CLASS_W *)class_w)[106].scale_y = (s16)((((CLASS_W *)class_w)[106].scale_y * 9) / 8);
        if (((CLASS_W *)class_w)[106].scale_x >= 16384) {
            step_mode = 1;
        }
    } else {
        option_row_pos_set(sp56);
        menu_set(0, 8, sp56, sp72, sp88, sp104);
        goto_sub_step(0);
        goto_sub_sub_step(2);
    }

    option_row_slide();
    ((CLASS_W *)class_w)[106].disp_flag = 0;
}
