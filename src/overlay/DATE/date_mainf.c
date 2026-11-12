/* @SymbolSize: 52 */
/* Function: date_mainf */
/* Overlay: DATE */
/* Address: 0x80133E44 */
/* Size: 52 bytes (13 instructions) */
#include "types.h"

extern u8 ba_string;
extern s32 k_reset();
extern s32 basyo_init();
extern s32 date_event_sched();

void date_mainf(void)
{
    k_reset(0);
    basyo_init(&ba_string);
    date_event_sched();
}
