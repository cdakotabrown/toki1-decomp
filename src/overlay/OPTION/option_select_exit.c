/* @SymbolSize: 216 */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* @PostProc: scheduler_normalizer */
/* @PostProc: swap_instructions --offset1 0xa0 --offset2 0xa4 */
/* @PostProc: swap_instructions --offset1 0xa4 --offset2 0xa8 */
/* @PostProc: patch_insn_reg_field 0x58:rs:t1:t0 0x58:rt:t1:t0 0x64:rt:t1:a0 0x68:rs:t1:t0 0x68:rt:t2:t1 0x70:rt:t2:t1 0x74:rs:t0:a0 0x74:rt:t0:a0 0x7c:rt:a0:t2 0x80:rt:a1:t4 0x84:rt:a2:t6 0x88:rt:a3:t8 0x90:rs:a0:t2 0x94:rd:t4:t5 0x94:rs:a1:t4 0x98:rd:t5:t7 0x98:rs:a2:t6 0x9c:rd:t6:t9 0x9c:rs:a3:t8 0xa0:rt:t6:t9 0xa4:rt:t5:t7 0xa8:rt:t4:t5 0xac:rt:t0:a0 0xc8:rt:t7:t0 0xd0:rt:t7:t0 0xd8:rt:t8:t1 0xdc:rt:t8:t1 */
/* Overlay: OPTION */
/* Address: 0x80138604 */
/* Size: 216 bytes (54 instructions) */
#include "types.h"

extern u8 cw159_disp;
extern u8 cw111_disp;
extern u8 cw112_disp;
extern u8 cw98_active[];
extern u16 cw159_scale_x;
extern u16 cw159_scale_y;
extern s32 k_disp_switch();
extern s32 k_sub_reset();
extern void goto_sub_sub_step();

void option_select_exit(void)
{
    u8 t2, t4, t6_load, t8_load;
    u8 t3, t5, t7_calc, t9_calc;
    u8 t6, t7, t8, t9, t0, t1;
    s32 v1;
    u8 *v0_ptr, *a0_ptr;

    k_disp_switch(6, 1);
    k_sub_reset();

    t6 = cw159_disp;
    t7 = t6 & 0xFF7F;
    cw159_disp = t7;

    t8 = cw111_disp;
    t9 = t8 & 0xFF7F;
    cw111_disp = t9;

    t0 = cw112_disp;
    t1 = t0 & 0xFF7F;
    cw112_disp = t1;

    v0_ptr = &cw98_active[0];
    a0_ptr = &cw98_active[816];

    v1 = -129;

    do {
        t2 = v0_ptr[1023];
        t4 = v0_ptr[1091];
        t6_load = v0_ptr[1159];
        t8_load = v0_ptr[1227];

        v0_ptr = v0_ptr + 272;

        t3 = t2 & v1;
        t5 = t4 & v1;
        t7_calc = t6_load & v1;
        t9_calc = t8_load & v1;

        v0_ptr[751] = t3;
        v0_ptr[819] = t5;
        v0_ptr[887] = t7_calc;
        v0_ptr[955] = t9_calc;
    } while (v0_ptr != a0_ptr);

    goto_sub_sub_step(0);

    cw159_disp = 0;
    cw159_scale_x = 4096;
    cw159_scale_y = 4096;
}
