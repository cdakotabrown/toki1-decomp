/* Function: date_basyo12_ba_string_set */
/* Overlay: DATE */
/* Address: 0x80139E0C */
/* Size: 60 bytes (15 instructions) */
#include "types.h"

extern u8 ba_string2;
extern u8 date_basyo12_ba_string_set_str0;
extern s32 strcpy();
extern s32 sub_sub_step_up();

s32 date_basyo12_ba_string_set(void)
{
    strcpy(&ba_string2, &date_basyo12_ba_string_set_str0);
    return sub_sub_step_up();
}
