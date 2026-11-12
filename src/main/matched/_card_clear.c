/* Address: 0x800B47D0, Size: 48 bytes */
/* @PostProc: convert_or_to_addu */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"
extern s32 _new_card(s32 a);
extern s32 _card_write(s32 a, s32 b, s32 c);
s32 _card_clear(s32 a) {
    _new_card(a);
    return _card_write(a, 63, 0);
}
