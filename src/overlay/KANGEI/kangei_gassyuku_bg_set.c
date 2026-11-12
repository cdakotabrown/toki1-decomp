/* kangei_gassyuku_bg_set - KANGEI overlay, Address: 0x80134094, Size: 164 bytes (41 insns) */
/* @SymbolSize: 152 */
#include "types.h"

extern void dec_bg_reset(void);
extern void ClearImage(s16 *, s32, s32, s32);
extern void dec_bg_cache_step(s32);
extern void sub_sub_step_up(void);
extern void strcpy(void *, void *);

extern u8 ba_string;
extern u8 kangei_gassyuku_bg_set_str0;

void
kangei_gassyuku_bg_set(void)
{
    s16 rect[4];

    dec_bg_reset();
    rect[0] = 320;
    rect[1] = 256;
    rect[2] = 320;
    rect[3] = 248;
    ClearImage(rect, 8, 0, 0);
    rect[0] = 704;
    rect[1] = 0;
    ClearImage(rect, 8, 0, 0);
    dec_bg_cache_step(17037);
    sub_sub_step_up();
    strcpy((void *)&ba_string, (void *)&kangei_gassyuku_bg_set_str0);
}
