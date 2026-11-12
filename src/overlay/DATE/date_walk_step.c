/* @SymbolStart: 0x80135AC4 */
/* @SymbolSize: 280 */
/* @PostProc: swap_branch_operands --offsets 0x70 */
/* Address: 0x80135AC4 (prologue 0x80135B14), Size: 280 bytes (70 insns) */
#include "types.h"

extern s16 date_open_step;
extern s16 date_open_sub;
extern s16 date_open_load_no;
extern s16 date_open_cnt;
extern s32 ss_serifu;
extern s32 ss_serifu_kind;
extern s32 sss_sound;
extern u8 now_year;
extern u8 now_month;
extern s32 ba_string;
extern u8 date_late_flag;
extern u8 date_final_flag;
extern u8 date_serifu_stride;
extern s32 date_walk_step_str0;
extern s32 date_serifu_tbl[];
extern s32 date_serifu_kind_tbl[];
extern s32 date_sound_tbl[];

extern void sub_sub_step_up(void);
extern void dec_bg_cache_step(s32);
extern void event_log_add(s32, s32);
extern void strcpy(s32, s32);

void date_walk_step(void)
{
    u32 step;
    u32 k98;

    date_open_step = 0;
    date_open_sub = 0;
    date_open_load_no = 0;
    date_open_cnt = 0;
    ss_serifu = date_serifu_tbl[20];
    ss_serifu_kind = date_serifu_kind_tbl[20];
    sss_sound = date_sound_tbl[20];
    step = now_year;
    k98 = 98;

    if (step == k98 || (step == 97 && (u32)now_month >= 3U)) {
        date_late_flag = 1;
    } else {
        date_late_flag = 0;
    }
    if (k98 == step || (step == 97 && now_month == 12)) {
        date_final_flag = 1;
    } else {
        date_final_flag = 0;
    }
    date_serifu_stride = 0;
    strcpy((s32)&ba_string, (s32)&date_walk_step_str0);
    dec_bg_cache_step(17195);
    event_log_add(1, 20);
    sub_sub_step_up();
}
