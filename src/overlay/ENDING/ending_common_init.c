/* @PostProc: patch_insn_reg_field 0x44:rt:t9:t0 0x4c:rt:t9:t0 0x54:rt:t0:t1 0x58:rt:t0:t1 0x5c:rt:t1:t2 0x60:rs:t1:t2 0x60:rt:t1:t2 0x68:rt:t1:t2 0x6c:rt:t2:t3 0x70:rs:t2:t3 0x70:rt:t2:t3 0x78:rt:t2:t3 0x7c:rt:t3:t4 0x80:rs:t3:t4 0x80:rt:t3:t4 0x88:rt:t3:t4 0x8c:rt:t4:t5 0x90:rs:t4:t5 0x90:rt:t4:t5 0x98:rt:t4:t5 0x9c:rt:t5:t6 0xa0:rs:t5:t6 0xa0:rt:t5:t6 0xa8:rt:t5:t6 0xac:rt:t6:t7 0xb0:rs:t6:t7 0xb0:rt:t6:t7 0xb8:rt:t6:t7 0xbc:rt:t7:t8 0xc0:rs:t7:t8 0xc0:rt:t7:t8 0xcc:rt:t7:t8 */
/* ending_common_init - ENDING overlay, Address: 0x80134D40, 48 instructions. */
#include "types.h"

extern s32 ending_common_ptr_tbl[];
extern volatile u32 ending_common_ptr_tbl_2;
extern volatile s16 ending_common_ptr_tbl_3;
extern volatile s16 ending_common_cel_aux;

void ending_common_init(void)
{
    (*(u32 *)&ending_common_ptr_tbl[0]) = 0x8019E458;
    (*(u32 *)&ending_common_ptr_tbl[1]) = 0x8019E45C;
    ending_common_ptr_tbl_2 = 0x8019E498;
    ending_common_ptr_tbl_3 = ending_common_cel_aux;
    (*(u32 *)&ending_common_ptr_tbl[4]) = 0x80180000;
    (*(u32 *)&ending_common_ptr_tbl[5]) = 0x8019A000;
    (*(u32 *)&ending_common_ptr_tbl[6]) = 0x80182000;
    (*(u32 *)&ending_common_ptr_tbl[7]) = 0x80186000;
    (*(u32 *)&ending_common_ptr_tbl[8]) = 0x8018A000;
    (*(u32 *)&ending_common_ptr_tbl[9]) = 0x8018E000;
    (*(u32 *)&ending_common_ptr_tbl[10]) = 0x80192000;
    (*(u32 *)&ending_common_ptr_tbl[11]) = 0x80196000;
}
