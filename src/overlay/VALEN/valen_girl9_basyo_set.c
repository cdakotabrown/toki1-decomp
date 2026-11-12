/* valen_girl9_basyo_set - VALEN overlay, Address: 0x80133568, Size: 80 bytes (20 insns) */
#include "types.h"

extern s32 ba_string2;
extern s32 ba_string;
extern s32 valen_girl9_basyo_set_str0;
extern s32 valen_girl9_basyo_set_str1;
extern void strcpy(s32 *, s32 *);
extern void dec_bg_cache_step(s32);
extern void sub_sub_step_up(void);

void valen_girl9_basyo_set(void)
{
    strcpy(&ba_string2, &valen_girl9_basyo_set_str0);
    strcpy(&ba_string, &valen_girl9_basyo_set_str1);
    dec_bg_cache_step(16506);
    sub_sub_step_up();
}
