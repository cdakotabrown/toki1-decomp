/* @SymbolStart: 0x80141D14 */
/* @SymbolSize: 92 */
/* Address: 0x80141D1C */
/* Size: 84 bytes (21 instructions) */
#include "types.h"
#include "tokimeki/date_obj_res_ex.h"

extern void sub_sub_step_up(void);
extern void load_image_rect(s32 a0, s32 a1, s32 a2, s32 a3, s32 t6);
extern s16 cal_cw_anim;
extern s16 cw97_anim;
extern s32 tpage_buf[];
extern DATE_OBJ_RES_EX shougatu_ev06_res;

void shougatu_image_load(void)
{
    s32 h_t6 = shougatu_ev06_res.ext_tile;

    load_image_rect(640, 0, 64, 128, h_t6);
    tpage_buf[10] = 1;
    cal_cw_anim = 2;
    cw97_anim = 0;
    sub_sub_step_up();
}
