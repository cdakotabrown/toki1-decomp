/* geko_kanji_set_step -- GEKO overlay, Address: 0x80140D64, Size: 76 bytes (19 insns) */
#include "types.h"

extern void set_kanji_string(s32 a0, s32 a1, s32 a2, s32 *a3, s32 stack0);
extern void device_cmd_send(s32 arg);
extern void sub_sub_step_up(void);

extern s32 geko_kanji_set_step_str0;

void geko_kanji_set_step(void)
{
    set_kanji_string(-128, 48, 1, &geko_kanji_set_step_str0, 0);
    device_cmd_send(36);
    device_cmd_send(1280);
    sub_sub_step_up();
}
