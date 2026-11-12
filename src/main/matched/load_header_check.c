/* Address: 0x800449A4 - 0x80044A3C (152 bytes) */
/* Region: GCC (OR encoding) */
/* Compiler: IDO 5.2 -O2 -G0 -mips1 -EL -non_shared */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern s32 device_port_init(u8 arg0, u8 arg1);
extern s32 device_cmd_send(s32 a0);

extern u8 device_init_flag_get_tbl0;
extern u8 device_load_start_tbl6;
extern u8 device_port_init_tbl0;

s32 load_header_check(s32 a0) {
    s32 a2;
    u32 *ptr;

    a2 = a0;

    if (a0 != 0) {
        ptr = (u32 *)0x8002E800;
    } else {
        ptr = (u32 *)0x8001C000;
    }

    if ((*ptr & 0xFFFF) != 64) {
        return -1;
    }

    if (a2 != 0) {
        device_port_init(device_load_start_tbl6, device_port_init_tbl0);
    } else {
        device_cmd_send(0x1200);
    }

    device_init_flag_get_tbl0 = (u8)a2;
    return a2 & 0xFF;
}
