/* Address: 0x80049370 */
/* Size: 80 bytes (20 instructions) */
/* Region: Game (0x80041000-0x80085FFF), IDO 5.2, leaf */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: patch_insn_reg_field --patch 0x14:rd:v1:a2 --patch 0x18:rs:v1:a2 --patch 0x20:rs:v1:a2 --patch 0x28:rd:a2:v1 --patch 0x44:rs:v1:a2 --patch 0x4c:rs:a2:v1 */
#include "types.h"
extern u8 screen_w[];
s32 *prim_alloc(s32 n, s32 slot) {
    u8 *p = &screen_w[slot * 12];
    s32 *r = *(s32 **)(p + 36) + *(s32 *)(p + 32);
    *(s32 *)(p + 32) += ((n + 3) / 4) * 4;
    return r;
}
