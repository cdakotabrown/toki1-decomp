/* date2_image_load - DATE2 overlay, Address: 0x80133088, Size: 76 bytes (19 insns) */
#include "types.h"

void zyotai_sprite_set_chk(void);
void LoadImage(void *arg0, void *arg1);

void date2_image_load(void)
{
    s16 rect[4];

    zyotai_sprite_set_chk();
    rect[0] = 320;
    rect[1] = 128;
    rect[2] = 384;
    rect[3] = 128;
    LoadImage(rect, (void *)0x80180000);
}
