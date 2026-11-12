/* @SymbolStart: 0x80155518 */
/* Address: 0x80155520 */
/* Size: 224 bytes (56 insns) */
/* @SymbolSize: 232 */
#include "types.h"

extern u8 D_800B1746;

extern s16 D_80094730;
extern volatile u8  D_8012531C;
extern volatile u8  D_800B0896;
extern volatile s16 D_8012438C;
extern volatile s16 D_8012438E;
extern volatile s16 D_80124390;
extern volatile s16 D_8012439C;
extern volatile s16 D_8012439E;
extern volatile s16 D_801243A0;

extern void func_80114B0C(void);
extern void func_80011DFC(void);

void func_80155520(void) {
    u8 arg_v = D_800B1746;

    s32 v0;

    if (arg_v == 1) {
        v0 = D_8012531C;
        if (v0 == 0) {
            D_80094730 = 4;
        }
        if (v0 == 1) {
            D_80094730 = 7;
        }
        if (v0 == 2) {
            D_80094730 = 10;
        }

        v0 = D_800B0896;
        if (v0 >= 6U && v0 < 9U) {
            D_80094730 = 0;
            D_8012439C = 1;
            D_8012439E = 4;
            D_801243A0 = 2;
            D_8012438C = 2;
            D_8012438E = 3;
            D_80124390 = 3;
        }

        func_80114B0C();
        return;
    }

    func_80011DFC();
}
