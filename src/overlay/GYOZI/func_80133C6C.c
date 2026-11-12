/* @SymbolStart: 0x80133C3C */
/* @SymbolSize: 96 */
#include "types.h"

extern void func_8008FB00(void);
extern void func_8004DE1C(void);

extern s16 D_800F5666[];

void func_80133C6C(void)
{
    D_800F5666[2] = D_800F5666[2] - 1;
    D_800F5666[0] = D_800F5666[0] - 1;
    D_800F5666[4] = D_800F5666[4] + 10;
    func_8008FB00();
    func_8004DE1C();
}
