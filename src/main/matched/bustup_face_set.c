/* Address: 0x800636C0 */
/* Size: 328 bytes (to the next known symbol at 0x80063808) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: scheduler_normalizer --block-aware */
/* Address: 0x800636C0 */
/* Size: 328 bytes (82 instructions) */
/* Region: Game */
#include "types.h"
#include "tokimeki/sprite.h"
extern SPRITE_REC Sprites[];


void bustup_face_set(u32 a0) {
    switch (a0) {
    case 0:
        Sprites[1].tpage = 7; Sprites[0].tpage = 7;
        break;
    case 1:
        Sprites[1].tpage = 7; Sprites[1].u += 64;
        Sprites[0].tpage = 7; Sprites[0].u += 64;
        break;
    case 2:
        Sprites[1].tpage = 8; Sprites[0].tpage = 8;
        break;
    case 3:
        Sprites[1].tpage = 8; Sprites[1].u += 64;
        Sprites[0].tpage = 8; Sprites[0].u += 64;
        break;
    case 4:
        Sprites[1].tpage = 9; Sprites[0].tpage = 9;
        break;
    case 5:
        Sprites[1].tpage = 9; Sprites[1].u += 64;
        Sprites[0].tpage = 9; Sprites[0].u += 64;
        break;
    }
}
