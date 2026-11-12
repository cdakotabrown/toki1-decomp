/* @SymbolStart: 0x80133800 */
/* @SymbolSize: 128 */
/* ending_serifu_face_step - ENDING overlay, Address: 0x80133800, Size: 128 bytes (32 instructions) */
#include "types.h"

extern void bustup_face_change();
extern void palette_upload_enqueue();
extern void serifu_run();
extern void sub_sub_step_up();
extern s32 bustup_check_init_tbl3;
extern s16 now_serifu_in;
extern s32 kuchi_stop;

void ending_serifu_face_step(void)
{
    s16 step = now_serifu_in;
    s32 a0;
    s16 t8;

    if (step == 4) {
        bustup_face_change(4);
        a0 = bustup_check_init_tbl3;
        palette_upload_enqueue(a0, 16, 1, 2, 1);
    }
    serifu_run();
    t8 = now_serifu_in;
    if (t8 == 7) {
        kuchi_stop = 1;
    } else {
        kuchi_stop = 0;
    }
}
