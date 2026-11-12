/* @PostProc: custom_script custom_func_8006EAE4.py */
#include "common.h"

extern s32 screen_w[];
extern u8 now_year;
extern u8 now_month;
extern u8 now_day;
extern s32 D_800EEBA0;
extern u8 ev_flag4c;
extern u8 ev_flag4e;
extern s32 reserved_date;
extern void get_grp(s8 *a0, s16 *a1);
extern s32 rand(void);

s32 file_select_main21(void) {
    s8 sp4C[12];
    s16 sp2C[16];
    s32 pad[4];
    s32 count;
    s32 *p;
    s32 i;

    (void)&pad;
    get_grp(sp4C, sp2C);

    count = 0;
    p = screen_w;
    i = 0;
    do {
        if ((s32)(p[114] << 30) < 0) {
            if (sp2C[i] >= 501) {
                count++;
            }
        }
        i++;
        p = (s32 *)((u8 *)p + 56);
    } while (i != 11);

    if (now_year != 98) goto fail;
    if (now_month != 2) goto fail;
    if (now_day != 15) goto fail;
    if (count < 2) goto fail;
    if (((u32)(D_800EEBA0 << 20) >> 29) < 4) goto fail;
    if (ev_flag4c == 0) goto fail;
    if (ev_flag4e == 0) goto fail;
    if ((s32)(reserved_date << 2) < 0) goto fail;
    if ((rand() & 0xF) >= 5) goto fail;
    return 1;
fail:
    return 0;
}
