/* @SymbolStart: 0x80156CF0 */
/* @SymbolSize: 352 */
/* @PostProc: scheduler_normalizer --block-aware */
/* @PostProc: swap_instructions --offset1 0x11c --offset2 0x124 */
/* @PostProc: swap_instructions --offset1 0xec --offset2 0xf0 */
/* @PostProc: swap_instructions --offset1 0xf4 --offset2 0xf8 */
/* @PostProc: patch_insn_reg_field 0x50:rt:t9:t0 0x54:rs:t9:t0 0x5c:rt:t8:t9 0x60:rt:t0:t1 0x64:rs:t0:t1 0x64:rt:t0:t1 0x6c:rs:t0:t1 0x6c:rt:t1:t2 0x70:rt:t1:t2 0x74:rt:t2:t3 0x78:rs:t2:t3 0x78:rt:t2:t3 0x80:rs:t2:t3 0x80:rt:t3:t4 0x88:rt:t3:t4 0x8c:rt:t4:t5 0x90:rs:t4:t5 0x90:rt:t4:t5 0x98:rs:t4:t5 0x98:rt:t5:t6 0x9c:rt:t5:t6 0xa0:rt:t6:t7 0xa4:rs:t6:t7 0xa4:rt:t6:t7 0xac:rs:t6:t7 0xac:rt:t7:t8 0xb0:rt:t7:t8 0xb4:rs:t8:t9 0xb4:rt:t8:t9 0xbc:rs:t8:t9 0xbc:rt:t9:t0 0xc4:rt:t9:t0 0xec:rt:t0:t1 0xf0:rt:t1:t2 0xf4:rs:t1:t2 0xf4:rt:t1:t2 0xf8:rs:t0:t1 0xf8:rt:t0:t1 0x11c:rt:t1:t2 0x124:rt:t0:t1 0x150:rt:t2:t3 0x15c:rt:t2:t3 */
/* Function: date_girl6_mode5_scene_init (real entry 0x80156CF0 after cross-prologue hoist) */
/* Overlay: DATE */
#include "types.h"

extern s32 sub_sub_step_up();
extern s32 palette_upload_enqueue();
extern s32 get_g_zyotai_s();
extern s32 param_clamp();
extern s32 date_sprite_init();
extern s32 date_object_set();
extern s32 date_girl6_mode5_bufptr_init();

extern s32 date_ss_serifuev;
extern s32 date_ss_serifuev_kind;
extern s32 date_sss_soundev;
extern s16 girl6_tokimeki;
extern s16 girl6_yuukou;
extern s16 girl6_syoushin;
extern s32 date_scene_set8_res[];
extern u8 date_object_set_tbl0;
extern s32 date_serifuev_tbl[];
extern s32 date_serifuev_kind_tbl[];
extern s32 date_soundev_tbl[];

s32 date_girl6_mode5_scene_init(void)
{
    date_ss_serifuev = date_serifuev_tbl[53];
    date_ss_serifuev_kind = date_serifuev_kind_tbl[53];
    date_sss_soundev = date_soundev_tbl[53];
    if ((u32)(get_g_zyotai_s(6) & 0x7F) < 2U) {
        girl6_tokimeki = girl6_tokimeki + 3;
        girl6_yuukou = girl6_yuukou + 2;
    } else {
        girl6_tokimeki = girl6_tokimeki + 1;
        girl6_yuukou = girl6_yuukou + 3;
    }
    girl6_syoushin = girl6_syoushin - 20;
    param_clamp();
    date_girl6_mode5_bufptr_init();
    palette_upload_enqueue(date_scene_set8_res[4], 17, 1, 2, 0);
    date_sprite_init(date_scene_set8_res[5], date_scene_set8_res[6], date_scene_set8_res[7], date_scene_set8_res[8], date_scene_set8_res[9],
                  date_scene_set8_res[10]);
    date_object_set(date_scene_set8_res[1], date_scene_set8_res[2], date_scene_set8_res[0], (*(s16 *)&date_scene_set8_res[3]));
    date_object_set_tbl0 = 1;
    return sub_sub_step_up();
}
