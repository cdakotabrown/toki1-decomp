/* valen_girl9_bustup_read - VALEN overlay, Address: 0x801335E8, Size: 68 bytes (17 insns) */
/* @SymbolSize: 48 */
#include "types.h"

extern void bustup_cd_read(s32);
extern void date_name_set(s32);
extern void sub_sub_step_up(void);

void valen_girl9_bustup_read(void)
{
    date_name_set(9);
    bustup_cd_read(25808);
    sub_sub_step_up();
}
