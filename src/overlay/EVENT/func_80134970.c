/* Address: 0x80134970 */
/* Size: 480 bytes (120 instructions) */
#include "types.h"

extern void func_800F9730(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g,
                          s32 h, s32 i, s32 j, s32 k, s32 l);
extern void func_8001A0CC(s32 a, s32 b, s32 c, s32 d, s32 e);

void func_80134970(void) {
    s32 white = 0x00FFFF00;

    func_800F9730(-150, -120, -155, -120, -80, 40, -105, 40, white, white,
                  9, 1);
    func_800F9730(-160, -120, -165, -120, -100, 40, -115, 40, 0x00FFFFFF,
                  0x00FFFFFF, 9, 1);
    func_800F9730(-162, -120, -167, -120, -110, 40, -135, 40, white, white,
                  9, 1);
    func_8001A0CC(0, 0, 9, 3, 0);
    func_800F9730(-125, -120, -130, -120, -40, 40, -50, 40, white, white,
                  13, 1);
    func_800F9730(-132, -120, -137, -120, -60, 40, -70, 40, 0x00FFFFFF,
                  0x00FFFFFF, 13, 1);
    func_8001A0CC(0, 0, 13, 3, 0);
}
