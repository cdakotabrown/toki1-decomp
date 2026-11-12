/* @SymbolStart: 0x80149F68 */
/* @SymbolSize: 440 */
/* @CompileEnv: IDO52_AS1_NO_LA_PULL=0 IDO52_AS1_XBB_VOLATILE_OK=0 */
/* Function: date_serifu_param_add */
/* Overlay: DATE */
/* Address: 0x80149F70 */
/* Size: 432 bytes (108 instructions) */
#include "types.h"

extern s32 bustup_face_change();
extern s32 param_clamp();
extern s32 sub_sub_step_up();

extern volatile u8 now_girl;
extern u8 screen_w[];
extern s32 serifu_no;

void date_serifu_param_add(void)
{
    s32 resp;
    s16 *rec;

    resp = serifu_no;
    bustup_face_change((u8)(resp + 1));
    if (serifu_no == 0) {
        rec = (s16 *)(screen_w + (s32)now_girl * 56);
        rec[223] = (s16)(rec[223] + 3);
        rec = (s16 *)(screen_w + (s32)now_girl * 56);
        rec[225] = (s16)(rec[225] + 2);
        rec = (s16 *)(screen_w + (s32)now_girl * 56);
        rec[227] = (s16)(rec[227] - 15);
    } else if (serifu_no == 1) {
        rec = (s16 *)(screen_w + (s32)now_girl * 56);
        rec[223] = (s16)(rec[223] + 2);
        rec = (s16 *)(screen_w + (s32)now_girl * 56);
        rec[225] = (s16)(rec[225] + 1);
        rec = (s16 *)(screen_w + (s32)now_girl * 56);
        rec[227] = (s16)(rec[227] - 10);
    } else {
        rec = (s16 *)(screen_w + (s32)now_girl * 56);
        rec[225] = (s16)(rec[225] + 1);
        rec = (s16 *)(screen_w + (s32)now_girl * 56);
        rec[227] = (s16)(rec[227] + 10);
    }
    param_clamp();
    sub_sub_step_up();
}
