/* Address: 0x8014A1A4 */
/* Size: 96 bytes (24 insns) */
/* @SymbolSize: 96 */
#include "types.h"

extern u32 step_counter;

extern void sd_cmd_limit_send(s32, s32);
extern void taco_rows_wait(void);
extern void taco_chime_play(void);

void taco_main16_rows_chime_step(void) {
    taco_rows_wait();
    taco_chime_play();
    if ((step_counter % 180) == 0) {
        sd_cmd_limit_send(0, 1280);
    }
    step_counter++;
}
