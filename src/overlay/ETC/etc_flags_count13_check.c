/* @CompileFlags: -Wo,-loopunroll,0 */
/* @PostProc: patch_insn_reg_field 0x1c:rd:t7:t8 0x20:rs:t7:t8 0x24:rt:v1:a0 0x2c:rt:a0:v1 0x38:rt:t8:t9 0x40:rd:t9:t1 0x40:rt:t8:t9 0x44:rs:t9:t1 0x50:rt:t0:t2 0x58:rd:t1:t4 0x58:rt:t0:t2 0x5c:rs:t1:t4 0x68:rt:t2:t5 0x70:rd:t3:t7 0x70:rt:t2:t5 0x74:rs:t3:t7 0x80:rt:t4:t8 0x88:rd:t5:t0 0x88:rt:t4:t8 0x8c:rs:t5:t0 0xa0:rt:t6:t1 0xa4:rs:t6:t1 0xa4:rt:t6:t1 0xac:rd:t7:t2 0xac:rt:t6:t1 0xb0:rd:t8:t3 0xb0:rt:t7:t2 0xb4:rs:t8:t3 */
/* Address: 0x80140E84, Size: 204 bytes (51 instructions) */
#include "types.h"

extern s32 girl0_flag;
extern u8 back_clear_b[];
extern u8 girl5_blk_end[];
extern u32 girl11_flag;

s32 etc_flags_count13_check(void)
{
    s32 count;
    u8 *p;
    u8 *end;

    count = 0;
    if ((girl0_flag << 30) < 0) {
        count++;
    }
    end = girl5_blk_end;
    p = back_clear_b;
    do {
        if ((*(s32 *)(p + 456) << 30) < 0) {
            count++;
        }
        if ((*(s32 *)(p + 512) << 30) < 0) {
            count++;
        }
        if ((*(s32 *)(p + 568) << 30) < 0) {
            count++;
        }
        if ((*(s32 *)(p + 624) << 30) < 0) {
            count++;
        }
        p += 224;
    } while (p != end);
    if (((u32)(girl11_flag << 20) >> 29) != 0) {
        count++;
    }
    if (count == 13) {
        return 1;
    }
    return 0;
}
