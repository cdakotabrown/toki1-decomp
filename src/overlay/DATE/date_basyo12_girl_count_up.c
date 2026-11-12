/* @SymbolStart: 0x80139D78 */
/* @SymbolSize: 88 */
/* date_basyo12_girl_count_up - DATE overlay, Address: 0x80139D78 (head) / 0x80139D9C (prologue), */
/* Size: 88 bytes (22 insns). */
#include "types.h"

extern void sub_sub_step_up(void);
extern u8 now_girl;
extern u8 screen_w[];
extern u8 now_sub_sub_step;

void date_basyo12_girl_count_up(void)
{
    u8 *rec = &screen_w[now_girl * 56];

    rec[490] = rec[490] + 1;
    sub_sub_step_up();
    now_sub_sub_step = now_sub_sub_step + 7;
}
