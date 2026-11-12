/* @SymbolSize: 64 */
/* ending_girl14_name_set - ENDING overlay, Address: 0x80133A10, Size: 64 bytes (16 insns) */
#include "types.h"

extern void strcpy(s32, s32);
extern void sub_sub_step_up(void);
extern u8 girl_no;
extern u8 ending_girl14_name_set_str0;
extern u8 g_name[];

void ending_girl14_name_set(void)
{
    girl_no = 14;
    strcpy((s32)g_name, (s32)&ending_girl14_name_set_str0);
    sub_sub_step_up();
}
