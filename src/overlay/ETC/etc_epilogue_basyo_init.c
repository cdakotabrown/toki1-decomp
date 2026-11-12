/* @SymbolSize: 60 */
/* Address: 0x80132840 */
/* Size: 64 bytes (16 instructions) */
#include "types.h"

extern u8 etc_basyo_init4_str0;

extern void sub_sub_step_up();
extern void text_sys_init();
extern void k_disp_start();
extern void basyo_init();

void etc_epilogue_basyo_init(void)
{
    text_sys_init();
    basyo_init(&etc_basyo_init4_str0);
    k_disp_start(1);
    sub_sub_step_up();
}
