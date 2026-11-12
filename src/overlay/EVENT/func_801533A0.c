/* @SymbolStart: 0x80153390 */
/* @SymbolSize: 268 */
/* @PostProc: swap_instructions --offset1 0x18 --offset2 0x1c */
/* Address: 0x80153390 (absorbed cross-prologue head) .. 0x801533A0 */
/* Size: 268 bytes (67 instructions) */
#include "types.h"

extern void func_8004C250(void);
extern void func_80011DFC(void);

extern s16 D_80094730;
extern u8 D_801243AC;
extern s16 D_800B096E;
extern s16 D_800B094E;
extern u8 D_800B1746;
extern u8 D_800B0860[];

void func_801533A0(void)
{
    s32 sel;
    s16 *rec;

    sel = D_801243AC;
    D_80094730 = (s16)(D_80094730 + sel);

    switch (sel) {
    case 0:
        D_800B096E = D_800B096E + 5;
        break;
    case 1:
        D_800B094E = D_800B094E + 5;
        break;
    case 2:
        rec = (s16 *)&D_800B0860[D_800B1746 * 52];
        rec[211] = rec[211] - 1;
        break;
    case 3:
        rec = (s16 *)&D_800B0860[D_800B1746 * 52];
        rec[211] = rec[211] + 1;
        break;
    default:
        break;
    }

    func_8004C250();
    func_80011DFC();
}
