/* @SymbolSize: 108 */
/* Address: 0x80132198 */
/* Size: 108 bytes (27 instructions) */
#include "types.h"

extern void sub_sub_step_up(void);
extern void class_clear_n(s32);
extern void text_sys_init(void);
extern void k_disp_start(s32);
extern void hizuke_init(void);
extern void basyo_init(void *);
extern u8 etc_hizuke_basyo_init_str0;

void etc_hizuke_basyo_init(void)
{
    s32 s0;
    s32 s1;

    s0 = 96;
    s1 = 160;
    do {
        class_clear_n(s0);
        s0++;
    } while (s0 != s1);
    text_sys_init();
    basyo_init(&etc_hizuke_basyo_init_str0);
    hizuke_init();
    k_disp_start(1);
    sub_sub_step_up();
}
