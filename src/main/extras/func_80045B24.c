/* Address: 0x80045B24 */
/* Size: 216 bytes (54 instructions) */
/* Compiler: GCC-like (original) - IDO 5.2 (ours) */
/* Region: GCC (0x80041000-0x80080FFF) */
#include "types.h"

extern s32 device_cmd_send(s32);

extern s8 device_init_flag_get_tbl0;
extern s8 device_load_start_tbl6;
extern s8 device_port_init_tbl0;

s32 func_80045B24(s32 arg0, s32 arg1)
{
    u32 *ptr;

    if (arg0 != 0) {
        ptr = (u32 *)0x8002E800;
    } else {
        ptr = (u32 *)0x8001C000;
    }

    if ((*ptr & 0xFFFF) == 0x40) {
        if (arg0 == 0) {
            device_cmd_send(0x22);
            device_cmd_send(0x2F);
            device_cmd_send((arg1 | 0x200) & 0xFFFF);
        } else {
            device_cmd_send(0x23);
            device_cmd_send(0x2E);
            device_cmd_send((arg1 | 0x200) & 0xFFFF);
        }

        device_load_start_tbl6 = (s8)arg0;
        device_port_init_tbl0 = (s8)arg1;
        device_init_flag_get_tbl0 = 1;
    }

    return -1;
}
