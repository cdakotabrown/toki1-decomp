/* @PostProc: patch_stack_complete --old 240 --new 256 */
/* @PostProc: remap_stack_slots --map '8:16' */
/* Address: 0x80154DAC */
/* Size: 404 bytes (101 instructions) */
#include "types.h"

typedef struct {
    u8 b[231];
} EvtTbl231;

extern EvtTbl231 D_8012536C;
extern s16 D_800B0962;
extern u8 D_800B1746;
extern u8 D_801243B0;

void func_80154DAC(s16 mode, s16 *outA, s16 *outB) {
    EvtTbl231 tbl;
    s16 i;
    u8 row;

    tbl = D_8012536C;
    if (mode > 0) {
        for (i = 0; i < 5; i++) {
            if (D_800B0962 >= 100 - i * 25) {
                *outA = 5 - i;
                break;
            }
        }
        if (D_800B0962 == 0) {
            *outA = 0;
        }
        row = D_800B1746;
        *outB = 5 - tbl.b[row * 21 + D_801243B0];
    } else {
        for (i = 0; i < 5; i++) {
            if (D_800B0962 >= 100 - i * 25) {
                *outA = i + 3;
                break;
            }
        }
        if (D_800B0962 == 0) {
            *outA = 8;
        }
        row = D_800B1746;
        *outB = tbl.b[row * 21 + D_801243B0] + 3;
    }
}
