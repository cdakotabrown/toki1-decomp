/* Address: 0x8014338C */
/* Size: 52 bytes (13 insns) */
#include "types.h"

extern s32 k_disp_start(s32);
extern s32 k_reset(s32);
extern s32 basyo_init(s32);

extern u8 bunkasai_basyo_init2_str0;

void bunkasai_basyo_init2(void) {
    k_reset(0);
    basyo_init((s32)&bunkasai_basyo_init2_str0);
    k_disp_start(1);
}
