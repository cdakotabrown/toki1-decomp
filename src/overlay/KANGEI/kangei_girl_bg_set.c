/* kangei_girl_bg_set - KANGEI overlay, Address: 0x801352BC, Size: 104 bytes (26 insns) */
#include "types.h"

extern void dec_bg_cache_step(s32);
extern void strcpy(void *, void *);
extern void sub_sub_step_up(void);

extern u8 ba_string;
extern u8 kangei_girl_bg_set_str0;
extern u8 kangei_girl_bg_set_str1;
extern u8 now_girl;

void
kangei_girl_bg_set(void)
{
    u32 hiddenT6 = now_girl;

    if (hiddenT6 < 4U) {
        dec_bg_cache_step(17065);
        strcpy(&ba_string, &kangei_girl_bg_set_str0);
    } else {
        dec_bg_cache_step(17046);
        strcpy(&ba_string, &kangei_girl_bg_set_str1);
    }
    sub_sub_step_up();
}
