/* @SymbolStart: 0x80137E64 */
/* @SymbolSize: 440 */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:2 */
/* @PostProc: swap_instructions --offset1 0x1b0 --offset2 0x1b4 */
/* @PostProc: patch_insn_reg_field 0x12c:rt:v0:t3 0x130:rs:v0:t3 0x130:rt:v0:t3 0x134:rt:t4:t5 0x138:rd:t3:t4 0x138:rt:v0:t3 0x13c:rd:t4:t5 0x13c:rs:t4:t5 0x13c:rt:t3:t4 0x140:rs:t4:t5 0x140:rt:t4:t5 0x148:rs:t4:t5 0x150:rs:v0:t3 0x150:rt:t5:t6 0x158:rt:t5:t6 0x170:rt:t6:t7 0x174:rs:t6:t7 0x174:rt:t6:t7 0x17c:rs:t6:t7 0x180:rt:t7:t8 0x184:rs:t7:t8 0x184:rt:t7:t8 0x18c:rs:t7:t8 0x190:rt:t8:t9 0x194:rs:t8:t9 0x194:rt:t8:t9 0x19c:rs:t8:t9 */
/* Function: date_ending */
/* Overlay: DATE */
/* Address: 0x80137E64 (cross-prologue head) / 0x80137E6C (body) */
/* Size: 440 bytes (110 instructions) */
#include "types.h"

extern void dec_bg_cache_step(s32 a);
extern void event_log_add(s32 a, s32 b);
extern void date_basyo_set(void);
extern void date_trans_set(void);

extern s32 serifu_no;
extern s16 now_serifu;
extern u8 girl8_spot24_cnt;
extern u8 rec_prev_val[];
extern u8 rec_idx;
extern u8 now_girl;
extern u8 now_date_spot;
extern u8 date_late_flag;
extern u8 date_final_flag;
extern u8 date_ride_kind;
extern u8 date_ride_fix;

s32 date_ending(void)
{
    s32 mode;
    s32 sound;
    s32 idx;

    mode = serifu_no;
    if (mode == 0) {
        if (date_final_flag != 0 || date_late_flag != 0) {
            sound = 17241;
            if (date_ride_fix == 0) {
                date_ride_kind = 3;
            }
        } else {
            sound = 17214;
            if (date_ride_fix == 0) {
                date_ride_kind = 0;
            }
        }
    } else if (mode == 1) {
        now_serifu = now_serifu + 5;
        if (date_final_flag != 0) {
            sound = 17250;
            if (date_ride_fix == 0) {
                date_ride_kind = 4;
            }
        } else {
            sound = 17223;
            if (date_ride_fix == 0) {
                date_ride_kind = 1;
            }
        }
    } else {
        now_serifu = now_serifu + 10;
        sound = 17232;
        if (date_ride_fix == 0) {
            date_ride_kind = 2;
        }
    }
    date_ride_fix = 1;
    dec_bg_cache_step(sound);
    date_basyo_set();
    idx = rec_idx;
    if (rec_prev_val[idx * 8] == 1) {
        rec_idx = idx - 1;
    }
    event_log_add(0, now_date_spot);
    date_trans_set();
    if (now_girl == 8 && date_ride_kind == 3 && girl8_spot24_cnt != 0) {
        serifu_no = 1;
        return 1;
    }
    serifu_no = 0;
    return 0;
}
