/* @SymbolSize: 60 */
/* shugaku_lobby_bg_set - SHUGAKU overlay, Address: 0x80139024, Size: 60 bytes (15 insns) */
#include "types.h"

extern void dec_bg_cache_step(s32 a);
extern void strcpy(s32 a, s32 b);
extern void sub_sub_step_up(void);

extern u8 ba_string;
extern u8 shugaku_lobby_bg_set_str0;

void shugaku_lobby_bg_set(void)
{
    dec_bg_cache_step(16933);
    strcpy((s32)&ba_string, (s32)&shugaku_lobby_bg_set_str0);
    sub_sub_step_up();
}
