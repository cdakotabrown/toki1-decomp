/* Address: 0x800450BC */
/* Size: 264 bytes (66 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: swap_registers_liveness --address 0x800450BC */
/* @PostProc: patch_insn_reg_field --patch 0x34:rt:v0:v1 --patch 0x3c:rs:v0:v1 --patch 0x40:rt:v0:v1 --patch 0x40:rs:v0:v1 --patch 0x64:rt:v0:v1 --patch 0x64:rs:v0:v1 --patch 0x68:rs:v0:v1 */
#include "types.h"

extern s32 get_reads(void);
extern void device_load_cmd_send(void);
extern s32 device_ready_check(s32 arg0);
extern s32 device_cmd_send(s32 a0);

extern u8 device_load_poll_tbl0;
extern u8 device_load_poll_tbl1;
extern u8 device_load_poll_tbl2;
extern u8 device_err[];

s32 device_load_poll(void) {
    s32 val;
    u8 counter;

    val = get_reads();

    if (((val - device_load_poll_tbl0 + 256) & 0xFF) < 5u) {
        goto done;
    }

    counter = device_load_poll_tbl2;
    if (counter == 0) {
        device_load_cmd_send();
        device_load_poll_tbl2 = device_load_poll_tbl2 + 1;
        goto done;
    }

    counter = (u8)(counter + 1);
    device_load_poll_tbl2 = counter;

    if (counter < 193u) {
        if (device_ready_check(device_load_poll_tbl1) == 0) {
            goto done;
        }
    }

    if (device_load_poll_tbl1 == 0) {
        device_cmd_send(0x22);
        device_cmd_send(0x21);
        device_cmd_send(0x2F);
    } else {
        device_cmd_send(0x23);
        device_cmd_send(0x24);
        device_cmd_send(0x2E);
    }

    device_err[device_load_poll_tbl1] = 0;

    return 1;
done:
    return -1;
}
