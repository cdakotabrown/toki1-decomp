/* @PostProc: scheduler_normalizer --block-aware */
/* Address: 0x801476E4 */
/* Size: 352 bytes (88 instructions) */
#include "types.h"

extern u16 D_80094718;
extern volatile u8 D_800E9E5F;
extern s32 ev_flag04;
extern volatile u8 D_800E9E1B;
extern volatile s32 D_800EC190;
extern s16 D_800E9E74;
extern volatile s32 D_800EC1B4;
extern s32 D_800B1AE4;
extern u8 D_800EAFA2;
extern s16 D_800EAFB8;

extern void func_800F7074(void);
extern void func_8003424C(void);
extern void func_80038388(void);

void func_801476E4(void) {
    func_800F7074();
    if (D_80094718 == 2) {
        ev_flag04 = 0;
        D_800E9E5F |= 0x80;
        D_800E9E1B |= 0x80;
        D_800E9E74 = 0;
        D_800EC190 &= 0x7FFFFFFF;
        D_800EC1B4 &= 0x7FFFFFFF;
        if ((u32)D_800B1AE4 % 240U == 0) {
            func_8003424C();
        }
    } else {
        ev_flag04 = 1;
        D_800E9E5F |= 0x80;
        D_800E9E1B |= 0x80;
        D_800EC190 &= 0x7FFFFFFF;
        D_800EC1B4 &= 0x7FFFFFFF;
        D_800EAFA2 = 0;
        D_800EAFB8 = 0;
        if ((u32)D_800B1AE4 % 240U == 120) {
            func_80038388();
        }
    }
}
