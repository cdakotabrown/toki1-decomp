/* Address: 0x8009AE1C */
/* Size: 44 bytes (11 instructions) */
/* @SymbolSize: 44 */
/* @Compiler: gcc260 */
/* @CompileFlags: -O1 */
/* @PostProc: strip_leading_instructions --count auto */
/* Region: Library */
#include "types.h"

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    u16 unk10;
    u16 unk12;
    u16 unk14;
    u16 unk16;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
} Rec24;

void GsInitCoord2param(Rec24 *p)
{
    p->unk14 = 0;
    p->unk12 = 0;
    p->unk10 = 0;
    p->unk20 = 0;
    p->unk8 = 4096;
    p->unk4 = 4096;
    p->unk0 = 4096;
    p->unk1C = 0;
    p->unk18 = 0;
}
