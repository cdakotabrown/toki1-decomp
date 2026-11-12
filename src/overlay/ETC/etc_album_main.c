/* @SymbolStart: 0x80140770 */
/* @SymbolSize: 256 */
/* @CompileEnv: IDO52_FORCE_WEB_NOSPILL=6 IDO52_FORCE_WEB_COLOR_NOSPLIT=6 */
/* Address: 0x80140780 (prologue); window starts at the absorbed */
#include "types.h"

extern s32 step_state;
extern u8 now_sub_step;
extern s32 pad_trig;
extern s32 etc_model_roty;
extern s32 etc_skip_req;

extern void etc_sub_step0();
extern void etc_sub_step1();
extern void etc_xa_main();
extern void etc_sub_stepff();
extern void basyo_show();
extern void etc_frame_save2();
extern void etc_model_matrix_set();
extern void goto_sub_step(s32);

void etc_album_main(void)
{
    step_state++;
    switch (now_sub_step) {
    case 0:
        etc_sub_step0();
        break;
    case 1:
        etc_sub_step1();
        break;
    case 2:
        etc_xa_main();
        break;
    case 255:
        etc_sub_stepff();
        break;
    }
    basyo_show();
    if (now_sub_step != 255 && etc_skip_req == 0 && (pad_trig & 0x800)) {
        etc_frame_save2();
        etc_model_roty = 0x800;
        etc_model_matrix_set();
        etc_skip_req = 1;
        goto_sub_step(2);
    }
}
