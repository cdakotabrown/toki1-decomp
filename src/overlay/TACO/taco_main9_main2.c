/* @SymbolStart: 0x801383EC */
/* @SymbolSize: 68 */
/* Address: 0x801383F4 */
#include "types.h"

extern u8 taco_load_no;

extern void goto_sub_step();
extern void device_port_init2();

void taco_main9_main2(void)
{
    u8 arg0 = taco_load_no;

    if (arg0 != 0) {
        return;
    }
    taco_load_no = 24;
    device_port_init2(1, 512);
    goto_sub_step(12);
}
