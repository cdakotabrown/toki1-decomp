/* @SymbolStart: 0x80142908 */
/* @SymbolSize: 72 */
/* Address: 0x80142910 */
/* Size: 64 bytes (16 instructions) */
#include "types.h"
#include "tokimeki/date_obj_res_ex.h"


extern void load_image_rect(s32, s32, s32, s32, s32);
extern void sub_sub_step_up(void);
extern s32 tpage_buf[];
extern DATE_OBJ_RES_EX shougatu_ev06_res;

void shougatu_image_load2(void) {
    load_image_rect(640, 0, 64, 128, shougatu_ev06_res.ext_tile);
    tpage_buf[10] = 1;
    sub_sub_step_up();
}
