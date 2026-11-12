/* @SymbolStart: 0x80139FD0 */
/* @SymbolSize: 464 */
/* @PostProc: patch_insn_reg_field 0x30:rt:t9:t0 0x34:rt:t1:t2 0x38:rs:t9:t0 0x38:rt:t0:t1 0x3c:rt:t0:t1 0x48:rt:t1:t2 0xb0:rt:t2:t3 0xb4:rt:t3:t4 0xb8:rs:t3:t4 0xb8:rt:t3:t4 0xbc:rs:t2:t3 0xbc:rt:t2:t3 0xe0:rt:t3:t4 0xe8:rt:t2:t3 0x130:rt:t4:t5 0x134:rt:t5:t6 0x138:rs:t5:t6 0x138:rt:t5:t6 0x13c:rs:t4:t5 0x13c:rt:t4:t5 0x160:rt:t5:t6 0x168:rt:t4:t5 0x194:rt:t6:t7 0x198:rs:t6:t7 0x198:rt:t6:t7 0x1a0:rd:t7:t8 0x1a0:rs:t6:t7 0x1a8:rt:t7:t8 0x1b8:rt:t8:t9 0x1bc:rt:t8:t9 0x1c4:rt:t9:t0 0x1cc:rt:t9:t0 */
/* Address: 0x8013A004 (true start 0x80139FD0) */
#include "types.h"
#include "tokimeki/etc_cg_res.h"

extern void strcpy();
extern void shougatu_res_set(void);
extern void palette_upload_enqueue();
extern void date_sprite_init();
extern void date_object_set();
extern void sub_sub_step_up(void);

extern u8 now_girl;
extern u8 screen_w[];
extern s16 shougatu_date_now_serifu;
extern s16 shougatu_date_now_serifu_in;
extern u8 ba_string[];
extern u8 shougatu_date_sprite_init2_str0[];
extern u8 now_month;


extern u32 cal_cw_attr;
extern s16 cal_cw_anim;
extern s32 wink2_stop;
extern ETC_CG_RES shougatu_girl1_res;

void shougatu_date_sprite_init2(void)
{
    u8 *rec;
    s32 phase;

    rec = &screen_w[now_girl * 56];
    rec[456] = rec[456] | 4;

    shougatu_date_now_serifu = 13;
    shougatu_date_now_serifu_in = 0;
    strcpy(ba_string, shougatu_date_sprite_init2_str0);
    shougatu_res_set();

    phase = now_month;
    if ((u32)phase >= 6 && (u32)phase < 10) {
        palette_upload_enqueue(shougatu_girl1_res.pal[0], 17, 1, 2, 0);
        date_sprite_init(shougatu_girl1_res.tile[0][0], shougatu_girl1_res.tile[0][1], shougatu_girl1_res.tile[0][2], shougatu_girl1_res.tile[0][3],
                      shougatu_girl1_res.tile[0][4], shougatu_girl1_res.tile[0][5]);
        date_object_set(shougatu_girl1_res.img, shougatu_girl1_res.anim_tbl, shougatu_girl1_res.hdr, shougatu_girl1_res.aux);
    } else {
        palette_upload_enqueue(shougatu_girl1_res.pal[1], 17, 1, 2, 0);
        date_sprite_init(shougatu_girl1_res.tile[1][0], shougatu_girl1_res.tile[1][1], shougatu_girl1_res.tile[1][2], shougatu_girl1_res.tile[1][3],
                      shougatu_girl1_res.tile[1][4], shougatu_girl1_res.tile[1][5]);
        date_object_set(shougatu_girl1_res.img, shougatu_girl1_res.anim_tbl, shougatu_girl1_res.hdr, shougatu_girl1_res.aux);
    }

    cal_cw_attr = cal_cw_attr | 0x80000000;
    cal_cw_anim = 0;
    wink2_stop = 1;
    now_girl = 14;
    sub_sub_step_up();
}
