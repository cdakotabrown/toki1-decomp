/* @SymbolStart: 0x8013B98C */
/* @SymbolSize: 176 */
/* Address: 0x8013B9A0 */
/* Size: 156 bytes (39 instructions) */
#include "types.h"

extern void bustup_cd_read(s32);
extern s32 etc_bustup_flag;
extern s32 etc_rec_year;
extern s32 etc_bustup_year_tbl[];
extern s32 etc_bustup_year_tbl_8;

void etc_bustup_year_read(s32 idx)
{
    s32 *p;
    s32 v;

    p = &etc_bustup_year_tbl[idx];
    v = *p;
    if (v == -1) {
        return;
    }
    etc_bustup_flag = 1;
    if (p == &etc_bustup_year_tbl_8) {
        switch (etc_rec_year) {
        case 95:
            bustup_cd_read(20723);
            break;
        case 96:
            bustup_cd_read(20768);
            break;
        case 97:
        default:
            bustup_cd_read(20813);
            break;
        }
    } else {
        bustup_cd_read(v);
    }
}
