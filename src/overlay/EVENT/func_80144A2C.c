/* @CompileEnv: IDO52_UGEN_TEMP_REG=5:v0,6:t1,7:t2,8:t3,9:t4,10:t5,11:t6,12:t7 IDO52_UGEN_TEMP_REG_INPLACE=5 */
/* @SymbolStart: 0x80144A24 */
/* @SymbolSize: 180 */
/* Address: 0x80144A2C */
/* Size: 180 bytes (45 instructions, incl. the 2-insn hoisted head) */
#include "types.h"

extern s32 D_800B1AF0;

extern u8 D_800EB02B;

extern s16 D_800EB042;
extern s16 D_800EB044;

extern u8 D_80123988;

extern void func_80011DFC();

void func_80144A2C(void) {
    s32 initFlag_v = D_800B1AF0;

    u8 tmp;

    if (initFlag_v == 0) {

        tmp = D_800EB02B;
        tmp |= 0x80;
        D_800EB02B = tmp;
        D_800EB044 = 0;
        D_800EB042 = 0;
    }

    D_800EB042 = D_800EB042 + 128;
    D_800EB044 = D_800EB044 + 128;
    if (D_800EB042 >= 4096) {

        tmp = D_800EB02B;
        tmp &= ~0x80;
        D_800EB02B = tmp;
        D_80123988 = 1;
        func_80011DFC();
    }

    D_800B1AF0 = D_800B1AF0 + 1;
}
