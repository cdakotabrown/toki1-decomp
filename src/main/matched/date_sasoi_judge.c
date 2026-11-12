/* Address: 0x80070910 */
/* Size: 644 bytes (161 instructions) */
/* Region: Game (0x80041000-0x80086000) */
/* Compiler: IDO 5.2 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: patch_stack_complete --old 152 --new 160 */
/* @PostProc: remap_stack_slots --map '136:124,104:92,152:160' */
#include "types.h"

extern s8 date_sasoi_judge_tbl0;
extern u8 screen_w[];
extern u8 now_year;
extern u8 now_month;
extern u8 now_day;
extern u16 basyo_pack;
extern s32 reserved_date;
extern u32 album_work;

extern void get_grp(u8 *out, s16 *clr);
extern s32 get_g_zyotai_s(s32 idx);
extern s32 rand(void);

s32 date_sasoi_judge(u8 *sel)
{
    u8 out[16];
    s16 clr[16];
    s32 i;
    s32 j;
    s32 idx;
    u8 *p;

    *sel = 0;

    if ((u32)(reserved_date << 2) >> 31 == 1) {
        return 0;
    }
    if (((u32)now_day + 6) / 7 == 2) {
        if ((u32)basyo_pack >> 12 < 11) {
            return 0;
        }
    }
    if (now_month == 12) {
        if (now_day == 17) {
            return 0;
        }
    }
    if (now_month == 12) {
        if (now_day == 24) {
            return 0;
        }
    }
    if (now_year == 98) {
        if (now_month >= 2U) {
            return 0;
        }
    }

    idx = album_work % 11;
    get_grp(out, clr);

    for (i = 0, j = idx; i < 11; i++) {
        idx = j % 11;
        p = &screen_w[idx * 56];
        if ((u32)(*(s32 *)(p + 456) << 30) >> 31 != 0) {
            if (*(s16 *)(p + 454) < 60) {
                if (get_g_zyotai_s(idx) != 4) {
                    if ((u32)out[idx] >= 33) {
                        if (rand() % 100 < 8) {
                            if (date_sasoi_judge_tbl0 != idx) {
                                *sel = idx;
                                date_sasoi_judge_tbl0 = idx;
                                return 1;
                            }
                        }
                    }
                }
            }
        }
        j++;
    }
    return 0;
}
