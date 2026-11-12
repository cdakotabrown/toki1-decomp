/* @SymbolStart: 0x8013E000 */
/* @SymbolSize: 228 */
/* Address: 0x8013E008 */
/* Size: 228 bytes (57 instructions, incl. the 2-insn hoisted head) */
#include "types.h"

extern s32 D_800EECBC;

extern u8  D_800B0E31;
extern volatile s16 D_800B0A3A;
extern volatile s16 D_800B0A3E;
extern volatile s16 D_800B0A42;
extern s16 D_80124394;
extern s16 D_80124396;
extern s16 D_80124398;
extern s16 D_801243A4;
extern s16 D_801243A6;
extern s16 D_801243A8;

extern void func_8004A8EC(s32);
extern void func_80033E88(void);
extern void func_80011EC4(s32);
extern void func_8004C250(void);
extern void func_80011DFC(void);

void func_8013E008(void) {
    s32 t6_v = D_800EECBC;

    if (t6_v == 1) {
        func_8004A8EC(2);
        func_80033E88();
        func_80011EC4(46);
    } else {
        D_800B0E31 = 1;
        D_80124394 = 3;
        D_80124396 = 3;
        D_80124398 = 3;
        D_801243A4 = 2;
        D_801243A6 = 2;
        D_801243A8 = 2;
        D_800B0A3A -= 2;
        D_800B0A3E -= 1;
        D_800B0A42 += 20;
        func_8004C250();
        func_80011DFC();
    }
}
