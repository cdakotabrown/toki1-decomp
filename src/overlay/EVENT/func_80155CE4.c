/* func_80155CE4 -- EVENT overlay, Address: 0x80155CE4 */
/* @SymbolStart: 0x80155C9C */
/* @SymbolSize: 144 */
/* @CompileEnv: IDO52_CONST_INREG_VALUES=3,4,2 */
/* @PostProc: scheduler_normalizer */
/* @PostProc: patch_insn_reg_field 0x10:rt:v0:t6 0x18:rt:v0:t6 0x20:rt:v0:t6 0x28:rt:v1:t7 0x2c:rt:v1:t7 0x34:rt:v1:t7 0x3c:rt:a0:t8 0x40:rt:a0:t8 0x48:rt:a0:t8 0x50:rt:v0:t6 0x5c:rt:v1:t7 0x6c:rt:a0:t8 */
#include "types.h"

extern void func_80011DFC(void);
extern u16 D_80124394;
extern u16 D_80124396;
extern u16 D_80124398;
extern u16 D_8012439C;
extern u16 D_8012439E;
extern u16 D_801243A0;
extern u16 D_801243A4;
extern u16 D_801243A6;
extern u16 D_801243A8;

void func_80155CE4(void) {
    D_80124396 = 3;
    D_80124394 = 3;
    D_8012439E = 4;
    D_8012439C = 4;
    D_801243A6 = 2;
    D_801243A4 = 2;
    D_80124398 = 3;
    D_801243A0 = 4;
    D_801243A8 = 2;
    func_80011DFC();
    func_80011DFC();
    func_80011DFC();
    func_80011DFC();
    func_80011DFC();
}
