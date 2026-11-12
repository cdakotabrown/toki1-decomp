/* @SymbolStart: 0x80138820 */
/* @SymbolSize: 72 */
/* Address: 0x80138828 (true entry 0x80138820, 2 insns hoisted above .sym) */
#include "types.h"

extern u8 taco_load_no;

extern void goto_sub_step();
extern void device_port_init2();
extern void taco_actor_pool_clear();

void taco_stage_go2(void)
{
    if (taco_load_no == 0) {
        taco_load_no = 56;
        taco_actor_pool_clear();
        device_port_init2(1, 512);
        goto_sub_step(32);
    }
}
