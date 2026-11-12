/* @SymbolSize: 68 */
/* Address: 0x80135360 */
#include "types.h"

extern s32 clut_set_adr;

extern void goto_sub_step();
extern void palette_upload_enqueue();
extern void all_object_disp_off();

void taco_reset5(void)
{
    all_object_disp_off();
    palette_upload_enqueue(clut_set_adr, 31, 1, 1, 0);
    goto_sub_step(3);
}
