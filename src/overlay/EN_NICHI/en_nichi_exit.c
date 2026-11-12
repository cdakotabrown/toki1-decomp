/* @SymbolSize: 144 */
/* en_nichi_exit - EN_NICHI overlay, Address: 0x80132278, Size: 152 bytes (38 insns) */
#include "types.h"

extern void tpage_buf_clear();
extern void class_clear_f();
extern void cursor_init();
extern void back_clear_switch();
extern void goto_step();
extern void goto_sub_step();
extern void goto_sub_sub_step();

extern u8  back_clear_r;
extern u8  back_clear_g;
extern u8  back_clear_b;
extern s32 en_nichi_csr_speed_wk;
extern volatile u8  csr_speed;
extern volatile u8  ret_step;
extern u8  ret_sub_step;
extern u8  ret_sub_sub_step;

void en_nichi_exit(void)
{
    tpage_buf_clear();
    class_clear_f();
    cursor_init(0);
    back_clear_r = 0;
    back_clear_g = 0;
    back_clear_b = 0;
    back_clear_switch(1);
    csr_speed = (u8)en_nichi_csr_speed_wk;
    goto_step(ret_step);
    goto_sub_step(ret_sub_step);
    goto_sub_sub_step(ret_sub_sub_step);
}
