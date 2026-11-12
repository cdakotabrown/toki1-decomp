/* @SymbolSize: 176 */
/* @PostProc: swap_instructions --offset1 0x40 --offset2 0x44 */
/* @PostProc: swap_instructions --offset1 0x3c --offset2 0x40 */
/* @PostProc: patch_insn_reg_field 0x44:rt:t9:t0 0x4c:rt:t9:t0 0x54:rt:t0:t1 0x58:rt:t0:t1 0x5c:rt:t1:t2 0x60:rs:t1:t2 0x60:rt:t1:t2 0x68:rt:t1:t2 0x6c:rt:t2:t3 0x70:rs:t2:t3 0x70:rt:t2:t3 0x78:rt:t2:t3 0x7c:rt:t3:t4 0x80:rs:t3:t4 0x80:rt:t3:t4 0x88:rt:t3:t4 0x8c:rt:t4:t5 0x90:rs:t4:t5 0x90:rt:t4:t5 0x98:rt:t4:t5 0x9c:rt:t5:t6 0xa0:rs:t5:t6 0xa0:rt:t5:t6 0xa8:rt:t5:t6 0xac:rt:t6:t7 0xb0:rs:t6:t7 0xb0:rt:t6:t7 0xbc:rt:t6:t7 */
#include "types.h"

extern u32 shugaku_maina_res_obj_a2;
extern u32 shugaku_maina_res_obj_a0;
extern u32 shugaku_maina_res_obj_a1;
extern s16 shugaku_maina_res_obj_scale;
extern u32 shugaku_maina_res_pal;
extern u32 shugaku_maina_res_spr0;
extern u32 shugaku_maina_res_spr1;
extern u32 shugaku_maina_res_spr2;
extern u32 shugaku_maina_res_spr3;
extern u32 shugaku_maina_res_spr4;
extern u32 shugaku_maina_res_spr5;

extern s16 shugaku_maina_res_scale_src;

void shugaku_ptr_init(void)
{
    shugaku_maina_res_obj_a2 = 0x801EA87C;
    shugaku_maina_res_obj_a0 = 0x801EA89C;
    shugaku_maina_res_obj_a1 = 0x801EA940;
    shugaku_maina_res_obj_scale = shugaku_maina_res_scale_src;
    shugaku_maina_res_pal = 0x801B0000;
    shugaku_maina_res_spr0 = 0x801B2000;
    shugaku_maina_res_spr1 = 0x801B6000;
    shugaku_maina_res_spr2 = 0x801BA000;
    shugaku_maina_res_spr3 = 0x801BE000;
    shugaku_maina_res_spr4 = 0x801C2000;
    shugaku_maina_res_spr5 = 0x801C6000;
}
