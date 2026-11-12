/* @CompileFlags: -Wc,-pic0 */
/* @RodataBase: 0x8013BE04 */
/* Address: 0x801359F0, Size: 772 bytes (193 instructions) */
#include "types.h"

typedef struct { s32 w[11]; } Blk44;
extern Blk44 master_girl_select16_tbl;

extern u8  now_sub_sub_step;
extern u8  denwa_cnt;
extern u8  girl0_basyo;
extern u8  girl4_basyo;
extern s32 girl4_flag;
extern u8  girl11_tel_cnt;
extern u8  master_talk_girl_no;

extern s32 girl_head_tbl[];
extern s32 girl_body_tbl[];
extern s32 girl_seq_tbl[];

extern s16 now_serifu;
extern s16 now_serifu_in;
extern s32 ss_serifu;
extern s32 ss_serifu_kind;
extern s32 sss_sound;

extern s32 master_serifu_ptr[];

extern void sub_sub_step_up(void);
extern s32  get_top_girl(void);
extern s32  get_g_zyotai_low(s32);
extern void bustup_cd_read(s32);
extern void goto_sub_sub_step(s32);
extern s32  get_CD_State2(void);
extern void device_load_start(s32, s32, s32, s32, s32, s32);
extern s32  device_load_poll(void);
extern void cd_read_start2(s32, void *, s32);
extern void master_serifu_set3(void);
extern void sub_step_up(void);
extern void game_reset(void);
extern void common_disp(void);

void master_girl_select16(void) {
    s32 pad1[2];
    s32 val;
    Blk44 buf;
    u32 state;
    s32 idx;
    s32 pad0[2];

    (void)&pad0;
    (void)&pad1;
    buf = master_girl_select16_tbl;
    state = now_sub_sub_step;
    switch (state) {
    case 0:
        sub_sub_step_up();
        val = get_top_girl();
        if (denwa_cnt >= 65U) {
            master_talk_girl_no = 12;
            bustup_cd_read(22523);
        } else if ((u32)get_g_zyotai_low(val) < 2) {
            master_talk_girl_no = (u8)val;
            if (((u32)girl0_basyo >> 4) == 6) {
                if ((u32)get_g_zyotai_low(0) < 2) {
                    master_talk_girl_no = 0;
                }
            }
            if (((u32)girl4_basyo >> 4) == 6) {
                if ((u32)get_g_zyotai_low(4) < 2) {
                    if (((u32)(girl4_flag << 30) >> 31) == 1) {
                        master_talk_girl_no = 4;
                    }
                }
            }
            bustup_cd_read(buf.w[master_talk_girl_no]);
        } else if (girl11_tel_cnt >= 2U) {
            master_talk_girl_no = 11;
            bustup_cd_read(27473);
        } else {
            master_talk_girl_no = 255;
            goto_sub_sub_step(4);
        }
        break;
    case 1:
        if (get_CD_State2() & 1) {
            idx = master_talk_girl_no;
            device_load_start(0, 0xC051, 0xC02B,
                          girl_head_tbl[idx], girl_body_tbl[idx], girl_seq_tbl[idx]);
            sub_sub_step_up();
        }
        break;
    case 2:
        if (device_load_poll() == 1) {
            cd_read_start2(11, (void *)0x801E8000, 0xA9C5);
            sub_sub_step_up();
        }
        break;
    case 3:
        if (get_CD_State2() & 1) {
            master_serifu_set3();
            ss_serifu = master_serifu_ptr[0];
            ss_serifu_kind = master_serifu_ptr[1];
            sss_sound = master_serifu_ptr[2];
            now_serifu = 0;
            now_serifu_in = 0;
            sub_step_up();
        }
        break;
    case 4:
        sub_step_up();
        sub_step_up();
        break;
    default:
        game_reset();
        break;
    }
    common_disp();
}
