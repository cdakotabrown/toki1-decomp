/* Address: 0x80085EEC */
/* Size: 356 bytes (89 instructions) */
/* Region: Game (0x80041000-0x80085FFF), body spills past 0x80086000 */
/* @SymbolSize: 344 (the last 3 target words are IDO .text padding) */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"
#include "tokimeki/sprite.h"
extern SPRITE_REC Sprites[];


void bustup_face_set2(u8 a0) {
    switch (a0) {
    case 0:
        Sprites[0].tpage = 7;
        Sprites[0].u = 48;
        Sprites[1].tpage = 7;
        Sprites[1].u = 0;
        break;
    case 1:
        Sprites[0].tpage = 7;
        Sprites[0].u = 112;
        Sprites[1].tpage = 7;
        Sprites[1].u = 64;
        break;
    case 2:
        Sprites[0].tpage = 8;
        Sprites[0].u = 48;
        Sprites[1].tpage = 8;
        Sprites[1].u = 0;
        break;
    case 3:
        Sprites[0].tpage = 8;
        Sprites[0].u = 112;
        Sprites[1].tpage = 8;
        Sprites[1].u = 64;
        break;
    case 4:
        Sprites[0].tpage = 9;
        Sprites[0].u = 48;
        Sprites[1].tpage = 9;
        Sprites[1].u = 0;
        break;
    case 5:
        Sprites[0].tpage = 9;
        Sprites[0].u = 112;
        Sprites[1].tpage = 9;
        Sprites[1].u = 64;
        break;
    }
}
