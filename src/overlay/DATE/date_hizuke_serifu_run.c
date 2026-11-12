/* @SymbolStart: 0x8013AEBC */
/* @SymbolSize: 600 */
/* @PostProc: swap_instructions --offset1 0x78 --offset2 0x7c */
/* @PostProc: patch_insn_reg_field 0x10:rt:v0:t6 0x14:rs:v0:t6 0x14:rt:v0:t6 0x20:rs:v0:t6 0x20:rt:v1:v0 0x24:rs:v0:t6 0x24:rt:t6:t7 0x2c:rs:v1:v0 0x30:rt:t6:t7 0x44:rt:a0:v1 0x48:rs:a0:v1 0x48:rt:a0:v1 0x50:rs:a0:v1 0x54:rt:t7:a0 0x58:rs:t7:a0 0x58:rt:t7:a0 0x60:rt:t7:a0 0x70:rt:a0:v1 0x74:rt:v1:a0 0x78:rs:v1:a0 0x78:rt:v1:a0 0x7c:rs:a0:v1 0x7c:rt:a0:v1 0x80:rs:v0:a1 0x80:rt:a1:v0 0x8c:rs:a0:v1 0x94:rs:v1:a2 0x94:rt:a2:a0 0xa8:rs:v1:a0 0xb0:rs:v0:a1 0xb0:rt:a1:v0 0xb8:rs:a0:a3 0xb8:rt:a3:v1 0xd0:rs:v1:a2 0xd0:rt:a2:a0 0xd8:rs:v0:a1 0xd8:rt:a1:v0 0xe0:rs:a0:v1 0xf8:rs:v1:a0 0x100:rs:v0:a1 0x100:rt:a1:v0 0x108:rs:a0:v1 0x120:rs:v0:a1 0x120:rt:a1:v0 0x128:rs:a0:a3 0x128:rt:a3:v1 0x130:rs:v1:a2 0x130:rt:a2:a0 0x164:rs:v0:a1 0x164:rt:a1:v0 0x16c:rs:a0:v1 0x174:rs:v1:a2 0x174:rt:a2:a0 0x1b4:rs:a0:a3 0x1b4:rt:a3:v1 */
/* Address: 0x8013AEC4 (true start 0x8013AEBC) */
#include "types.h"

extern s32 sub_sub_step_up();
extern s32 serifu_main();

extern u8 step_mode;
extern u8 now_year;
extern u8 now_month;
extern u8 now_day;
extern u32 frame_counter;
extern s16 date_now_serifu_hizuke;
extern s16 date_now_serifu_hizuke_in;
extern s16 *now_s;
extern s16 *now_s_in;
extern s32 ss_s;
extern s32 ss_s_kind;
extern s32 sss_so;
extern u8 serifu_no;
extern s32 date_serifu_tbl[];
extern s32 date_serifu_kind_tbl[];
extern s32 date_sound_tbl[];

s32 date_hizuke_serifu_run(void)
{
    s32 step;
    u32 first;
    s32 month;
    s32 day;
    s32 year;

    step = step_mode;
    first = (u32)step < 1;
    step_mode = step + 1;

    if (first != 0) {
        month = now_month;
        if (month == 3 && now_day == 21 && now_year != 96) {
            date_now_serifu_hizuke = 0;
        } else {
            year = now_year;
            day = now_day;
            if (month == 3 && day == 20 && year == 96) {
                date_now_serifu_hizuke = 0;
            } else if (year == 95 && month == 7 && day == 23) {
                date_now_serifu_hizuke = 1;
            } else if (year == 96 && month == 7 && day == 28) {
                date_now_serifu_hizuke = 1;
            } else if (year == 97 && month == 7 && day == 27) {
                date_now_serifu_hizuke = 1;
            } else if (month == 9 && day == 23 && year != 96) {
                if ((frame_counter & 1) != 0) {
                    date_now_serifu_hizuke = 2;
                } else {
                    date_now_serifu_hizuke = 3;
                }
            } else if (month == 9 && day == 22 && year == 96) {
                if ((frame_counter & 1) != 0) {
                    date_now_serifu_hizuke = 2;
                } else {
                    date_now_serifu_hizuke = 3;
                }
            } else if (month == 12 && day == 23) {
                date_now_serifu_hizuke = 4;
            } else {
                sub_sub_step_up();
                return 0;
            }
        }
    }

    if ((u32)step_mode >= 11) {
        step_mode = 10;
    }
    now_s = &date_now_serifu_hizuke;
    now_s_in = &date_now_serifu_hizuke_in;
    ss_s = date_serifu_tbl[24];
    ss_s_kind = date_serifu_kind_tbl[24];
    sss_so = date_sound_tbl[24];
    return serifu_main(serifu_no, 1, 0);
}
