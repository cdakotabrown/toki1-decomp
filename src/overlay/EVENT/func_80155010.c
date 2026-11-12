/* @SymbolStart: 0x80155008 */
/* @SymbolSize: 108 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v1:v0 0x14:rs:v1:v0 */
/* func_80155010 - EVENT overlay, Address: 0x80155010, Size: 116 bytes (29 insns) */
#include "types.h"

extern u8 D_800B1746;

extern u8 D_800B0A65;

extern void func_8004A7C4();
extern void func_8004A734(void);
extern void func_8004A764(void);

void func_80155010(void) {
    u8 v0_v = D_800B1746;

    u8 stateFlag;

    if (v0_v == 2) {
        func_8004A7C4();
        return;
    }

    if (v0_v == 1) {

        stateFlag = D_800B0A65;
        if (stateFlag == 0) {
            func_8004A734();
            return;
        }
    }

    func_8004A764();
}
