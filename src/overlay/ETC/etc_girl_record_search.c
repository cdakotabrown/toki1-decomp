/* @SymbolSize: 344 */
/* Address: 0x8013EEEC */
/* Size: 344 bytes (86 instructions) */
#include "types.h"

extern void etc_hizuke_set(void);

extern u8 now_girl;
extern s32 etc_rec_girl;
extern s32 etc_rec_year;
extern s32 etc_rec_month;
extern s32 etc_rec_day;
extern s32 etc_record_index;
extern s32 etc_event_step;

void etc_girl_record_search(void)
{
top:
    etc_hizuke_set();
    if (etc_rec_year != 0) {
        goto check;
    }
    if (etc_rec_month != 0) {
        goto check;
    }
    if (etc_rec_day != 0) {
        goto check;
    }
step:
    if (etc_record_index < 255) {
        etc_record_index = etc_record_index + 1;
    }
    etc_hizuke_set();
    if (etc_rec_year == 0) {
        goto settle;
    }
    if (etc_rec_month == 0) {
        goto settle;
    }
    if (etc_rec_day != 0) {
        goto check;
    }
settle:
    if (etc_record_index == 255) {
        etc_event_step = -1;
        etc_record_index = etc_record_index;
        goto check;
    }
    etc_record_index = etc_record_index;
    goto step;
check:
    if (etc_rec_girl == now_girl) {
        return;
    }
    if (etc_event_step == -1) {
        etc_record_index = etc_record_index;
        return;
    }
    if (etc_record_index < 255) {
        etc_record_index = etc_record_index + 1;
        goto top;
    }
    etc_record_index = etc_record_index;
    if (etc_record_index != 255) {
        goto top;
    }
    etc_event_step = -1;
}
