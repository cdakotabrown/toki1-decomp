/* @SymbolSize: 208 */
/* @PostProc: patch_insn_reg_field 0x44:rt:t9:t0 0x4c:rt:t9:t0 0x54:rt:t0:t1 0x58:rt:t0:t1 0x5c:rt:t1:t2 0x60:rs:t1:t2 0x60:rt:t1:t2 0x68:rt:t1:t2 0x6c:rt:t2:t3 0x70:rs:t2:t3 0x70:rt:t2:t3 0x78:rt:t2:t3 0x7c:rt:t3:t4 0x80:rs:t3:t4 0x80:rt:t3:t4 0x88:rt:t3:t4 0x8c:rt:t4:t5 0x90:rs:t4:t5 0x90:rt:t4:t5 0x98:rt:t4:t5 0x9c:rt:t5:t6 0xa0:rs:t5:t6 0xa0:rt:t5:t6 0xa8:rt:t5:t6 0xac:rt:t6:t7 0xb0:rs:t6:t7 0xb0:rt:t6:t7 0xb8:rt:t6:t7 0xbc:rt:t7:t8 0xc0:rs:t7:t8 0xc0:rt:t7:t8 0xc8:rt:t7:t8 0xcc:rt:t8:t9 0xd0:rs:t8:t9 0xd0:rt:t8:t9 0xdc:rt:t8:t9 */
/* Function: date_girl2_mode4_res_ptr_init */
/* Overlay: DATE */
/* Address: 0x80150124 */
/* Size: 208 bytes (52 instructions) */
#include "types.h"

extern volatile s16 date_ev11_cel_aux;

extern volatile s32 date_ev11_res;
extern volatile s32 date_ev11_res_img;
extern volatile s32 date_ev11_res_anim_tbl;
extern volatile s16 date_ev11_res_aux;
extern volatile s32 date_ev11_res_pal;
extern volatile s32 date_ev11_res_pal2;
extern volatile s32 date_ev11_res_tile0;
extern volatile s32 date_ev11_res_tile1;
extern volatile s32 date_ev11_res_tile2;
extern volatile s32 date_ev11_res_tile3;
extern volatile s32 date_ev11_res_tile4;
extern volatile s32 date_ev11_res_tile5;
extern volatile s32 date_ev11_res_ext_tile;

void date_girl2_mode4_res_ptr_init(void) {
    date_ev11_res = (s32)0x801D2370;
    date_ev11_res_img = (s32)0x801D2374;
    date_ev11_res_anim_tbl = (s32)0x801D23A4;
    date_ev11_res_aux = date_ev11_cel_aux;
    date_ev11_res_pal = (s32)0x801B0000;
    date_ev11_res_pal2 = (s32)0x801D2000;
    date_ev11_res_tile0 = (s32)0x801B2000;
    date_ev11_res_tile1 = (s32)0x801B6000;
    date_ev11_res_tile2 = (s32)0x801BA000;
    date_ev11_res_tile3 = (s32)0x801BE000;
    date_ev11_res_tile4 = (s32)0x801C2000;
    date_ev11_res_tile5 = (s32)0x801C6000;
    date_ev11_res_ext_tile = (s32)0x801CE000;
}
