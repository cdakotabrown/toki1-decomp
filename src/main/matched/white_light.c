/* Address: 0x8004AF88 */
/* Size: 176 bytes (44 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: patch_stack_complete --old 48 --new 64 */
/* @PostProc: remap_stack_slots --map '44:56' */
#include "types.h"

extern void _sprite_set_box_shade(s32, s32, s32, s32, s32, s32, s32);
extern void dtd_on_tpage(s32, s32, s32, s32, s32);

void white_light(s32 intensity) {
    s32 color;

    if (intensity < 256) {
        color = intensity * 65793;
    } else {
        color = 0x00FFFFFF;
    }

    _sprite_set_box_shade(-160, -120, 160, 240, 1, color, 129);
    _sprite_set_box_shade(0, -120, 160, 240, 1, color, 129);
    dtd_on_tpage(0, 0, 1, 1, 1);
}
