/* Address: 0x80065AA0, Size: 44 bytes (two jr-ra paths) */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"
extern s16 cw93_anim;
void icon_mem_card_switch(s32 a) {
    if ((a & 0xFF) != 0) {
        cw93_anim = 0xE;
    } else {
        cw93_anim = 0xD;
    }
}
