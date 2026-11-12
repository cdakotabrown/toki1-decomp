/* @SymbolStart: 0x8014DFB8 */
/* @SymbolSize: 248 */
/* @CompileEnv: IDO52_AS1_XBB_SKIP_CAND=occ:1 */
/* Function: date_palette_cycle10 */
/* Overlay: DATE */
#include "types.h"

extern s32 frame_counter;
extern u8 now_sub_sub_step;
extern s16 date_anim_ptn;
extern s16 date_anim_ptn2;
extern s16 date_anim_ptn2_dir;
extern s32 date_ev38_res_pal;

extern void palette_upload_enqueue(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4);

void date_palette_cycle10(void)
{
    if ((u32)frame_counter % 10U != 0) {
        return;
    }
    if ((u32)now_sub_sub_step < 13U) {
        return;
    }

    date_anim_ptn++;
    if (date_anim_ptn >= 4) {
        date_anim_ptn = 0;
    }

    date_anim_ptn2 += date_anim_ptn2_dir;
    if (date_anim_ptn2 == 0) {
        date_anim_ptn2_dir = 1;
    }
    if (date_anim_ptn2 == 3) {
        date_anim_ptn2_dir = -1;
    }

    palette_upload_enqueue(date_ev38_res_pal, 19, 1, 2, date_anim_ptn);
    palette_upload_enqueue(date_ev38_res_pal, 18, 1, 2, date_anim_ptn2 + 4);
}
