/* @CompileFlags: -Wo,-loopunroll,0 */
/* @PostProc: swap_registers a0:v1 */
/* @PostProc: patch_insn_reg_field 0x34:rt:v1:a0 0x40:rs:v1:a0 0x40:rt:v1:a0 0x54:rs:v1:a0 0x68:rs:v1:a0 0x7c:rs:v1:a0 0x90:rs:v1:a0 0x94:rs:v1:a0 0x94:rt:v1:a0 0xa4:rs:v1:a0 */
/* Address: 0x80140DCC, Size: 184 bytes (46 instructions) */
#include "types.h"

extern u8 ev_flag00;
extern u8 ev_flag01;
extern u8 ev_flag02;
extern u8 D_800EE763[];
extern u8 ev_flag_scan_end[];

s32 etc_flags_count_check(void)
{
    s32 count;
    u8 *p;

    count = 0;
    if (ev_flag00 != 0) {
        count++;
    }
    if (ev_flag01 != 0) {
        count++;
    }
    if (ev_flag02 != 0) {
        count++;
    }
    p = D_800EE763;
    do {
        if (p[1388] != 0) {
            count++;
        }
        if (p[1389] != 0) {
            count++;
        }
        if (p[1390] != 0) {
            count++;
        }
        if (p[1391] != 0) {
            count++;
        }
        p += 4;
    } while (p != ev_flag_scan_end);
    if (count >= 30) {
        return 1;
    }
    return 0;
}
