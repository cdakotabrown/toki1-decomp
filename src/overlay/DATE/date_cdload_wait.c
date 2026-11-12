/* @SymbolStart: 0x80139B38 */
/* @SymbolSize: 488 */
/* @PostProc: patch_insn_reg_field 0x8c:rt:t2:t3 0x90:rs:t2:t3 0x90:rt:t2:t3 0x94:rt:v0:t5 0x98:rd:t3:t4 0x98:rt:t2:t3 0x9c:rd:t3:t4 0x9c:rs:t3:t4 0x9c:rt:t2:t3 0xa0:rd:t3:t4 0xa0:rt:t3:t4 0xa4:rd:v0:t5 0xa4:rs:v0:t5 0xa4:rt:t3:t4 0xa8:rs:v0:t5 0xa8:rt:v0:t5 0xb0:rt:t4:t2 0xb4:rs:v0:t5 0xb8:rt:t4:t2 0xbc:rt:t5:t6 0xc0:rs:t5:t6 0xc0:rt:t5:t6 0xc8:rs:t5:t6 0xc8:rt:t6:t7 0xcc:rt:t6:t7 0xe8:rt:t7:t8 0xec:rs:t7:t8 0x120:rt:t8:t9 0x128:rt:t8:t9 0x130:rt:t9:t0 0x134:rt:t9:t0 0x13c:rt:t0:t1 0x140:rt:t0:t1 0x148:rt:t1:t2 0x14c:rt:t2:t3 0x150:rs:t2:t3 0x150:rt:t2:t3 0x154:rt:t1:t2 0x15c:rt:t3:t4 0x160:rs:t3:t4 0x160:rt:t3:t4 0x164:rt:t2:t3 0x16c:rt:t4:t5 0x170:rs:t4:t5 0x170:rt:t4:t5 0x174:rt:t3:t4 0x17c:rt:t5:t6 0x180:rs:t5:t6 0x180:rt:t5:t6 0x184:rt:t4:t5 0x18c:rt:t5:t6 0x1a4:rt:t6:t7 0x1a8:rt:t6:t7 0x1b0:rt:t7:t8 0x1b4:rt:t7:t8 0x1bc:rt:t8:t9 0x1c0:rt:t8:t9 0x1c8:rt:t9:t0 0x1cc:rt:t9:t0 0x1d4:rt:t0:t1 0x1d8:rt:t0:t1 */
/* Address: 0x80139B40 (true start 0x80139B38) */
#include "types.h"

extern s32 sub_sub_step_up();
extern s32 palette_upload_enqueue();
extern s32 get_CD_State2();
extern s32 cd_read_start2();
extern s32 class_clear_n();
extern s32 date_buf_tbl3_init();

extern s32 serifu_no;
extern s32 serifu_no_bak;
extern u8 date_aug_flag;
extern u8 now_sub_sub_step;
extern u32 step_counter;

extern volatile u8 day_flag;
extern volatile u8 now_girl;
extern u8 girl0_spot26_cnt[];
extern s32 date_pal_on;
extern s16 date_now_serifu_basyo12;
extern s32 date_aug_res_pal;

extern u8 cal_cw_prio;
extern u8 cal_cw_mode;
extern u8 cal_cw_disp;
extern u8 cal_cw_rate;
extern u8 cal_cw_bright;
extern s16 cal_cw_aux14;
extern s16 cal_cw_anim;
extern s16 cal_cw_frame;
extern s16 cal_cw_x;
extern s16 cal_cw_y;
extern u32 cal_cw_attr;
extern u8 cal_cw_clut;
extern s32 cal_cw_img;
extern s32 cal_cw_anim_tbl;
extern s32 cal_cw_hdr;

extern s32 date_aug_res;
extern s32 date_aug_res_img;
extern s32 date_aug_res_anim_tbl;
extern s16 date_aug_res_aux;
extern u8 date_aug_pal_ptn;

void date_cdload_wait(void)
{
    s32 t6;
    u32 v1;
    u32 first;
    u8 rec;

    t6 = serifu_no;

    if (t6 == 1) {
        serifu_no = serifu_no_bak;
        date_aug_flag = 0;
        sub_sub_step_up();
        now_sub_sub_step = now_sub_sub_step + 12;
        return;
    }

    v1 = step_counter;
    first = v1 < 1;
    v1 = v1 + 1;
    step_counter = v1;
    if (first != 0) {
        day_flag = day_flag | 0x10;
        rec = girl0_spot26_cnt[now_girl * 56];
        date_pal_on = 1;
        if (rec != 0) {
            date_now_serifu_basyo12 = date_now_serifu_basyo12 + 1;
        }
        cd_read_start2(29, 0x801B0000, 0xBD4F);
    }
    if ((get_CD_State2() & 1) == 0) {
        return;
    }
    date_buf_tbl3_init();

    palette_upload_enqueue(date_aug_res_pal, 17, 1, 3, 0);
    class_clear_n(96);

    cal_cw_prio = 12;
    cal_cw_mode = 64;
    cal_cw_attr = 0x41000000;
    cal_cw_disp = 4;
    cal_cw_img = date_aug_res_img;
    cal_cw_anim_tbl = date_aug_res_anim_tbl;
    cal_cw_hdr = date_aug_res;
    cal_cw_aux14 = date_aug_res_aux;
    cal_cw_anim = 0;
    cal_cw_frame = 0;
    cal_cw_rate = 1;
    cal_cw_clut = 17;
    cal_cw_x = -160;
    cal_cw_y = -120;
    cal_cw_bright = 128;
    date_aug_pal_ptn = 0;
    sub_sub_step_up();
}
