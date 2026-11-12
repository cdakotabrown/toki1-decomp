/* @SymbolStart: 0x80156874 */
/* @SymbolSize: 112 */
/* @RodataBase: 0x801200B4 */
/* Address: 0x80156874 (prologue 0x8015687C) */
/* Size: 104 bytes (26 instructions) */
#include "types.h"

extern s32 func_80011DFC(void);
extern void func_80045FF4(void);
extern void func_X0(void);
extern void func_X1(void);
extern void func_X2(void);
extern void func_X3(void);
extern void func_X4(void);
extern void func_X5(void);
extern void func_X6(void);
extern void func_X7(void);
extern void func_X8(void);
extern void func_X9(void);

extern u8 D_800B1746;

extern u8 D_8012531C;

s32 func_8015687C(void) {
    if (D_8012531C == 1) {
        switch (D_800B1746) {
            case 0: case 1: case 2: case 3: case 4:
            case 5: case 6: case 7: case 8: case 9:
                func_80011DFC();
                return 0;
        }
    }
    func_80045FF4();
}
