/* @SymbolSize: 356 */
/* @PostProc: patch_insn_reg_field 0x2c:rd:t8:t9 0x30:rs:t8:t9 0x4c:rd:t9:t0 0x50:rd:t0:t1 0x50:rt:t9:t0 0x54:rs:t0:t1 0x5c:rd:t1:t3 0x60:rs:t1:t3 0x7c:rd:t2:t4 0x80:rd:t3:t5 0x80:rt:t2:t4 0x84:rs:t3:t5 0x8c:rd:t4:t7 0x90:rs:t4:t7 0xb4:rd:t5:t8 0xb8:rd:t6:t9 0xb8:rt:t5:t8 0xbc:rs:t6:t9 0xc4:rd:t7:t1 0xc8:rs:t7:t1 0xe0:rd:t8:t2 0xe4:rd:t9:t3 0xe4:rt:t8:t2 0xe8:rs:t9:t3 0xf0:rd:t0:t5 0xf4:rs:t0:t5 0x10c:rd:t1:t6 0x110:rd:t2:t7 0x110:rt:t1:t6 0x114:rs:t2:t7 0x11c:rd:t3:t9 0x120:rs:t3:t9 0x138:rd:t4:t0 0x13c:rd:t5:t1 0x13c:rt:t4:t0 0x140:rs:t5:t1 0x148:rd:t6:t3 0x14c:rs:t6:t3 */
/* Address: 0x801493E8 */
/* Size: 356 bytes (89 instructions) */
#include "types.h"

extern s32 bukatu_rec[];
extern s32 bukatu_tbl[];

#define BITCHK(x) ((((u32)(x) << 5) >> 30) >= 3 && ((x) << 1) < 0)

s32 get_etc_sel_index(void) {
    s32 i;
    s32 *p;

    if (BITCHK(bukatu_rec[0])) {
        return 0;
    }
    if (BITCHK(bukatu_rec[1])) {
        return 1;
    }
    if (BITCHK(bukatu_rec[2])) {
        return 2;
    }
    i = 3;
    p = bukatu_tbl;
    do {
        if (BITCHK(p[411])) {
            return i;
        }
        if (BITCHK(p[412])) {
            return i + 1;
        }
        if (BITCHK(p[413])) {
            return i + 2;
        }
        if (BITCHK(p[414])) {
            return i + 3;
        }
        p += 4;
        i += 4;
    } while (i != 11);
    return -1;
}
