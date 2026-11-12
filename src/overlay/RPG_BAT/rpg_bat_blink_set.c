/* Address: 0x8013A464 */
/* Size: 20 bytes */
/* @SymbolSize: 20 */
#include "types.h"
#include "tokimeki/class_w.h"
extern CLASS_W class_w[];


void rpg_bat_blink_set(u8 a, u8 b)
{
    class_w[144].clut = a;
    class_w[144].bright = b;
}
