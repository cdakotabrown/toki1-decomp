/* Address: 0x8004523C - 0x80045324 (232 bytes) */
/* Region: Game (0x80041000-0x80085FFF) */
/* Compiler: IDO 5.2 -O2 -G0 -mips1 -EL -non_shared */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: swap_registers_liveness --address 0x8004523C */
/* @PostProc: swap_registers_liveness --address 0x8004523C */
/* @PostProc: swap_registers_liveness --address 0x8004523C */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern s32 device_cmd_send(s32 a0);

extern u8 device_init_flag_get_tbl0;
extern u8 device_load_start_tbl6;
extern u8 device_port_init_tbl0;

s32 device_port_init(s32 arg0, s32 arg1) {
    u32 *base;
    u32 val;

    if (arg0 != 0) {
        base = (u32 *)0x8002E800;
    } else {
        base = (u32 *)0x8001C000;
    }

    val = *base;
    if ((val & 0xFFFF) != 64) {
        return -1;
    }

    if (arg0 == 0) {
        device_cmd_send(0x22);
        device_cmd_send(0x21);
        device_cmd_send(0x2F);
        device_cmd_send((arg1 | 0x200) & 0xFFFF);
    } else {
        device_cmd_send(0x23);
        device_cmd_send(0x24);
        device_cmd_send(0x2E);
        device_cmd_send((arg1 | 0x200) & 0xFFFF);
    }

    device_load_start_tbl6 = (u8)arg0;
    device_port_init_tbl0 = (u8)arg1;
    device_init_flag_get_tbl0 = 1;
}
