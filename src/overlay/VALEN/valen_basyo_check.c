/* valen_basyo_check - VALEN overlay, Address: 0x801334B8, Size: 88 bytes (22 insns) */
#include "types.h"

extern s32 strcmp(s32 *, s32 *);
extern void sub_sub_step_up(void);
extern void strcpy(s32 *, s32 *);

extern s32 ba_string;
extern s32 ba_string2;

void valen_basyo_check(void)
{
    if (strcmp(&ba_string, &ba_string2) == 0) {
        sub_sub_step_up();
    }
    strcpy(&ba_string, &ba_string2);
    sub_sub_step_up();
}
