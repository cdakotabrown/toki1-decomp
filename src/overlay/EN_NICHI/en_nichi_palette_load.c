/* en_nichi_palette_load - EN_NICHI overlay, Address: 0x80132378, Size: 216 bytes (54 insns) */
#include "types.h"

extern void palette_upload_enqueue();
extern void sub_sub_step_up();

extern volatile s32 en_nichi_intro_pal_ptr;

void en_nichi_palette_load(void)
{
    palette_upload_enqueue(en_nichi_intro_pal_ptr, 16, 1, 1, 0);
    palette_upload_enqueue(en_nichi_intro_pal_ptr, 17, 1, 1, 1);
    palette_upload_enqueue(en_nichi_intro_pal_ptr, 18, 1, 1, 2);
    palette_upload_enqueue(en_nichi_intro_pal_ptr, 19, 1, 1, 3);
    palette_upload_enqueue(en_nichi_intro_pal_ptr, 31, 1, 3, 0);
    palette_upload_enqueue(en_nichi_intro_pal_ptr, 30, 1, 3, 1);
    sub_sub_step_up();
}
