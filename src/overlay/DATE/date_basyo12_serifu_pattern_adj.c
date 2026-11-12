/* @SymbolStart: 0x80139DD0 */
/* @SymbolSize: 60 */
/* date_basyo12_serifu_pattern_adj - DATE overlay, Address: 0x80139DD0 (head) / 0x80139DE0 (prologue), */
/* Size: 60 bytes (15 insns). */
#include "types.h"

extern void sub_sub_step_up(void);
extern u8  date_pattern;
extern s16 now_serifu;

void date_basyo12_serifu_pattern_adj(void)
{
    now_serifu = now_serifu + date_pattern * 2;
    sub_sub_step_up();
}
