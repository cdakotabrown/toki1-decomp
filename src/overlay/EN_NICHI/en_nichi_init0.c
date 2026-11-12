/* en_nichi_init0 - EN_NICHI overlay, Address: 0x801321EC, Size: 140 bytes (35 insns) */
#include "types.h"

extern void date_x_taku_set();
extern void device_cmd_send();
extern void cursor_init();
extern void set_flag_b3_high_bit();
extern void srand();
extern void hizuke_disp_switch();
extern void mes_window_switch();
extern void message_disp_switch();
extern void mascot_disp_switch();
extern void sub_sub_step_up();

extern u8 csr_speed;
extern s32 en_nichi_csr_speed_wk;
extern s32 vblnk_counter;

void en_nichi_init0(void)
{
    date_x_taku_set(1, 0);
    device_cmd_send(127);
    cursor_init(0);
    set_flag_b3_high_bit(0);
    srand(vblnk_counter);
    hizuke_disp_switch(0);
    mes_window_switch(0);
    message_disp_switch(0);
    mascot_disp_switch(0);
    sub_sub_step_up();
    en_nichi_csr_speed_wk = csr_speed;
    csr_speed = 1;
}
