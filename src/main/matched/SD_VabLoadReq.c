/* Address: 0x8007BF24 */
/* Size: 112 bytes (28 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: remap_stack_slots --map 24:40 */
/* @PostProc: swap_branch_operands --offsets 0x18 */
#include "types.h"

extern u16 sd_vab_ch0;
extern s32 gSDInfo;
extern s32 gSDSerialInfo;

extern void seq_restart(s16 n);

void SD_VabLoadReq(s16 n) {
    if (n == sd_vab_ch0) {
        if (gSDInfo & 0x100) {
            seq_restart(n);
        }
    }
    gSDSerialInfo |= 0x01000000 << n;
}
