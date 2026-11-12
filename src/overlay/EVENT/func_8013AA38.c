/* @CompileEnv: IDO52_COPYPROP_VAR=-4 */
/* func_8013AA38 -- EVENT overlay, Address: 0x8013AA38, Size: 176 bytes (44 insns) */
#include "types.h"

typedef void (*FuncPtr)(void);

typedef struct {
    FuncPtr ptrs[49];
} JumpTable;

extern void func_80078950(char *s, u8 a1);
extern void func_800FEF74(void);

extern u8 D_800B1AF6;
extern char D_8011F740;
extern JumpTable D_80121968;

void func_8013AA38(void)
{
    s32 v;
    JumpTable table;

    table = D_80121968;

    func_80078950(&D_8011F740, D_800B1AF6);
    table.ptrs[D_800B1AF6]();

    v = D_800B1AF6;
    if (v == 23 || v == 24) {
        func_800FEF74();
    }
}
