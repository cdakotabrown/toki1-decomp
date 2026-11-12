/* @SymbolStart: 0x80154868 */
/* @SymbolSize: 232 */
/* @CompileEnv: IDO52_CONST_INREG_VALUES=2,3:-1:1:6 */
/* @PostProc: swap_instructions --offset1 0xb8 --offset2 0xbc */
/* @PostProc: patch_insn_reg_field 0x50:rt:v0:t4 0x5c:rt:v1:t5 0x94:rt:v0:t4 0x9c:rt:v1:t5 0xa4:rt:v1:t5 0xac:rt:v0:t4 0xb4:rt:v0:t4 0xbc:rt:a0:v0 0xc0:rt:a0:v0 0xc8:rt:a0:v0 0xd0:rt:v0:t4 0xd8:rt:v1:t5 0xe4:rt:a0:v0 */
/* Address: 0x80154870 */
/* Size: 224 bytes (56 insns).  ⭐ BYTE-PERFECT 56/56 (2026-08-15). */
#include "types.h"

extern u8 D_800B1AF6;
extern s16 D_800B095E;
extern s16 D_80094730;
extern s16 D_8012438C;
extern s16 D_8012438E;
extern s16 D_80124390;
extern u16 D_80124394;
extern u16 D_80124396;
extern u16 D_80124398;
extern s16 D_8012439C;
extern s16 D_801243A0;
extern s16 D_801243A2;
extern s16 D_801243A4;

extern void func_80011DFC(void);

extern u8 D_800B1746;

void func_80154870(void)
{
    if (D_800B1746 != 6) {
        func_80011DFC();
        D_800B1AF6 = D_800B1AF6 + 8;
        return;
    }
    if (D_800B095E >= 150) {
        D_80094730 = D_80094730 + 3;
        func_80011DFC();
        D_800B1AF6 = D_800B1AF6 + 8;
        return;
    }
    D_8012439C = 2;
    D_8012438E = 3;
    D_8012438C = 3;
    D_801243A0 = 2;
    D_801243A4 = 2;
    D_80124396 = 3;
    D_80124394 = 3;
    D_801243A2 = 2;
    D_80124390 = 3;
    D_80124398 = 3;
    func_80011DFC();
}
