/* Address: 0x8013F2B0 */
/* Size: 68 bytes (17 instructions) */
#include "types.h"

extern s32 shougatu_menu_sub_res[];

void shougatu_bank_init2b(void)
{
    shougatu_menu_sub_res[0] = 0x801DDBB4;
    shougatu_menu_sub_res[1] = 0x801DDBB8;
    shougatu_menu_sub_res[2] = 0x801DDC70;
    shougatu_menu_sub_res[4] = 0x801DB000;
}
