/* Likely Symbol: holiday_init_0 (from o.bin, offset +96 bytes from 0x800850B4) */
/* Address: 0x80085054 - 0x80085074 (32 bytes) */
/* Region: IDO (>= 0x80081000) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern void yazirushi_disp_switch(s32 arg0);

void yazirushi_off(void) {
    yazirushi_disp_switch(0);
}
