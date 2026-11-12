/* Address: 0x80044F84 (thunk at 0x80044F84, body at 0x80044F8C) */
/* Size: 224 bytes (56 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: swap_registers v1:v0 --only 0x14,0x18,0xa4,0xa8 */
#include "types.h"

extern u8 device_load_poll_tbl1;
extern u8 device_load_start_tbl0;
extern u8 device_load_start_tbl1;
extern void change_trans_pointer(u32);
extern void device_cmd_send(s32);

s32 device_load_cmd_send(void) {
    u8 b;
    u8 c;

    if (device_load_poll_tbl1 == 0) {
        b = device_load_start_tbl0;
        if (b == 0) {
            c = device_load_start_tbl1;
            if (c == 0) {
                change_trans_pointer(0x801B8000);
                device_cmd_send(17);
                return 1;
            }
        }
        if (b == 0) {
            change_trans_pointer(0x801B8000);
            device_cmd_send(19);
            return 1;
        }
        c = device_load_start_tbl1;
        if (c == 0) {
            change_trans_pointer(0x801C8000);
            device_cmd_send(20);
            return 1;
        }
        return 0;
    }

    b = device_load_start_tbl0;
    if (b == 0) {
        c = device_load_start_tbl1;
        if (c == 0) {
            change_trans_pointer(0x801B8000);
            device_cmd_send(18);
            return 1;
        }
    }
    if (b == 0) {
        change_trans_pointer(0x801E0000);
        device_cmd_send(22);
        return 1;
    }
    c = device_load_start_tbl1;
    if (c == 0) {
        change_trans_pointer(0x801B8000);
        device_cmd_send(21);
        return 1;
    }
    return 0;
}
