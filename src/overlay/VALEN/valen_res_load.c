/* @SymbolSize: 60 */
/* valen_res_load - VALEN overlay, Address: 0x8013253C, Size: 68 bytes (17 insns) */
#include "types.h"

extern void cd_read_start2(s32, s32, s32);
extern void valen_res_init(void);
extern void sub_sub_step_up(void);

void valen_res_load(void)
{
    cd_read_start2(7, (s32)0x80197000, 0xAFAC);
    valen_res_init();
    sub_sub_step_up();
}
