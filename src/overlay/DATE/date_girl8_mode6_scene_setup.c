/* @SymbolStart: 0x8014D788 */
/* @SymbolSize: 344 */
/* @CompileEnv: IDO52_CONST_INREG_VALUES=8 */
/* @PostProc: patch_insn_reg_field 0xcc:rt:v0:t1 0xd0:rs:v0:t1 0xd0:rt:v0:t1 0xd4:rt:v1:v0 0xdc:rt:v1:v0 0xe4:rs:v0:t1 0xe4:rt:t1:t2 0xe8:rt:t1:t2 0xec:rt:t2:t3 0xf0:rs:t2:t3 0xf0:rt:t2:t3 0xf8:rs:t2:t3 0xf8:rt:t3:t4 0xfc:rt:t3:t4 0x108:rt:v1:v0 0x10c:rt:t4:t5 0x114:rt:t4:t5 0x11c:rt:t5:t6 0x120:rt:t5:t6 0x128:rt:t6:t7 0x12c:rt:t6:t7 0x134:rt:t7:t8 0x138:rt:t7:t8 0x140:rt:t8:t9 0x144:rt:t8:t9 0x14c:rt:t9:t0 0x154:rt:t9:t0 */
#include "types.h"
#include "tokimeki/class_w.h"

extern void sub_sub_step_up(void);
extern void palette_upload_enqueue(s32, s32, s32, s32, s32);
extern void param_clamp(void);
extern void date_sprite_init(s32, s32, s32, s32, s32, s32);
extern void date_object_set(s32, s32, s32, s32);
extern void date_girl8_mode6_res_ptr_set(void);

extern s32 date_ss_serifuev;
extern s32 date_ss_serifuev_kind;
extern s32 date_sss_soundev;

extern s32 date_scene_setup5_res[];

extern volatile s16 girl8_yuukou;
extern volatile s16 girl8_syoushin;

extern u8 cal_cw_flag4;

extern s16 kao2[];
extern s16 ans[];
extern CLASS_W class_w[];
extern s32 date_serifuev_tbl[];
extern s32 date_serifuev_kind_tbl[];
extern s32 date_soundev_tbl[];

void date_girl8_mode6_scene_setup(void)
{
    s16 t;

    date_ss_serifuev = date_serifuev_tbl[5];
    date_ss_serifuev_kind = date_serifuev_kind_tbl[5];
    date_sss_soundev = date_soundev_tbl[5];
    date_girl8_mode6_res_ptr_set();
    palette_upload_enqueue(date_scene_setup5_res[4], 17, 1, 2, 0);
    date_sprite_init(date_scene_setup5_res[5], date_scene_setup5_res[6], date_scene_setup5_res[7],
                  date_scene_setup5_res[8], date_scene_setup5_res[9], date_scene_setup5_res[10]);
    date_object_set(date_scene_setup5_res[1], date_scene_setup5_res[2], date_scene_setup5_res[0], (*(s16 *)&date_scene_setup5_res[3]));
    t = girl8_yuukou;
    cal_cw_flag4 = 8;
    girl8_yuukou = t + 2;
    girl8_syoushin = girl8_syoushin - 20;
    class_w[97].cel_mode = 8;
    param_clamp();
    kao2[0] = 2;
    kao2[1] = 2;
    kao2[2] = 2;
    ans[0] = 1;
    ans[1] = 1;
    ans[2] = 1;
    sub_sub_step_up();
}
