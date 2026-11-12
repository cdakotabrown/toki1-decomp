/* date_serifu_reset - DATE overlay, Address: 0x801324D8 */
/* Size: 468 bytes (117 instructions) */
/* @SymbolSize: 468 */
/* @PostProc: swap_instructions --offset1 0xa0 --offset2 0xa4 */
/* @PostProc: patch_insn_reg_field 0xa4:rt:a0:t6 0xa0:rt:v1:a0 0xa8:rs:v1:a0 0xa8:rt:v1:a0 0xac:rs:a0:t6 0xac:rt:t6:t7 0xb0:rs:a0:t6 0xb0:rt:t7:t8 0xb4:rs:t7:t8 0xb4:rt:t7:t8 0xb8:rs:t6:t7 0xb8:rt:t6:t7 0xbc:rs:v1:a0 0xbc:rt:t9:t0 0xc0:rs:t9:t0 0xc0:rt:t0:t1 0xc4:rd:t8:t9 0xc4:rs:t6:t7 0xc4:rt:t7:t8 0xc8:rs:t8:t9 0xc8:rt:t0:t1 0xd0:rs:v1:a0 0xd0:rt:t3:t5 0xd4:rs:t3:t5 0xd4:rt:t3:t5 0xd8:rd:t5:t6 0xd8:rt:t3:t5 0xdc:rd:t5:t6 0xdc:rs:t5:t6 0xdc:rt:t3:t5 0xe4:rt:t7:t8 0xe8:rs:t7:t8 0xe8:rt:t7:t8 0xec:rt:t9:t0 0xf0:rd:t1:t2 0xfc:rs:v1:a0 0x100:rd:t2:t4 0x104:rd:t4:t3 0x104:rs:t1:t2 0x104:rt:t2:t4 0x108:rd:t6:t7 0x108:rs:t4:t3 0x108:rt:t5:t6 0x10c:rt:t6:t7 0x184:rt:t7:t8 0x18c:rs:t9:t0 0x18c:rt:t9:t0 0x190:rt:t8:t9 0x194:rs:t9:t0 0x1a4:rt:t8:t9 */
#include "types.h"

extern void sub_sub_step_up(void);
extern void tpage_buf_clear(void);
extern s32  get_g_zyotai_s(s32);
extern void bustup_ptr_init(void);

extern u8  now_basyo;
extern u8  date_pattern;
extern s16 now_serifu;
extern s16 now_serifu_in;
extern s16 now_serifuev;
extern s16 now_serifuev_in;
extern s16 now_serifue_in;
extern s16 sound_seek_time;
extern u8  now_day;
extern u8  now_youbi;
extern u8  now_girl;
extern s32 date_pal_on;
extern u8  date_machi_mode;
extern u8  date_serifu_stride;
extern u8  date_basyo12_cmd_flag;
extern u8  date_first_sun_flag;
extern u8  date_ride_fix;
extern s16 date_now_serifu_machi;
extern s16 date_now_serifu_machi_in;
extern s16 date_now_serifu_hizuke;
extern s16 date_now_serifu_hizuke_in;
extern s16 date_now_serifu_zyotai_in;
extern s16 date_now_serifu_basyo0;
extern s16 date_now_serifu_basyo0_in;
extern s16 date_now_serifu_basyo14_in;
extern s16 date_now_serifu_basyo15_in;
extern s16 date_now_serifu_basyo12;
extern s16 date_now_serifu_basyo12_in;

void date_serifu_reset(void)
{
    s32 v0;
    s32 x;
    s32 k;

    tpage_buf_clear();

    if (now_basyo == 11 && date_pattern == 2) {
        now_serifu = now_serifu + date_serifu_stride;
    } else {
        now_serifu = now_serifu + date_pattern * date_serifu_stride;
    }

    now_serifu_in = 0;
    v0 = get_g_zyotai_s(now_girl);

    k = date_machi_mode;
    x = v0 & 0x7F;
    date_now_serifu_machi = ((x >= 2U) + (x >= 3U)) * ((k == 0) + 1) + k * 6U
               - (k == 2) * 3;

    date_now_serifu_machi_in = 0;
    now_serifue_in = 0;
    date_now_serifu_zyotai_in = 0;
    date_now_serifu_hizuke = 0;
    date_now_serifu_hizuke_in = 0;
    date_now_serifu_basyo12 = 0;
    date_now_serifu_basyo12_in = 0;
    now_serifuev = 0;
    now_serifuev_in = 0;
    date_now_serifu_basyo14_in = 0;
    date_now_serifu_basyo15_in = 0;
    date_now_serifu_basyo0 = 0;
    date_now_serifu_basyo0_in = 0;
    date_ride_fix = 0;
    date_pal_on = 0;

    if (now_youbi == 6U && now_day < 8U) {
        date_first_sun_flag = 1;
    } else {
        date_first_sun_flag = 0;
    }

    tpage_buf_clear();
    bustup_ptr_init();

    sound_seek_time = 0;
    date_basyo12_cmd_flag = 0;
    sub_sub_step_up();
}
