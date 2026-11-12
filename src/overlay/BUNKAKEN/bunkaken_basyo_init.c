/* Address: 0x801459F4 */
/* Size: 52 bytes (13 insns) */
#include "types.h"

extern s32 k_disp_start();
extern s32 k_reset();
extern s32 basyo_init();

extern u8 bunkaken_basyo_init_str0;

void bunkaken_basyo_init(void) {
    k_reset(0);
    basyo_init(&bunkaken_basyo_init_str0);
    k_disp_start(1);
}
