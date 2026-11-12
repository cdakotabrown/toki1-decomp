/* @SymbolSize: 152 */
/* @CompileEnv: IDO52_AS1_XBB_VOLATILE_OK=all IDO52_UGEN_TEMP_REG=5:t1 */
/* func_80133988 -- GYOZI overlay, Address: 0x80133988, Size: 152 bytes (38 insns) */
#include "types.h"

extern void func_800BCE10(void *a, void *b);
extern void func_8004DE1C(void);

extern u8 D_800F62CF;
extern volatile s16 D_80145EB4;
extern u8 D_80145EC8;
extern u8 D_80145F60;
extern char D_800D92A0[];
extern char D_801458F0[];

void func_80133988(void)
{
    s32 v0;
    s32 v1;

    func_800BCE10(D_800D92A0, D_801458F0);

    if (D_80145F60 != 0) {
        v0 = 0;
    } else {
        v0 = 15;
    }
    v1 = D_80145EC8;

    if (v1 == 0) {
        v0 = (s16)(v0 + 2);
    }
    D_80145EB4 = v0;
    if (v1 == 0) {
        D_80145EB4 = v0;
        if (D_800F62CF == 5) {
            D_80145EB4 = v0 + 2;
        }
    }

    func_8004DE1C();
}
