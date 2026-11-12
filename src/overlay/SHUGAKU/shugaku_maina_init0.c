#include "types.h"

extern u8 ba_string;
extern u8 shugaku_maina_init0_str0;

extern s32 shugaku_ss_serifuev;
extern s32 shugaku_ss_serifuev_kind;
extern s32 shugaku_sss_soundev;
extern s16 with_yoshi;
extern s16 move_in;
extern u8  fade_bri;
extern u8  shugaku_use_sprite_fade;
extern s32 shugaku_serifuev_tbl[];
extern s32 shugaku_serifuev_kind_tbl[];
extern s32 shugaku_soundev_tbl[];

extern void strcpy(s32 a0, s32 a1);
extern void shugaku_bank_ptr_init2(void);
extern void sub_sub_step_up(void);

void shugaku_maina_init0(void)
{
    u8 flag;

    strcpy((s32)&ba_string, (s32)&shugaku_maina_init0_str0);
    shugaku_bank_ptr_init2();
    shugaku_ss_serifuev = shugaku_serifuev_tbl[66];
    shugaku_ss_serifuev_kind = shugaku_serifuev_kind_tbl[66];
    shugaku_sss_soundev = shugaku_soundev_tbl[66];
    with_yoshi = 0;
    move_in = 0;

    flag = fade_bri;
    if (flag != 0) {
        shugaku_use_sprite_fade = 1;
    } else {
        shugaku_use_sprite_fade = 0;
    }
    sub_sub_step_up();
}
