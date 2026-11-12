/* @SymbolSize: 176 */
/* @PostProc: patch_insn_reg_field 0x44:rt:t9:t0 0x4c:rt:t9:t0 0x54:rt:t0:t1 0x58:rt:t0:t1 0x5c:rt:t1:t2 0x60:rs:t1:t2 0x60:rt:t1:t2 0x68:rt:t1:t2 0x6c:rt:t2:t3 0x70:rs:t2:t3 0x70:rt:t2:t3 0x78:rt:t2:t3 0x7c:rt:t3:t4 0x80:rs:t3:t4 0x80:rt:t3:t4 0x88:rt:t3:t4 0x8c:rt:t4:t5 0x90:rs:t4:t5 0x90:rt:t4:t5 0x98:rt:t4:t5 0x9c:rt:t5:t6 0xa0:rs:t5:t6 0xa0:rt:t5:t6 0xa8:rt:t5:t6 0xac:rt:t6:t7 0xb0:rs:t6:t7 0xb0:rt:t6:t7 0xbc:rt:t6:t7 */
/* Function: date_girl4_mode3_bufptr_init */
/* Overlay: DATE */
/* Address: 0x801586F0 */
/* Size: 176 bytes (44 instructions) */
#include "types.h"

extern volatile s16 date_ev3f_cel_scale;

extern volatile s32 date_scene_init10_res;
extern volatile s32 date_scene_init10_res_cels;
extern volatile s32 date_scene_init10_res_anim_tbl;
extern volatile s16 date_scene_init10_res_scale;
extern volatile s32 date_scene_init10_res_pal;
extern volatile s32 date_scene_init10_res_bg0;
extern volatile s32 date_scene_init10_res_bg1;
extern volatile s32 date_scene_init10_res_bg2;
extern volatile s32 date_scene_init10_res_bg3;
extern volatile s32 date_scene_init10_res_bg4;
extern volatile s32 date_scene_init10_res_bg5;

void date_girl4_mode3_bufptr_init(void) {
    date_scene_init10_res = (s32)0x801D2240;
    date_scene_init10_res_cels = (s32)0x801D2248;
    date_scene_init10_res_anim_tbl = (s32)0x801D2288;
    date_scene_init10_res_scale = date_ev3f_cel_scale;
    date_scene_init10_res_pal = (s32)0x801B0000;
    date_scene_init10_res_bg0 = (s32)0x801B2000;
    date_scene_init10_res_bg1 = (s32)0x801B6000;
    date_scene_init10_res_bg2 = (s32)0x801BA000;
    date_scene_init10_res_bg3 = (s32)0x801BE000;
    date_scene_init10_res_bg4 = (s32)0x801C2000;
    date_scene_init10_res_bg5 = (s32)0x801C6000;
}
