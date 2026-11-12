/* Function: func_80079DCC */
/* Address: 0x80079DCC */
/* Size: 196 bytes (49 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @Measured: 2026-09-24 2/48 exact (4.2%), Body 48 vs target 49 words -- SEMI (alt-entry fragment) */
#include "types.h"

extern s32 gSDInfo;
extern s32 gSDHeadAdrs;
extern s32 gSDBodyAdrs;
extern s32 gSDBodyLAdrs;

extern s32 SsVabOpenHeadSticky(s32, s32, s32);
extern void device_port_error_set(s32);
extern s32 SsVabTransBody(s32);
extern void SsVabTransCompleted(s32);

void func_80079DCC(s32 *base, s32 offset, s32 channel) {
    s32 *ptr;
    s32 val_a2;
    s32 val_a0;
    s32 result;
    s16 ch;

    ptr = (s32 *)((s8 *)base + offset);
    val_a2 = *(s32 *)((s8 *)ptr - 8720);
    val_a0 = *(s32 *)((s8 *)ptr - 8752);
    ch = (s16)channel;
    result = SsVabOpenHeadSticky(val_a0, ch, val_a2);
    if (result == -1) {
        device_port_error_set(((ch << 8) & 0xFF00) | 0x01010000);
        return;
    }
    gSDInfo |= (0x10000 << ch);
    result = SsVabTransBody(*(s32 *)((s8 *)ptr - 8736));
    if (result == -1) {
        device_port_error_set(((ch << 8) & 0xFF00) | 0x01020000);
        return;
    }
    SsVabTransCompleted(1);
    (void)gSDHeadAdrs;
    (void)gSDBodyAdrs;
    (void)gSDBodyLAdrs;
}
