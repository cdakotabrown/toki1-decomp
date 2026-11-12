/* Address: 0x80145A28 */
/* Size: 52 bytes (13 insns) */
#include "types.h"

extern s32 k_reset();
extern s32 mes_window_switch();
extern s32 basyo_init();

extern u8 bunkaken_basyo_init2_str0;

void bunkaken_basyo_init2(void) {
    k_reset(0);
    basyo_init(&bunkaken_basyo_init2_str0);
    mes_window_switch(0);
}
