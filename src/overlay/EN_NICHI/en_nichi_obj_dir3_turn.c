/* en_nichi_obj_dir3_turn - EN_NICHI overlay, Address: 0x801352DC, Size: 276 bytes (69 insns) */
/* @SymbolStart: 0x801352D8 */
/* @SymbolSize: 280 */
#include "types.h"

extern s32 rand(void);
extern u8 en_nichi_obj_wk[][48];

void en_nichi_obj_dir3_turn(s32 a0, s32 a1)
{
    s32 *p;
    s32 r;

    p = (s32 *)en_nichi_obj_wk[a0];
    if (p[10] >= 3) {
        return;
    }

    switch (p[1]) {
    case 1:
        if (a1 < 196) {
            r = rand();
            if (!(r & 1)) {
                p[11] = 0;
            } else {
                p[11] = 1;
            }
            p[10] = p[2] * 2 + 4;
            p[9] = rand() % 15 + 30;
        }
        break;
    case 2:
        if (a1 < 900) {
            r = rand();
            if (!(r & 1)) {
                p[11] = 0;
            } else {
                p[11] = 1;
            }
            p[10] = p[2] * 2 + 4;
            p[9] = rand() % 15 + 30;
        }
        break;
    }
}
