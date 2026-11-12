/* Address: 0x80079DB8  Size: 216 bytes (54 insns) */
/* Region: Game */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern u32 gSDHeadAdrs[];
extern u32 gSDBodyAdrs[];
extern u32 gSDBodyLAdrs[];
extern u32 gSDInfo;

extern s32 SsVabOpenHeadSticky(u32 a0, s32 a1, u32 a2);
extern s32 SsVabTransBody(u32 a0);
extern void SsVabTransCompleted(s32 a0);
extern void device_port_error_set(u32 code);

void SD_VabLoad(s16 a0) {
    s32 idx = (s32)a0;
    s32 ret;

    ret = SsVabOpenHeadSticky(gSDHeadAdrs[idx], idx, gSDBodyLAdrs[idx]);
    if (ret == -1) {
        device_port_error_set((((u32)idx << 8) & 0xFF00u) | 0x01010000u);
        return;
    }
    gSDInfo |= (0x10000 << idx);
    ret = SsVabTransBody(gSDBodyAdrs[idx]);
    if (ret == -1) {
        device_port_error_set((((u32)idx << 8) & 0xFF00u) | 0x01020000u);
        return;
    }
    SsVabTransCompleted(1);
}
