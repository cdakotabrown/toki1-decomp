/* ending_girl12_name_set - ENDING overlay, Address: 0x801339D0, Size: 64 bytes (16 instructions) */
#include "types.h"

extern void strcpy(void *, char *);
extern void sub_sub_step_up(void);

extern u8 girl_no;
extern u8 g_name[];
extern char ending_girl12_name_set_str0[];

void ending_girl12_name_set(void)
{
    girl_no = 12;
    strcpy(g_name, ending_girl12_name_set_str0);
    sub_sub_step_up();
}
