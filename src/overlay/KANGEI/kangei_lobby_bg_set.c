/* @SymbolSize: 72 */
/* kangei_lobby_bg_set - KANGEI overlay, Address: 0x80135B18, Size: 72 bytes (18 insns) */
#include "types.h"

extern u8 ba_string;
extern u8 kangei_lobby_bg_set_str0;
extern void dec_bg_cache_step(s32);
extern void strcpy(void *, void *);
extern void event_log_add(s32, s32);
extern void sub_sub_step_up(void);

void kangei_lobby_bg_set(void)
{
    dec_bg_cache_step(17065);
    strcpy(&ba_string, &kangei_lobby_bg_set_str0);
    event_log_add(14, 13);
    sub_sub_step_up();
}
