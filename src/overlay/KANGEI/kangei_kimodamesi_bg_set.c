/* @SymbolSize: 72 */
/* kangei_kimodamesi_bg_set - KANGEI overlay, Address: 0x80135324, Size: 72 bytes (18 insns) */
#include "types.h"

extern u8 ba_string2;
extern u8 kangei_kimodamesi_bg_set_str0;
extern void dec_bg_cache_step(s32);
extern void event_log_add(s32, s32);
extern void strcpy(void *, void *);
extern void sub_sub_step_up(void);

void kangei_kimodamesi_bg_set(void)
{
    dec_bg_cache_step(17094);
    event_log_add(14, 11);
    strcpy(&ba_string2, &kangei_kimodamesi_bg_set_str0);
    sub_sub_step_up();
}
