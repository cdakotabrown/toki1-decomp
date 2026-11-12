/* @CompileFlags: -cckr */
/* @SymbolStart: 0x80153004 */
/* @SymbolSize: 372 */
/* @RodataBase: 0x80120010 */
/* Address: 0x80153004 (cross-prologue head absorbed; the .sym draws the */
/* start at 0x8015300C). Size: 372 bytes (93 insns) including the 2-insn head */
#include "types.h"

extern u8 D_800B1746;
extern s16 D_8012439C;
extern s16 D_80094730;

void func_80011DFC();

void func_8015300C()
{
    s16 three;

    three = 3;
    switch (D_800B1746) {
    case 0:
        if (D_8012439C == 4) {
            D_80094730 += 1;
        } else if (D_8012439C == 2) {
            D_80094730 += 2;
        }
        break;
    case 1:
        if (D_8012439C == 4) {
            D_80094730 += 1;
        } else if (three == D_8012439C) {
            D_80094730 += 2;
        }
        break;
    case 2:
        if (three == D_8012439C && D_8012439C == 2) {
            D_80094730 += 1;
        } else if (three == D_8012439C && three == D_8012439C) {
            D_80094730 += 2;
        }
        break;
    case 3:
        if (three == D_8012439C) {
            D_80094730 += 1;
        } else if (D_8012439C == 2) {
            D_80094730 += 2;
        }
        break;
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
        break;
    }
    func_80011DFC();
}
