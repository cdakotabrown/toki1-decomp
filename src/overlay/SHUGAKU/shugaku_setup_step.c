#include "types.h"

extern u8  now_girl;
extern u8  basho_num;
extern u8  now_sub_sub_step;
extern u8  fade_bri;
extern s16 shugaku_pbirth_now_serifu;
extern s16 shugaku_pbirth_now_serifu_in;

extern void date_name_set(s32 a0);
extern void shugaku_tbl_init(void);
extern void shugaku_girl_ptr_set(void);
extern void sub_sub_step_up(void);

void shugaku_setup_step(void)
{
    u8 sidx;
    u8 cur;
    u8 flag;
    u8 f80;
    u8 t8;

    sidx = now_girl;
    date_name_set((s32)sidx);
    shugaku_tbl_init();
    shugaku_girl_ptr_set();

    flag = basho_num;
    if (flag != 2) {
        f80 = fade_bri;
        if (f80 != 0) {
            basho_num = 1;
        } else {
            basho_num = 0;
        }
    }

    shugaku_pbirth_now_serifu = 0;
    shugaku_pbirth_now_serifu_in = 0;
    sub_sub_step_up();

    flag = basho_num;
    if (flag == 2) {
        cur = now_sub_sub_step;
        now_sub_sub_step = (u8)(cur + 2);
    }
}
