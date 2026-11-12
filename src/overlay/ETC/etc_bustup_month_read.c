/* @SymbolStart: 0x8013BD40 */
/* @SymbolSize: 172 */
/* @RodataBase: 0x8014C9D4 */
/* etc_bustup_month_read -- ETC overlay, Address: 0x8013BD48, Size: 164 bytes (41 insns). */
#include "types.h"

extern s32 etc_rec_month;

extern void bustup_cd_read(s32);

extern s32 etc_bustup_month_tbl[];
extern s32 etc_bustup_month_tbl_1_0[];
extern s32 etc_bustup_flag;

void etc_bustup_month_read(s32 idx, s32 arg2)
{
    s32 v;

    switch (etc_rec_month - 1) {
    case 5:
    case 6:
    case 7:
    case 8:
        v = etc_bustup_month_tbl[idx];
        if (v != -1) {
            etc_bustup_flag = 1;
            bustup_cd_read(v);
        }
        break;
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 9:
    case 10:
    case 11:
        v = etc_bustup_month_tbl_1_0[idx];
        if (v != -1) {
            etc_bustup_flag = 1;
            bustup_cd_read(v);
        }
        break;
    }
}
