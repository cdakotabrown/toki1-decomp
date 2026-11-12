/* Address: 0x80151380 */
/* Size: 72 bytes */
/* @SymbolSize: 72 */
#include "types.h"
#include "tokimeki/class_w.h"
extern CLASS_W class_w[];


void rpg_bat_fade_set(s32 a)
{
    class_w[149].bright = a;
    class_w[150].bright = a;
    class_w[151].bright = a;
    if (a < 128) {
        class_w[155].bright = 128 - a;
        class_w[156].bright = 128 - a;
        class_w[157].bright = 128 - a;
    }
}
