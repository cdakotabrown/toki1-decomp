/* @PostProc: patch_insn_reg_field 0x44:rt:t9:t0 0x4c:rt:t9:t0 0x54:rt:t0:t1 0x58:rt:t0:t1 0x5c:rt:t1:t2 0x60:rs:t1:t2 0x60:rt:t1:t2 0x68:rt:t1:t2 0x6c:rt:t2:t3 0x70:rs:t2:t3 0x70:rt:t2:t3 0x78:rt:t2:t3 0x7c:rt:t3:t4 0x80:rs:t3:t4 0x80:rt:t3:t4 0x88:rt:t3:t4 0x8c:rt:t4:t5 0x90:rs:t4:t5 0x90:rt:t4:t5 0x98:rt:t4:t5 0x9c:rt:t5:t6 0xa0:rs:t5:t6 0xa0:rt:t5:t6 0xa8:rt:t5:t6 0xac:rt:t6:t7 0xb0:rs:t6:t7 0xb0:rt:t6:t7 0xb8:rt:t6:t7 0xbc:rt:t7:t8 0xc0:rs:t7:t8 0xc0:rt:t7:t8 0xc8:rt:t7:t8 0xcc:rt:t8:t9 0xd0:rs:t8:t9 0xd0:rt:t8:t9 0xd8:rt:t8:t9 0xdc:rt:t9:t0 0xe0:rs:t9:t0 0xe0:rt:t9:t0 0xe8:rt:t9:t0 0xec:rt:t0:t1 0xf0:rs:t0:t1 0xf0:rt:t0:t1 0xf8:rt:t0:t1 0xfc:rt:t1:t2 0x100:rs:t1:t2 0x100:rt:t1:t2 0x108:rt:t1:t2 0x10c:rt:t2:t3 0x110:rs:t2:t3 0x110:rt:t2:t3 0x11c:rt:t2:t3 */
/* ending_girl6_set - ENDING overlay, Address: 0x80135460. */
#include "types.h"
#include "tokimeki/ending_girl_res.h"

extern volatile u32 ending_girl_res_6_anim_tbl;
extern volatile s16 ending_girl_res_6_aux;
extern volatile s16 ending_girl6_girl14_cel_aux;
extern ENDING_GIRL_RES ending_girl_res[];

void ending_girl6_set(void)
{
    ending_girl_res[6].hdr = 0x801EE6E8;
    ending_girl_res[6].img = 0x801EE704;
    ending_girl_res_6_anim_tbl = 0x801EE768;
    ending_girl_res_6_aux = ending_girl6_girl14_cel_aux;
    ending_girl_res[6].pal = 0x801A0000;
    ending_girl_res[6].tile[0] = 0x801A2000;
    ending_girl_res[6].tile[1] = 0x801A6000;
    ending_girl_res[6].tile[2] = 0x801CA000;
    ending_girl_res[6].tile[3] = 0x801CE000;
    ending_girl_res[6].tile[4] = 0x801AA000;
    ending_girl_res[6].tile[5] = 0x801AE000;
    ending_girl_res[6].tile[6] = 0x801B2000;
    ending_girl_res[6].tile[7] = 0x801B6000;
    ending_girl_res[6].tile[8] = 0x801BA000;
    ending_girl_res[6].tile[9] = 0x801BE000;
    ending_girl_res[6].tile[10] = 0x801C2000;
    ending_girl_res[6].tile[11] = 0x801C6000;
}
