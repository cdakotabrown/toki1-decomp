/* @SymbolStart: 0x8013A694 */
/* @SymbolSize: 472 */
/* @RodataBase: 0x8015AF70 */
/* Function: date_basyo18_en_nichi_step */
/* Overlay: DATE */
/* Address: 0x8013A694 (cross-prologue head) / 0x8013A6B0 (body) */
/* Size: 472 bytes (118 instructions) */
#include "types.h"

extern s32 bustup_face_change();
extern s32 param_clamp();
extern s32 sub_sub_step_up();

extern volatile u8 now_girl;
extern u8 screen_w[];
extern s16 para_str;
extern s16 now_serifu;
extern s32 en_nichi_res;

void date_basyo18_en_nichi_step(void)
{
    s16 *rec;
    s32 sel;

    sel = en_nichi_res;
    now_serifu = (s16)(now_serifu + sel);
    switch ((u32)sel) {
    case 0:
        rec = (s16 *)(screen_w + (s32)now_girl * 56);
        rec[223] = (s16)(rec[223] + 1);
        rec = (s16 *)(screen_w + (s32)now_girl * 56);
        rec[225] = (s16)(rec[225] + 1);
        para_str = (s16)(para_str - 10);
        bustup_face_change(1);
        break;
    case 1:
        rec = (s16 *)(screen_w + (s32)now_girl * 56);
        rec[225] = (s16)(rec[225] + 1);
        bustup_face_change(1);
        break;
    case 2:
        rec = (s16 *)(screen_w + (s32)now_girl * 56);
        rec[223] = (s16)(rec[223] - 1);
        bustup_face_change(2);
        break;
    case 3:
        rec = (s16 *)(screen_w + (s32)now_girl * 56);
        rec[223] = (s16)(rec[223] + 5);
        if (now_girl != 2) {
            bustup_face_change(0);
        } else {
            bustup_face_change(1);
        }
        break;
    case 4:
        bustup_face_change(2);
        break;
    }
    param_clamp();
    sub_sub_step_up();
}
