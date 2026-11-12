/* @PostProc: pad_trailing_nop --count 1 */
/* @SymbolSize: 84 */
/* olh_box_show - OLH overlay, Address: 0x801323CC, Size: 92 bytes (23 insns) */
#include "types.h"

extern void _sprite_set_box_shade(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, u32 a5, s32 a6);
extern void dtd_on(s32 a0);

void olh_box_show(void) {
    _sprite_set_box_shade(-144, -88, 288, 196, 4, 0x00A0A0A0U, 2);
    dtd_on(4);
}
