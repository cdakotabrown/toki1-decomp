/* kangei_basyo_image_load - KANGEI overlay, Address: 0x80136CE0, Size: 128 bytes (32 insns) */
#include "types.h"

extern void basyo_string_step(void);
extern void load_image_rect(s32, s32, s32, s32, void *);

extern u8 kangei_basyo_mark_img;

void
kangei_basyo_image_load(void)
{
    void *buf = (void *)&kangei_basyo_mark_img;

    basyo_string_step();
    load_image_rect(800, 256, 4, 12, buf);
    load_image_rect(804, 256, 4, 12, buf);
    load_image_rect(808, 256, 4, 12, buf);
}
