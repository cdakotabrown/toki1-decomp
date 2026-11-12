/* @CompileEnv: IDO52_AS1_XBB_VOLATILE_OK=all */
/* Address: 0x80153180 (true entry 0x80153178 -- cross-prologue head) */
/* Size: 364 bytes (91 instructions, incl. the 2-insn hoisted head) + 8-byte head = 364 absorbed */
/* @SymbolStart: 0x80153178 */
/* @SymbolSize: 364 */
#include "types.h"

extern volatile u8 D_800B0895;
extern volatile u8 D_800B1746;
extern s16 D_80094730;
extern u8 D_800B0860[];

extern void func_80011DFC(void);
extern void func_80011EC4(s32);
extern void func_8004C250(void);

void func_80153180(void) {
    s32 sel;
    u32 c;
    s16 *p;

    sel = D_800B0895;
    if (sel == 95) {
        c = D_800B1746;
        if (c == 2 || c == 8 || c == 10) {
            D_80094730 = 1;
        }
        p = (s16 *)&D_800B0860[D_800B1746 * 52];
        p[211] += 1;
        func_80011DFC();
    } else if (sel == 96) {
        c = D_800B1746;
        if (c == 2 || c == 8 || c == 10) {
            D_80094730 = 4;
        } else {
            D_80094730 = 6;
        }
        func_80011EC4(20);
    } else {
        c = D_800B1746;
        if (c == 2 || c == 8 || c == 10) {
            D_80094730 = 9;
        } else {
            D_80094730 = 11;
        }
        p = (s16 *)&D_800B0860[D_800B1746 * 52];
        p[211] += 1;
        func_80011EC4(31);
    }
    func_8004C250();
}
