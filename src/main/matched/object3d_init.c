/* Address: 0x80059A08 */
/* Size: 128 bytes (32 instructions) */
/* Region: Game (0x80041000-0x80085FFF), IDO 5.2 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern u8 class_w[];

void object3d_init(s32 idx) {
    u8 *p = &class_w[idx * 68];

    p[0] = 1;
    p[1] = 0;
    p[2] = 0;
    p[3] = 0;
    *(u16 *)(p + 100) = 0;
    *(u16 *)(p + 102) = 0;
    *(u16 *)(p + 104) = 0;
    *(u16 *)(p + 106) = 0;
    *(u16 *)(p + 108) = 4096;
    *(u16 *)(p + 110) = 4096;
    *(u16 *)(p + 112) = 4096;
    *(u16 *)(p + 114) = 0;
    *(u16 *)(p + 116) = 0;
    *(u16 *)(p + 118) = 0;
    *(u16 *)(p + 120) = 0;
    *(u16 *)(p + 122) = 0;
    *(u16 *)(p + 124) = 0;
    *(u16 *)(p + 126) = 0;
    *(u16 *)(p + 128) = 0;
    *(u16 *)(p + 130) = 0;
    p[132] = 0;
    p[133] = 1;
    p[134] = 0;
}
