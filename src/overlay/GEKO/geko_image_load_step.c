/* @SymbolStart: 0x80139BA0 */
/* @SymbolSize: 72 */
/* Address: 0x80139BA8 */
/* Size: 64 bytes (16 instructions) */
#include "types.h"
#include "tokimeki/date_obj_res_ex.h"


extern void load_image_rect(s32, s32, s32, s32, s32);
extern void sub_sub_step_up(void);

extern s32 tpage_buf[];
extern GEKO_OBJ_RES_EX geko_ptc_res;

void geko_image_load_step(void) {
    s32 a0 = geko_ptc_res.ext_tile;

    load_image_rect(320, 0, 64, 128, a0);
    tpage_buf[5] = 1;
    sub_sub_step_up();
}
