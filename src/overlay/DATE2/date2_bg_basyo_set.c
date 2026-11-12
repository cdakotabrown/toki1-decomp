/* @SymbolStart: 0x80133488 */
/* @SymbolSize: 232 */
/* Address: 0x80133490, Size: 224 bytes (56 insns) */
#include "types.h"

extern u8 date2_mode_no;

extern u8 date2_mode_no;

extern u8 now_year;
extern char ba_string2[];
extern char date2_bg_basyo_set_str0[];
extern char date2_bg_basyo_set_str1[];
extern char date2_bg_basyo_set_str2[];
extern char date2_bg_basyo_set_str3[];

void dec_bg_cache_step(s32 arg0);
void strcpy(void *arg0, void *arg1);
void sub_sub_step_up(void);

void date2_bg_basyo_set(void)
{
    u8 v0;
    u8 t6;

    v0 = date2_mode_no;
    switch (v0) {
    case 0:
        dec_bg_cache_step(17232);
        strcpy(ba_string2, date2_bg_basyo_set_str0);
        break;
    case 1:
        t6 = now_year;
        if ((u32)t6 >= 97U) {
            dec_bg_cache_step(17241);
            strcpy(ba_string2, date2_bg_basyo_set_str1);
        } else {
            dec_bg_cache_step(17214);
            strcpy(ba_string2, date2_bg_basyo_set_str2);
        }
        break;
    case 2:
        dec_bg_cache_step(17223);
        strcpy(ba_string2, date2_bg_basyo_set_str3);
        break;
    }
    sub_sub_step_up();
}
