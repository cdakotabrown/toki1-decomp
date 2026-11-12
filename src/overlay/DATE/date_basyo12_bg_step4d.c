/* @SymbolSize: 88 */
/* Function: date_basyo12_bg_step4d */
/* Overlay: DATE */
#include "types.h"

extern u8 date_basyo12_cmd_flag;
extern u8 ba_string2;
extern u8 date_basyo12_bg_step4d_str0;

extern void sub_sub_step_up(void);
extern void device_cmd_send(s32);
extern void dec_bg_cache_step(s32);
extern void strcpy(s32, s32);

void date_basyo12_bg_step4d(void)
{
    dec_bg_cache_step(17195);

    if (date_basyo12_cmd_flag != 0) {
        device_cmd_send(512);
    }

    strcpy((s32)&ba_string2, (s32)&date_basyo12_bg_step4d_str0);
    sub_sub_step_up();
}
