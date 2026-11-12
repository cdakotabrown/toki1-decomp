/* Function: SD_VabLoadNext */
/* Address: 0x80079D08 */
/* Size: 176 bytes (44 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: patch_stack_complete --old 56 --new 64 */
/* @SymbolStart: 0x80079D00 */
/* @SymbolSize: 184 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: remap_stack_slots 28:44 */
/* @PostProc: swap_instructions 0x50 0x54 */
/* @PostProc: patch_insn_reg_field 0x0:rt:t0:t1 0x4:rs:t0:t1 0x4:rt:t2:t1 0x18:rt:t1:t0 0x28:rs:t2:t1 0x44:rt:t9:t2 0x48:rt:t9:t2 0x50:rd:t4:t9 0x58:rs:t4:t1 0x58:rt:t2:t9 0x58:rd:t5:t1 0x60:rs:t5:t1 0x60:rd:t2:t1 0x64:rd:t6:t4 0x6c:rt:t2:t1 0x70:rs:t6:t4 0x80:rt:t0:t1 0x84:rs:t0:t1 0x84:rt:t0:t1 0x88:rd:t7:t5 0x90:rs:t7:t1 0x90:rt:t0:t5 0x90:rd:t0:t1 0x98:rt:t0:t1 0xa0:rt:t1:t0 */
#include "types.h"

extern s32 gSDSerialInfo;
extern void SD_VabLoad(s16 idx, s32 mask_lo, s32 lo_mask, s32 hi_mask);

void SD_VabLoadNext(void) {
    s32 hi_mask = 0xF0000000;
    s32 lo_mask = 0x0F000000;
    s32 val = gSDSerialInfo;
    s32 v0 = 0;

    do {
        s32 mask = (0x01000000 << v0) & lo_mask;
        if ((val & 0xFF000000 & mask) && !(val & 0xFF000000 & hi_mask)) {
            s32 newval = (0x10000000 << v0) & 0xF0000000;
            val = (val & ~mask) | newval;
            gSDSerialInfo = val;
            SD_VabLoad((s16)v0, mask, lo_mask, hi_mask);
            val = gSDSerialInfo & ~newval;
            gSDSerialInfo = val;
            return;
        }
        v0++;
    } while (v0 != 4);
}
