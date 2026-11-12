/* @SymbolStart: 0x80133D64 */
/* @SymbolSize: 132 */
/* func_80133D6C -- GYOZI overlay, Address: 0x80133D64, Size: 132 bytes (33 insns) */
#include "types.h"

extern void func_801343A4(void);

extern u8 D_800F62CF;
extern u8 D_800F563A;
extern s16 D_80145EB8;

void func_80133D6C(void)
{
    u32 mode = D_800F62CF;
    u32 t1;

    if (mode == 4) {
        t1 = D_800F563A;
        if ((t1 >> 4) == 6) {
            if (D_80145EB8 == 1) {
                D_80145EB8 = 2;
                goto tail;
            }
        }
        if ((t1 >> 4) == 7) {
            if (D_80145EB8 == 0) {
                D_80145EB8 = 1;
            }
        }
    }
tail:
    func_801343A4();
}
