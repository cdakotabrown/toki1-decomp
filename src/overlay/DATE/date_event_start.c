/* @SymbolStart: 0x801326AC */
/* @SymbolSize: 124 */
/* Function: date_event_start */
/* Overlay: DATE */
/* Address: 0x801326BC */
/* Size: 108 bytes (27 instructions) */
#include "types.h"

extern void goto_step(s32 a);

extern s32 work_tarao;
extern u8 ev_flag00[];
extern u8 now_event_no;
extern u8 date_event_flag;
extern s32 date_disp_flag;

void date_event_start(s16 slot, s16 repeat, u8 idx)
{
    work_tarao = slot;
    if (repeat != 0) {
        ev_flag00[idx] = 1;
    }
    now_event_no = idx;
    date_event_flag = 1;
    date_disp_flag = date_disp_flag | 2;
    goto_step(94);
}
