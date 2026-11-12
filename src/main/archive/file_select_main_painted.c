/* Function: file_select_main */
/* Address: 0x80070910 */
/* Size: 644 bytes (161 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: patch_stack_complete --old 176 --new 160 */
/* @PostProc: custom_script custom_func_80070910.py */
#include "common.h"

extern s32 reserved_date;
extern u8 now_day;
extern u8 now_month;
extern u8 now_year;
extern u16 basyo_pack;
extern u32 album_work;
extern s8 date_sasoi_judge_tbl0;
extern u32 screen_w;
extern void get_grp(s8 *a0, s16 *a1);
extern s32 get_g_zyotai_s(s32 a0);
extern s32 rand(void);

s32 file_select_main(s8 *arg0) {
    s8 sp7C[36];
    s16 sp5C[16];
    s32 counter;
    s32 curIdx;

    *arg0 = 0;

    if (((u32)(reserved_date << 2) >> 31) == 1) {
        return 0;
    }

    if ((u32)(now_day + 6) / 7u == 2) {
        if ((u32)basyo_pack >> 12 < 11) {
            return 0;
        }
    }

    if (now_month == 12 && now_day == 17) {
        return 0;
    }
    if (now_month == 12 && now_day == 24) {
        return 0;
    }
    if (now_year == 98 && (u32)now_month < 2) {
        return 0;
    }

    curIdx = album_work % 11u;
    get_grp(sp7C, sp5C);

    counter = 0;
    do {
        s32 charIdx;
        u32 *charData;

        charIdx = curIdx % 11;
        charData = (u32 *)((u8 *)&screen_w + charIdx * 56);

        if ((charData[114] << 30) >> 31) {
            if ((s32)(*(s16 *)((u8 *)charData + 454)) < 60) {
                if (get_g_zyotai_s(charIdx) != 4) {
                    if ((u32)(u8)sp7C[charIdx] >= 33u) {
                        s32 randVal = rand() % 100;
                        if (randVal < 8 && date_sasoi_judge_tbl0 != charIdx) {
                            counter++;
                            *arg0 = (s8)charIdx;
                            date_sasoi_judge_tbl0 = (s8)charIdx;
                            return 1;
                        }
                    }
                }
            }
        }
        counter++;
        curIdx++;
    } while (counter != 11);

    return 0;
}
