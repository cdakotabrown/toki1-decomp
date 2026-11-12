/* @SymbolSize: 148 */
#include "types.h"

extern u8 back_clear_r;
extern u8 back_clear_g;
extern u8 back_clear_b;
extern s32 taco_view_flag;
extern u8 taco_load_no;
extern u8 taco_load_state;

extern void device_port_init2(s32 a0, s32 a1);
extern void device_cmd_send(s32 a0);
extern void taco_light_init(void);
extern void taco_actor_reset(void);
extern void taco_sprite_init(void);
extern void taco_actor_pool_clear(void);
extern void sub_sub_step_up(void);

void taco_mainb_init(void)
{
    device_port_init2(0, 515);
    device_cmd_send(36);
    taco_light_init();
    taco_actor_reset();
    taco_sprite_init();
    back_clear_r = 120;
    back_clear_g = 168;
    back_clear_b = 240;
    taco_view_flag = 3;
    taco_load_no = 36;
    taco_load_state = 0;
    taco_actor_pool_clear();
    sub_sub_step_up();
}
