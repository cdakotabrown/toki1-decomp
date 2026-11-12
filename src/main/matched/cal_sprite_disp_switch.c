/* Address: 0x800692C4 */
/* Size: 192 bytes (48 instructions) */
/* Region: Game (0x80041000-0x80085FFF), IDO 5.2 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern volatile u32 Sprites;
extern volatile u32 sprite1_attribute;
extern volatile u32 sprite2_attribute;
extern volatile u32 sprite3_attribute;

void cal_sprite_disp_switch(s32 arg0) {
    if (arg0 == 1) {
        Sprites = Sprites & 0x7FFFFFFF;
        sprite1_attribute = sprite1_attribute & 0x7FFFFFFF;
        sprite2_attribute = sprite2_attribute & 0x7FFFFFFF;
        sprite3_attribute = sprite3_attribute & 0x7FFFFFFF;
    } else {
        Sprites = Sprites | 0x80000000;
        sprite1_attribute = sprite1_attribute | 0x80000000;
        sprite2_attribute = sprite2_attribute | 0x80000000;
        sprite3_attribute = sprite3_attribute | 0x80000000;
    }
}
