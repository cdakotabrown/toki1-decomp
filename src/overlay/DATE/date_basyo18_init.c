/* date_basyo18_init - DATE overlay, Address: 0x80136B3C, Size: 196 bytes (49 insns) */
/* @SymbolStart: 0x80136B04 */
/* @SymbolSize: 252 */
/* @PostProc: patch_insn_reg_field 0xa4:rt:t2:t3 0xac:rt:t2:t3 0xcc:rt:t3:t4 0xd0:rt:t3:t4 */
#include "types.h"

extern void strcpy(s32 *, s32 *);
extern s32 get_g_zyotai_s(s32);
extern void dec_bg_cache_step(s32);
extern void event_log_add(s32, s32);
extern void sub_sub_step_up(void);
extern u8  day_flag;
extern s32 ss_serifu, ss_serifu_kind, sss_sound;
extern s32 ba_string, date_basyo18_init_str0, ba_string2, date_basyo18_init_str1;
extern u8  date_serifu_stride;
extern u8  now_girl;
extern s16 ans[];
extern s16 kao[];
extern u8  date_pattern;
extern s32 now_date_spot, en_nichi_res;
extern s32 date_serifu_tbl[];
extern s32 date_serifu_kind_tbl[];
extern s32 date_sound_tbl[];

void date_basyo18_init(void)
{
    s32 v0;

    day_flag = day_flag | 0x10;
    ss_serifu = date_serifu_tbl[4];
    ss_serifu_kind = date_serifu_kind_tbl[4];
    sss_sound = date_sound_tbl[4];
    strcpy(&ba_string, &date_basyo18_init_str0);
    strcpy(&ba_string2, &date_basyo18_init_str1);
    date_serifu_stride = 5;
    v0 = now_girl;
    kao[0] = 0;
    v0 = get_g_zyotai_s(v0);
    kao[1] = (s16)(v0 & 0x7F);
    ans[0] = 0;
    ans[1] = 0;
    date_pattern = 0;
    now_date_spot = 34;
    en_nichi_res = 0;
    dec_bg_cache_step(17103);
    event_log_add(1, (s32)*(u8 *)&now_date_spot);
    sub_sub_step_up();
}
