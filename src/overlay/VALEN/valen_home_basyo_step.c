/* @SymbolSize: 92 */
/* valen_home_basyo_step - VALEN overlay, Address: 0x8013399C, Size: 92 bytes (23 insns) */
#include "types.h"

extern s32 ba_string2;
extern s32 valen_home_basyo_step_str0;
extern s32 valen_home_basyo_step_str1;
extern s32 strcmp(s32 *, s32 *);
extern void sub_sub_step_up(void);
extern void strcpy(s32 *, s32 *);
extern void basyo_string2_step(void);

void valen_home_basyo_step(void)
{
    if (strcmp(&ba_string2, &valen_home_basyo_step_str0) == 0) {
        sub_sub_step_up();
        return;
    }
    strcpy(&ba_string2, &valen_home_basyo_step_str1);
    basyo_string2_step();
}
