/* Address: 0x80086954 */
/* Size: 64 bytes (16 instructions) */
/* Region: Library (0x80086000+) */
/* Likely Symbol: DecDCTvlc */
/* Compiler: IDO 5.2 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: convert_or_to_addu */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern s32 device_load_poll(void);

extern s32 load_done_flag;

void load_done_flag_set(void) {
    if (load_done_flag == 0) {
        if (device_load_poll() == 1) {
            load_done_flag = 1;
        }
    }
}
