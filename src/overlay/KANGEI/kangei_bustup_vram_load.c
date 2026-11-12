/* @SymbolSize: 76 */
/* kangei_bustup_vram_load - KANGEI overlay, Address: 0x801350F0, Size: 76 bytes (19 insns) */
#include "types.h"

extern void zyotai_sprite_set_chk(void);
extern void LoadImage(s16 *, void *);

void kangei_bustup_vram_load(void)
{
    s16 rect[4];

    zyotai_sprite_set_chk();
    rect[0] = 320;
    rect[1] = 128;
    rect[2] = 384;
    rect[3] = 128;
    LoadImage(rect, (void *)0x80180000);
}
