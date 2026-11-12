/* Function: device_state_init */
/* Address: 0x80044A5C */
/* Size: 100 bytes (25 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern u8  device_load_start_tbl0;
extern u8  device_load_start_tbl1;
extern s32 device_load_start_tbl2;
extern s32 device_load_start_tbl3;
extern s32 device_load_start_tbl4;
extern s32 device_load_start_tbl5;
extern u8  device_load_start_tbl6;
extern u8  device_port_init_tbl0;
extern u8  device_load_poll_tbl2;
extern u8  device_load_start_tbl7;
u8 device_err[2] = { 0, 0 };

void device_state_init(void)
{
    device_err[0] = 0;
    device_err[1] = 0;
    device_load_start_tbl6 = 255;
    device_load_start_tbl2 = 0;
    device_load_start_tbl3 = 0;
    device_load_start_tbl4 = 0;
    device_load_start_tbl5 = 0;
    device_load_start_tbl0 = 0;
    device_load_start_tbl1 = 0;
    device_load_poll_tbl2 = 0;
    device_load_start_tbl7 = 0;
    device_port_init_tbl0 = 0;
}
