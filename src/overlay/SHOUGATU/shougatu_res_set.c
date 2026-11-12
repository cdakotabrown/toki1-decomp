/* @PostProc: patch_insn_reg_field 0x44:rt:t9:t0 0x4c:rt:t0:t1 0x50:rt:t9:t0 0x58:rs:t0:t1 0x58:rt:t0:t1 0x5c:rt:t0:t1 0x64:rt:t1:t2 0x68:rt:t1:t2 0x6c:rt:t2:t3 0x70:rs:t2:t3 0x70:rt:t2:t3 0x78:rt:t2:t3 0x7c:rt:t3:t4 0x80:rs:t3:t4 0x80:rt:t3:t4 0x88:rt:t3:t4 0x8c:rt:t4:t5 0x90:rs:t4:t5 0x90:rt:t4:t5 0x98:rt:t4:t5 0x9c:rt:t5:t6 0xa0:rs:t5:t6 0xa0:rt:t5:t6 0xa8:rt:t5:t6 0xac:rt:t6:t7 0xb0:rs:t6:t7 0xb0:rt:t6:t7 0xb8:rt:t6:t7 0xbc:rt:t7:t8 0xc0:rs:t7:t8 0xc0:rt:t7:t8 0xc8:rt:t7:t8 0xcc:rt:t8:t9 0xd0:rs:t8:t9 0xd0:rt:t8:t9 0xd8:rt:t8:t9 0xdc:rt:t9:t0 0xe0:rs:t9:t0 0xe0:rt:t9:t0 0xe8:rt:t9:t0 0xec:rt:t0:t1 0xf0:rs:t0:t1 0xf0:rt:t0:t1 0xf8:rt:t0:t1 0xfc:rt:t1:t2 0x100:rs:t1:t2 0x100:rt:t1:t2 0x108:rt:t1:t2 0x10c:rt:t2:t3 0x110:rs:t2:t3 0x110:rt:t2:t3 0x118:rt:t2:t3 0x11c:rt:t3:t4 0x120:rs:t3:t4 0x120:rt:t3:t4 0x12c:rt:t3:t4 */
/* Address: 0x801397E0 */
/* Size: 288 bytes (72 instructions) */
#include "types.h"
#include "tokimeki/etc_cg_res.h"

extern volatile s32 shougatu_girl1_res_anim_tbl;
extern volatile s16 shougatu_girl1_res_aux;
extern volatile s16 shougatu_girl1_res_aux_src;
extern ETC_CG_RES shougatu_girl1_res;

void shougatu_res_set(void)
{
    shougatu_girl1_res.hdr = 0x801E60CC;
    shougatu_girl1_res.img = 0x801E60D0;
    shougatu_girl1_res_anim_tbl = 0x801E60E4;
    shougatu_girl1_res_aux = shougatu_girl1_res_aux_src;
    shougatu_girl1_res.pal[0] = 0x801AE000;
    shougatu_girl1_res.pal[1] = 0x801B0000;
    shougatu_girl1_res.tile[0][0] = 0x801B2000;
    shougatu_girl1_res.tile[0][1] = 0x801B6000;
    shougatu_girl1_res.tile[0][2] = 0x801BA000;
    shougatu_girl1_res.tile[0][3] = 0x801BE000;
    shougatu_girl1_res.tile[0][4] = 0x801C2000;
    shougatu_girl1_res.tile[0][5] = 0x801C6000;
    shougatu_girl1_res.tile[1][0] = 0x801CE000;
    shougatu_girl1_res.tile[1][1] = 0x801D2000;
    shougatu_girl1_res.tile[1][2] = 0x801D6000;
    shougatu_girl1_res.tile[1][3] = 0x801DA000;
    shougatu_girl1_res.tile[1][4] = 0x801DE000;
    shougatu_girl1_res.tile[1][5] = 0x801E2000;
}
