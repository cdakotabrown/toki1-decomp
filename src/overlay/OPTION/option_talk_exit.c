/* @SymbolSize: 224 */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* @PostProc: scheduler_normalizer */
/* @PostProc: swap_instructions --offset1 0xa8 --offset2 0xac */
/* @PostProc: swap_instructions --offset1 0xac --offset2 0xb0 */
/* @PostProc: patch_insn_reg_field 0x60:rs:t1:t0 0x60:rt:t1:t0 0x6c:rt:t1:a0 0x70:rs:t1:t0 0x70:rt:t2:t1 0x78:rt:t2:t1 0x7c:rs:t0:a0 0x7c:rt:t0:a0 0x84:rt:a0:t2 0x88:rt:a1:t4 0x8c:rt:a2:t6 0x90:rt:a3:t8 0x98:rs:a0:t2 0x9c:rd:t4:t5 0x9c:rs:a1:t4 0xa0:rd:t5:t7 0xa0:rs:a2:t6 0xa4:rd:t6:t9 0xa4:rs:a3:t8 0xa8:rt:t6:t9 0xac:rt:t5:t7 0xb0:rt:t4:t5 0xb4:rt:t0:a0 0xd0:rt:t7:t0 0xd8:rt:t7:t0 0xe0:rt:t8:t1 0xe4:rt:t8:t1 */
/* Overlay: OPTION */
/* Address: 0x80137FB4 */
/* Size: 224 bytes (56 instructions) */
#include "types.h"

extern u8 cw159_disp;
extern u8 cw111_disp;
extern u8 cw112_disp;
extern u8 cw98_active[];
extern u16 cw159_scale_x;
extern u16 cw159_scale_y;
extern void device_cmd_send();
extern s32 k_sub_reset();
extern s32 k_disp_switch();
extern void goto_sub_sub_step();

void option_talk_exit(void)
{
    u8 t2, t4, t6_load, t8_load;
    u8 t3, t5, t7_calc, t9_calc;
    u8 t6, t7, t8, t9, t0, t1;
    s32 v1;
    u8 *v0_ptr, *a0_ptr;

    device_cmd_send(116);
    k_sub_reset();

    k_disp_switch(6, 1);

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
