/* @CompileFlags: -cckr */
/* @SymbolStart: 0x8013AF74 */
/* @SymbolSize: 940 */
#include "types.h"
extern volatile u32 D_800B1AE4;
extern s8 D_801218DC[];

void func_8013AF84()
{
    short i;
    u32 d;

    if (D_800B1AE4 % 51 == 0) {
        for (i = 0; i < 12;) {
            D_801218DC[i++] = func_800789A0() % 4;
        }
    }
    d = 1000;
    i = (D_800B1AE4 << 4) % d;
    func_800FF320(i, 0, 9);
    i = (D_800B1AE4 << 4) % d;
    func_800FF320(i - 10, 1, 9);
    i = (D_800B1AE4 << 4) % d;
    func_800FF320(i - 20, 2, 9);
    i = ((D_800B1AE4 << 4) + 500) % d;
    func_800FF320(i, 6, 9);
    i = ((D_800B1AE4 << 4) + 500) % d;
    func_800FF320(i - 10, 7, 9);
    i = ((D_800B1AE4 << 4) + 500) % d;
    func_800FF320(i - 20, 8, 9);
    func_8001A0CC(0, 0, 9, 3, 0);
    d = 800;
    i = d - (D_800B1AE4 << 4) % d;
    func_800FF320(i, 3, 13);
    i = d - (D_800B1AE4 << 4) % d;
    func_800FF320(i + 10, 4, 13);
    i = d - (D_800B1AE4 << 4) % d;
    func_800FF320(i + 20, 5, 13);
    i = d - ((D_800B1AE4 << 4) + 400) % d;
    func_800FF320(i, 9, 13);
    i = d - ((D_800B1AE4 << 4) + 400) % d;
    func_800FF320(i + 10, 10, 13);
    i = d - ((D_800B1AE4 << 4) + 400) % d;
    func_800FF320(i + 20, 11, 13);
    func_8001A0CC(0, 0, 13, 3, 0);
}
