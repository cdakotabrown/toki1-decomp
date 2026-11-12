/* Function: seq_busy_check */
/* Address: 0x80079E90 */
/* Size: 76 bytes (19 instructions; widened 2026-09-27) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
/* @SymbolSize: 76 */
#include "types.h"

extern u16 sd_vab_ch0;
extern s32 gSDSerialInfo;

s32 seq_busy_check(s32 arg0) {
    u16 val;
    s32 flags;

    val = sd_vab_ch0;
    flags = gSDSerialInfo;
    if (!(flags & (0x10000000 << val))) {
        if (!(flags & (0x01000000 << val))) {
            return 1;
        }
    }
    return 0;
}
