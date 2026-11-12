/* date_palette_cycle13 -- DATE overlay, Address: 0x8014DB18. BYTE-PERFECT 49/49. */
/* @SymbolStart: 0x8014DB08 */
/* @SymbolSize: 196 */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:0 */
/* @PostProc: swap_instructions --offset1 0xb0 --offset2 0xb4 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v1:v0 0x14:rs:v1:v0 */
#include "types.h"
#include "tokimeki/class_w.h"

extern s32 palette_upload_enqueue();

extern u8 cal_cw_bright;
extern s32 frame_counter;
extern s32 date_ev37_sub_res_pal;
extern volatile s16 date_anim_ptn;
extern CLASS_W class_w[];

s32 date_palette_cycle13(void)
{
    s32 v0;
    u32 t0;
    s32 a0v;

    v0 = cal_cw_bright;
    class_w[98].bright = (u8)v0;
    if ((u32)v0 < 8U) {
        class_w[98].disp_flag = class_w[98].disp_flag & 0x7F;
    } else {
        class_w[98].disp_flag = class_w[98].disp_flag | 0x80;
    }
    t0 = frame_counter;
    if (t0 % 13 == 0) {
        v0 = date_anim_ptn;
        v0 = (s16)(v0 + 1);
        if (v0 >= 4) {
            v0 = 0;
        }
        a0v = date_ev37_sub_res_pal;
        date_anim_ptn = (s16)v0;
        v0 = palette_upload_enqueue(a0v, 18, 1, 2, v0);
    }
    return v0;
}
