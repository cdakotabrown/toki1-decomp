/* @PostProc: scheduler_normalizer */
/* Address: 0x80147570 */
/* Size: 164 bytes (41 instructions) */
#include "types.h"

extern u8 D_800E9E1B;
extern u8 D_800E9E5F;
extern u32 D_800EC190;
extern u32 D_800EC1B4;
extern s32 ev_flag04;
extern u32 D_800B1AE4;

extern void func_800F7074(void);
extern void func_8003424C(void);

void func_80147570(void) {
    func_800F7074();
    ev_flag04 = 0;
    D_800E9E5F = D_800E9E5F | 0x80;
    D_800E9E1B = D_800E9E1B | 0x80;
    D_800EC190 = D_800EC190 & 0x7FFFFFFF;
    D_800EC1B4 = D_800EC1B4 & 0x7FFFFFFF;
    if (D_800B1AE4 % 240 == 0) {
        func_8003424C();
    }
}
