/* @SymbolStart: 0x8013871C */
/* @SymbolSize: 132 */
/* Address: 0x8013873C (true entry 0x8013871C, 8 insns hoisted above .sym) */
/* Size: 132 bytes (33 insns) */
#include "types.h"

extern u8 back_clear_r;
extern u8 back_clear_g;
extern u8 back_clear_b;
extern s32 taco_view_flag;
extern u8 taco_load_no;
extern u8 taco_load_state;

extern void sub_sub_step_up(void);
extern void device_port_init2(s32 a0, s32 a1);
extern void taco_actor_reset(void);
extern void taco_sprite_init(void);
extern void taco_actor_pool_clear(void);

void taco_init(void)
{
    back_clear_r = 8;
    back_clear_g = 0x10;
    back_clear_b = 0x60;
    taco_view_flag = 4;
    taco_actor_pool_clear();
    taco_actor_reset();
    taco_sprite_init();
    taco_load_no = 52;
    taco_load_state = 0;
    device_port_init2(0, 512);
    sub_sub_step_up();
}
