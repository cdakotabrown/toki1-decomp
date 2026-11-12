/* Address: 0x80099B70 */
/* Size: 376 bytes (REAL WINDOW, 94 words; 392 recorded) */
/* @SymbolStart: 0x80099B70 */
/* @SymbolSize: 376 */
/* @Compiler: gcc260 */
/* @CompileFlags: -O2 -fno-schedule-insns2 --konami-lib-sched */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern u32 *GsOUT_PACKET_P;
extern u16 GsORGOFSX;
extern u16 GsORGOFSY;

typedef struct PrimSpec {
    u32 flags;
    u16 u04;
    u16 u06;
    u16 u08;
    u16 u0A;
    u16 u0C;
    u8  u0E;
    u8  u0F;
    s16 u10;
    s16 u12;
    u8  u14;
    u8  u15;
    u8  u16;
} PrimSpec;

typedef struct SlotHdr {
    u8   pad0[4];
    u32 *p04;
    s32  v08;
} SlotHdr;

void GsSortFastSprite(PrimSpec *spec, SlotHdr *slot, u16 idx) {
    u32 flags;
    u32 *p;
    u32 *ot;
    u16 x, y;

    flags = spec->flags;
    if ((s32)flags < 0) {
        return;
    }
    if (spec->u08 == 0) {
        return;
    }
    if (spec->u0A == 0) {
        return;
    }
    p = GsOUT_PACKET_P;
    x = spec->u04 + GsORGOFSX; y = spec->u06 + GsORGOFSY;
    p[1] = (spec->u0C & 0x1F) | 0xE1000200 | ((flags >> 17) & 0x180) | ((flags >> 23) & 0x60);
    p[2] = ((flags >> 5) & 0x02000000) | 0x64000000 | ((flags << 18) & 0x01000000)
         | (spec->u16 << 16) | (spec->u15 << 8) | spec->u14;
    p[3] = x | (y << 16);
    p[4] = spec->u0E | (spec->u0F << 8) | (spec->u12 << 22) | ((spec->u10 << 12) & 0x3F0000);
    p[5] = spec->u08 | (spec->u0A << 16);
    ot = idx + slot->p04 - slot->v08;
    p[0] = *ot + 0x05000000;
    *ot = (u32)p & 0x00FFFFFF;
    GsOUT_PACKET_P = p + 6;
}
