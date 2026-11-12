/* Address: 0x80045324 */
/* Size: 220 bytes (55 words; the last is the alignment nop after `jr ra`) */
/* Region: Game (IDO 5.2 -O2, -Wc,-pic1) */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern void device_cmd_send(s32);
extern u8 device_init_flag_get_tbl0;
extern u8 device_load_start_tbl6;
extern u8 device_port_init_tbl0;

s32 device_port_init2(s32 a0, s32 a1)
{
    u32 *p;

    if (a0 != 0) {
        p = (u32 *)0x8002E800;
    } else {
        p = (u32 *)0x8001C000;
    }
    if ((*p & 0xFFFF) != 0x40) {
        return -1;
    }
    if (a0 == 0) {
        device_cmd_send(34);
        device_cmd_send(47);
        device_cmd_send((u16)(a1 | 0x200));
    } else {
        device_cmd_send(35);
        device_cmd_send(46);
        device_cmd_send((u16)(a1 | 0x200));
    }
    device_load_start_tbl6 = a0;
    device_port_init_tbl0 = a1;
    device_init_flag_get_tbl0 = 1;
}
