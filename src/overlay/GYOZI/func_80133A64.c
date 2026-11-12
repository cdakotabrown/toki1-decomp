/* @SymbolStart: 0x80133A5C */
/* @SymbolSize: 60 */
/* func_80133A64 -- GYOZI overlay, Address: 0x80133A5C, Size: 60 bytes (15 insns) */
#include "types.h"

extern void func_8008E2E0(void);
extern void func_8008E280(void);

extern s32 D_8012E66C;

void func_80133A64(void)
{
    s32 temp = D_8012E66C;

    if (temp == 0) goto label0;
    func_8008E2E0();
    goto label1;

label0:
    func_8008E280();

label1:
    ;
}
