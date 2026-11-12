/* @CompileEnv: IDO52_CONST_INREG_VALUES=1:8 */
/* @SymbolStart: 0x80139DD4 */
/* @SymbolSize: 180 */
/* - `@CompileEnv: IDO52_CONST_INREG_VALUES=1:8` (Hook 16, type-scoped) */
#include "types.h"

extern void shared_system_state_reset(void);
extern void bustup_ptr_init(void);
extern void yazirushi_off(void);
extern void date_name_set(u8);
extern void shougatu_ptab_init(void);
extern void sub_sub_step_up(void);

extern u8 vblank_flag;
extern s32 vb_ready;
extern u8 screen_bri;
extern u8 fade_bri;
extern s32 fade_flag;
extern s32 bustup_on;
extern u8 bg_change_tbl2;
extern u8 now_girl;
extern s32 shougatu_date_ss_serifu;
extern s32 shougatu_date_ss_serifu_kind;
extern s32 shougatu_date_sss_sound;
extern s32 shougatu_date_serifu_tbl[];
extern s32 shougatu_date_serifu_kind_tbl[];
extern s32 shougatu_date_sound_tbl[];

void shougatu_girl1_init(void)
{
    vblank_flag = 0;
    vb_ready = 1;
    shared_system_state_reset();
    screen_bri = 128;
    fade_bri = 0;
    fade_flag = 0;
    bustup_on = 0;
    bg_change_tbl2 = 0;
    bustup_ptr_init();
    yazirushi_off();
    now_girl = 1;
    date_name_set(1);
    shougatu_ptab_init();
    shougatu_date_ss_serifu = shougatu_date_serifu_tbl[4];
    shougatu_date_ss_serifu_kind = shougatu_date_serifu_kind_tbl[4];
    shougatu_date_sss_sound = shougatu_date_sound_tbl[4];
    sub_sub_step_up();
}
