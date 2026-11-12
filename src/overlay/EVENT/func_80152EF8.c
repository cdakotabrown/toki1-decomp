/* @CompileEnv: IDO52_FORCE_WEB_COLOR=2:v1 */
/* @SymbolStart: 0x80152EF0 */
/* @SymbolSize: 276 */
/* Address: 0x80152EF8 */
/* Size: 268 bytes (67 instructions) */
#include "types.h"

extern s16 D_8012439C;
extern s16 D_80094730;

extern void func_80011DFC(void);

extern u8 D_800B1746;

void func_80152EF8(void) {
    s16 sel2;

    switch ((u32)D_800B1746) {
    case 5:
    case 9:
        sel2 = D_8012439C;
        if (sel2 == 3) {
            D_80094730 += 1;
        } else if (sel2 == 2) {
            D_80094730 += 2;
        }
        break;
    case 7:
        sel2 = D_8012439C;
        if (sel2 == 4) {
            D_80094730 += 1;
        } else if (sel2 == 3) {
            D_80094730 += 2;
        }
        break;
    default:
        sel2 = D_8012439C;
        if (sel2 == 4) {
            D_80094730 += 1;
        } else if (sel2 == 2) {
            D_80094730 += 2;
        }
        break;
    }

    func_80011DFC();
}
