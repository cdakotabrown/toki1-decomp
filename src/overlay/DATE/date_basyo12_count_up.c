/* @SymbolStart: 0x80139D20 */
/* @SymbolSize: 88 */
/* date_basyo12_count_up - DATE overlay, Address: 0x80139D20 (head) / 0x80139D40 (prologue), */
/* Size: 88 bytes (22 insns). */
#include "types.h"

extern void sub_sub_step_up(void);
extern u8  now_girl;
extern u8  screen_w[];
extern s16 date_now_serifu_basyo12;

void date_basyo12_count_up(void)
{
    if (screen_w[now_girl * 56 + 490] == 0) {
        date_now_serifu_basyo12 = date_now_serifu_basyo12 + 1;
    }
    sub_sub_step_up();
}
