/* @SymbolStart: 0x8013C818 */
/* @SymbolSize: 196 */
/* @RodataBase: 0x8014D5F0 */
/* @PostProc: patch_insn_reg_field 0x20:rd:t6:t7 0x28:rt:t6:t7 0x2c:rt:t6:t7 0x34:rs:t6:t7 0x3c:rt:t7:t8 0x40:rt:t7:t8 0x4c:rt:t8:t9 0x50:rt:t8:t9 0x5c:rt:t9:t0 0x60:rt:t9:t0 0x6c:rt:t0:t1 0x70:rt:t0:t1 0x7c:rt:t1:t2 0x80:rt:t1:t2 0x84:rt:t2:t3 0x8c:rt:t2:t3 0x90:rt:t3:t4 0x94:rt:t3:t4 0x98:rt:t4:t5 0xa0:rt:t4:t5 0xa4:rt:t5:t6 0xa8:rt:t5:t6 0xac:rt:t6:t7 0xb4:rt:t6:t7 0xb8:rt:t7:t8 0xbc:rt:t7:t8 0xc0:rt:t8:t9 0xc4:rt:t8:t9 */
/* Address: 0x8013C824, Size: 184 bytes (46 insns) */
#include "types.h"

u16 tt_pad_config_icon_get(s32 a0, u8 *a1)
{
    volatile u16 result;

    switch ((u32)(a0 & 0xFF)) {
    case 5:
        result = 30;
        *a1 = 0;
        break;
    case 6:
        result = 31;
        *a1 = 0;
        break;
    case 4:
        result = 32;
        *a1 = 0;
        break;
    case 7:
        result = 33;
        *a1 = 0;
        break;
    case 2:
        result = 34;
        *a1 = 15;
        break;
    case 3:
        result = 35;
        *a1 = 15;
        break;
    case 0:
        result = 36;
        *a1 = 15;
        break;
    case 1:
        result = 37;
        *a1 = 15;
        break;
    }

    return result;
}
