/* @SymbolStart: 0x80153C0C */
/* @SymbolSize: 204 */
/* Address: 0x80153C1C, Size: 188 bytes (47 instructions) */
#include "types.h"

extern s32 sub_sub_step_up(void);
extern s32 palette_upload_enqueue(s32 handle, s32 a1, s32 a2, s32 a3, s32 a4);

extern s32 date_ev2f_res_pal;
extern u16 now_serifuev;
extern s32 serifu_no;
extern u8 date_ev2f_ans;

s32 date_serifu_palette_step(void)
{
    s32 sel;

    sel = serifu_no;
    now_serifuev = now_serifuev + sel;
    if (sel == 0) {
        palette_upload_enqueue(date_ev2f_res_pal, 17, 1, 2, 0);
        date_ev2f_ans = 0;
    } else if (sel == 1) {
        palette_upload_enqueue(date_ev2f_res_pal, 17, 1, 2, 2);
        date_ev2f_ans = 2;
    } else {
        palette_upload_enqueue(date_ev2f_res_pal, 17, 1, 2, 1);
        date_ev2f_ans = 1;
    }
    return sub_sub_step_up();
}
