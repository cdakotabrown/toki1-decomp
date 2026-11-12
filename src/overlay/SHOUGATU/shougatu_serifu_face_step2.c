/* @PostProc: swap_registers v0:v1 --bidirectional --exclude-offsets 0x6C */
/* Address: 0x80137DE4 */
/* Size: 112 bytes (28 instructions) */
#include "types.h"

extern u8 get_g_zyotai_h(s32 arg);
extern void bustup_face_change(s32 arg);
extern void shougatu_date_serifu_run(void);
extern s16 shougatu_date_now_serifu_in;

void shougatu_serifu_face_step2(void)
{
    s32 v1;

    shougatu_date_serifu_run();
    v1 = shougatu_date_now_serifu_in;
    if (v1 == 3) goto call3;
    if (v1 != 11) goto check5;
call3:
    bustup_face_change(3);
    return;
check5:
    if (v1 == 5) goto domul;
    if (v1 != 13) return;
domul:
    bustup_face_change(get_g_zyotai_h(8) & 0xFF);
}
