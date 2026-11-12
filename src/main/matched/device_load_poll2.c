/* Function: device_load_poll2 */
/* Size: 188 bytes (47 instructions) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: patch_insn_reg_field --patch 0x34:rt:v0:v1 --patch 0x3c:rs:v0:v1 --patch 0x40:rt:v0:v1 --patch 0x40:rs:v0:v1 --patch 0x64:rt:v0:v1 --patch 0x64:rs:v0:v1 --patch 0x68:rt:v1:t1 --patch 0x68:rs:v0:v1 --patch 0x6c:rt:v1:t1 --patch 0x70:rs:v1:t1 */
#include "types.h"

extern s32 get_reads(void);
extern void device_load_cmd_send(void);
extern s32 device_ready_check(s32 arg0);

extern u8 device_load_poll_tbl0;
extern u8 device_load_poll_tbl1;
extern u8 device_load_poll_tbl2;
extern u8 device_err[];

s32 device_load_poll2(void) {
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

    device_err[device_load_poll_tbl1] = 0;
    return 1;
done:
    return -1;
}
