/* @CompileFlags: -cckr */
/* @SymbolStart: 0x80158470 */
/* @SymbolSize: 164 */
#include "types.h"
extern u8 D_800B1746;
extern u8 D_80125320;
extern u8 D_800B0BCF;
extern u8 D_800B1AF6;

func_80158478() {
    int r; int saved;
    saved = D_800B1AF6;
    func_8004CF30();
    if ((D_800B1746 | 0) == 9 && D_80125320 == 9) {
        if (D_800B0BCF == 0) func_8004A8EC(3); else func_8004A8EC(4);
        r = func_80033E88();
        if (saved != D_800B1AF6) D_800B1AF6 += 5;
    }
}
