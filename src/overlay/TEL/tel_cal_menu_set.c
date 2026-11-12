/* tel_cal_menu_set - TEL overlay, Address: 0x80136458, Size: 456 bytes (114 insns) */
/* @PostProc: patch_stack_complete --old 264 --new 272 */
/* @PostProc: remap_stack_slots --from 232 --to 236 */
/* @PostProc: remap_stack_slots --from 200 --to 204 */
/* @PostProc: remap_stack_slots --from 168 --to 172 */
/* @PostProc: remap_stack_slots --from 136 --to 140 */
/* @PostProc: remap_stack_slots --from 124 --to 132 */
/* @PostProc: remap_stack_slots --from 120 --to 136 */
#include "types.h"

extern u8 now_year;
extern u8 now_month;
extern u8 now_day;
extern u8 cal_day_kind[];
extern s8 menu_csr;
extern s32 tel_cal_day_tbl[];
extern s32 tel_cal_month_tbl[];

extern s32 reserved_date_check(u8, s32, s32);
extern s32 get_calx_pos(s32, s32);
extern s32 get_caly_pos(s32, s32);
extern void menu_set(s32, s32, s16 *, s16 *, s16 *, s16 *);

s32 tel_cal_menu_set(void)
{
    s16 aX[16];
    s16 aY[16];
    s16 aW[16];
    s16 aH[16];
    s32 s0;
    s32 s6;
    s32 t0;
    s32 a3;
    s32 s7;
    s32 row;
    u8 val;

    s0 = now_day;
    t0 = 0;
    s6 = 0;
    a3 = 0;

    do {
        s0 = s0 + 1;
        if ((u32)cal_day_kind[0] < (u32)s0) {
            s0 = 1;
            s6 = 1;
        }
        val = cal_day_kind[(s6 << 6) + s0];
        if (val != 1) {
            if (val != 2) {
                goto next;
            }
        }
        if (reserved_date_check(now_year, now_month + s6, s0) != 0) {
            goto next;
        }
        s7 = s0 - 1;
        aX[t0] = (s16)(get_calx_pos(s6, s7) - 16);
        aY[t0] = (s16)(get_caly_pos(s6, s7) - 16);
        aW[t0] = 16;
        aH[t0] = 16;
        tel_cal_day_tbl[t0] = s0;
        row = now_month;
        tel_cal_month_tbl[t0] = s6 + row;
        t0++;
    next:
        a3++;
    } while (a3 != 28);

    menu_set(1, t0 & 0xFF, aX, aY, aW, aH);
    menu_csr = 0;
    return t0;
}
