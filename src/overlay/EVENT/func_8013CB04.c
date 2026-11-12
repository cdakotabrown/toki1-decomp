/* Address: 0x8013CAB8 (cross-prologue head absorbed) .. 0x8013CB04 */
/* Size: 124 bytes (31 instructions) */
/* @SymbolStart: 0x8013CAB8 */
/* @SymbolSize: 124 */
#include "types.h"

extern s32 D_801204E8;
extern s32 D_80120504;
extern s32 D_80120520;
extern s32 D_80120678;
extern s32 D_8012067C;
extern s32 D_80120680;
extern volatile s16 D_800B0B72;
extern volatile s16 D_800B0B76;

extern void func_8004C250(void);
extern void func_80011DFC(void);

void func_8013CB04(void) {
    D_80120678 = D_801204E8;
    D_8012067C = D_80120504;
    D_80120680 = D_80120520;
    D_800B0B72 = D_800B0B72 + 3;
    D_800B0B76 = D_800B0B76 + 2;
    func_8004C250();
    func_80011DFC();
}
