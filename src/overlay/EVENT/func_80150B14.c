/* @CompileEnv: IDO52_AS1_XBB_VOLATILE_OK=all IDO52_FORCE_WEB_COLOR=78:t2,79:t1 */
/* @SymbolStart: 0x80150B0C */
/* Address: 0x80150B14 */
/* Size: 232 bytes (58 insns) */
/* @SymbolSize: 240 */
/* @PostProc: patch_insn_reg_field 0x1c:rt:v0:t0 0x20:rd:v0:t0 0x20:rs:v0:t0 0x38:rs:v0:t0 0x90:rs:v1:t4 0x90:rt:t4:v1 0xa4:rs:t0:t8 0xa4:rt:t8:t0 0xc0:rs:v1:ra 0xc0:rt:ra:v1 0xd4:rs:v1:a3 0xd4:rt:a3:v1 */
#include "types.h"

extern u8 D_800B1746;

extern u8 D_800B0F35[];
extern volatile u8  D_800B0F2C;
extern volatile s16 D_80124394[];
extern s16          D_8012439C[];
extern volatile s16 D_801243A4[];

extern void func_80011DFC(void);

void func_80150B14(void) {
    u8 arg_v = D_800B1746;

    u8 key;
    volatile s16 *pA;
    s16 *pB;
    volatile s16 *pC;
    s16 i;
    s16 src, v;

    key = D_800B0F35[arg_v];
    pA = D_80124394;
    pB = D_8012439C;
    pC = D_801243A4;
    for (i = 0; i < 3; i++) {
        src = pB[i];
        pA[i] = src;
        v = pA[i];
        if (v == 0) {
            pA[i] = 1;
            v = pA[i];
        }
        if (v == 4) {
            pA[i] = 3;
        }
        if (D_800B0F2C == key) {
            pA[i] = 3;
        }

        pC[i] = src - 1;
        v = pC[i];
        if (v == -1) {
            pC[i] = 0;
            v = pC[i];
        }
        if (v == 3) {
            pC[i] = 2;
        }
    }

    func_80011DFC();
}
