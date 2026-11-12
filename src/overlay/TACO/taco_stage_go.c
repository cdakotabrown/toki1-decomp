/* @SymbolStart: 0x80138614 */
/* @SymbolSize: 76 */
/* Address: 0x8013861C (true entry 0x80138614, 2 insns hoisted above .sym) */
#include "types.h"

extern u8 taco_load_no;

extern void goto_sub_step();
extern void device_port_init2();
extern void taco_actor_pool_clear();

void taco_stage_go(void)
{
    if (taco_load_no == 0) {
        taco_actor_pool_clear();
        taco_load_no = 40;
        device_port_init2(1, 513);
        goto_sub_step(22);
    }
}
