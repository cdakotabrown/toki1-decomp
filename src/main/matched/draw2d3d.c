/* Address: 0x80043AE8 */
/* Size: 20 bytes (5 instructions) */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"

extern u8 scene_sub_a;
extern u8 scene_sub_b;

void draw2d3d(s32 a0, s32 a1) {
    scene_sub_a = (u8)a0;
    scene_sub_b = (u8)a1;
}
