/* @SymbolSize: 112 */
/* Address: 0x8013CC6C */
/* Size: 104 bytes (26 instructions) */
#include "types.h"

extern void sub_step_up(void);
extern void sub_sub_step_up(void);
extern s32 etc_rec_month;
extern s32 etc_rec_day;
extern s32 etc_record_index;
extern s32 etc_rec_year;

void etc_sub_sub_step6(void)
{
    s32 t6 = etc_rec_year;

    if (t6 != 0) {
        goto check_t9;
    }
    if (etc_rec_month != 0) {
        goto check_t9;
    }
    if (etc_rec_day == 0) {
        goto call_429B8;
    }
check_t9:
    if (etc_record_index != 255) {
        goto call_429FC;
    }
call_429B8:
    sub_step_up();
    return;
call_429FC:
    sub_sub_step_up();
}
