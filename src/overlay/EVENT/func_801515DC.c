/* @CompileFlags: -Wc,-pic1 */
/* Address: 0x801515DC, Size: 312 bytes (78 instructions) */
/* @SymbolStart: 0x801515D4 */
/* @SymbolSize: 320 */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:2 IDO52_CONST_INREG_VALUES=1 */
/* @PostProc: patch_stack_complete --old 48 --new 80 */
/* @PostProc: scheduler_normalizer */
#include "types.h"

extern u8 D_80125304;
extern u8 D_80125308;
extern u8 D_80125320;
extern s16 D_80094730;
extern s32 D_800EECBC;
extern u8 D_800B1746;
extern u8 D_800B0BCF;

extern void func_800469F4();
extern void func_80115C18();
extern void func_80114B0C();

s32 func_801515DC(void) {
    s32 v0;
    s32 msg;

    v0 = D_800EECBC;
    if (v0 == 0) {
        msg = 17097;
        if (D_80125308 != 0 || D_80125304 != 0) {
            D_80125320 = 3;
        } else {
            msg = 17070;
            D_80125320 = 0;
        }
    }
    if (v0 == 1) {
        D_80094730 = D_80094730 + 5;
        if (D_80125308 != 0) {
            msg = 17106;
            D_80125320 = 4;
        } else {
            msg = 17079;
            D_80125320 = 1;
        }
    }
    if (v0 == 2) {
        D_80094730 = D_80094730 + 10;
        msg = 17088;
        D_80125320 = 2;
    }
    func_800469F4(msg);
    func_80115C18();
    func_80114B0C();
    if (D_800B1746 == 8 && D_80125320 == 3 && D_800B0BCF != 0) {
        D_800EECBC = 1;
        return 1;
    }
    D_800EECBC = 0;
    return 0;
}
