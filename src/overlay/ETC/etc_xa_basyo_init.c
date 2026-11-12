/* Address: 0x801322E0 */
/* Size: 100 bytes (25 instructions) */
#include "types.h"

extern u8 etc_xa_basyo_init_str0;

extern void sub_sub_step_up();
extern void device_cmd_send();
extern void load_header_check();
extern void set_XAsector();
extern void text_sys_init();
extern void k_disp_start();
extern void basyo_init();

void etc_xa_basyo_init(void)
{
    load_header_check(0);
    set_XAsector(0x03000656, 0x0B000E0C, 15);
    device_cmd_send(768);
    text_sys_init();
    basyo_init(&etc_xa_basyo_init_str0);
    k_disp_start(1);
    sub_sub_step_up();
}
