/* @SymbolStart: 0x8013301C */
/* @SymbolSize: 108 */
/* date2_image_save - DATE2 overlay, Address: 0x80133034, Size: 84 bytes (21 insns) */
/* behind a shrunken @SymbolSize:80 window (truncation fake). Now the window is */
#include "types.h"

extern s32 step_counter;

void memcpy(s32 arg0, s32 arg1, s32 arg2);
void zyotai_sprite_set_chk(void);

void date2_image_save(void)
{
    s32 cnt;
    s32 n;

    n = step_counter * 5120;
    memcpy(n + (s32)0x801A0000, n + (s32)0x80180000, 5120);
    cnt = step_counter;
    cnt++;
    step_counter = cnt;
    if (cnt == 18) {
        zyotai_sprite_set_chk();
    }
}
