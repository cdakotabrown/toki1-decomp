/* valen_cel_read0 - VALEN overlay, Address: 0x80133EE8, Size: 112 bytes (28 insns) */
/* @SymbolSize: 56 */
#include "types.h"

extern void cd_read_start2(s32, s32, s32);
extern void valen_cel_init(void);
extern void sub_sub_step_up(void);

void valen_cel_read0(void)
{
    cd_read_start2(3, (s32)0x801B0000, 0xAF43);
    valen_cel_init();
    sub_sub_step_up();
}
