/* func_80141D20 -- GYOZI overlay, Address: 0x80141D20, Size: 232 bytes */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[39];
} JumpTable;

extern u8 D_800F647A;
extern JumpTable D_80148EB4;

extern u8 D_801317EB;
typedef struct {
    s8  pad0[7];
    s8  c7;
    s16 c8;
    s8  pad2[12];
    s16 d6;
    s16 d8;
} B8C0Blk;
extern B8C0Blk D_8012B8C0;

extern void func_80144018(void);

void func_80141D20(void)
{
    s32 counter;
    JumpTable table;

    table = D_80148EB4;
    table.ptrs[D_800F647A](128);

    if ((u32)D_800F647A < 16) {
        D_8012B8C0.c7 = D_801317EB;
        if (D_8012B8C0.d8 == 0) {
            if (D_8012B8C0.c8 == 0) {
                counter = (s16)(D_8012B8C0.d6 + 1);
                D_8012B8C0.d6 = counter;
                if (counter >= 3) {
                    D_8012B8C0.d6 = 0;
                }
            }
        }
    }
    func_80144018();
}
