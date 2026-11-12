/* @SymbolSize: 60 */
/* kangei_nozoki_bg_set - KANGEI overlay, Address: 0x80136BC0, Size: 60 bytes (15 insns) */
#include "types.h"

extern u8 ba_string;
extern u8 kangei_nozoki_bg_set_str0;
extern void dec_bg_cache_step(s32);
extern void sub_sub_step_up(void);
extern void strcpy(void *, void *);

void kangei_nozoki_bg_set(void)
{
    dec_bg_cache_step(17084);
    sub_sub_step_up();
    strcpy(&ba_string, &kangei_nozoki_bg_set_str0);
}
