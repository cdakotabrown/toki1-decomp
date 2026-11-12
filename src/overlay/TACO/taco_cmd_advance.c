/* @SymbolSize: 48 */
/* Address: 0x80137F38 */
#include "types.h"

extern void goto_sub_step();
extern void device_cmd_send();
extern void taco_xa_sector_set();

void taco_cmd_advance(void)
{
    taco_xa_sector_set(10);
    device_cmd_send(177);
    goto_sub_step(2);
}
