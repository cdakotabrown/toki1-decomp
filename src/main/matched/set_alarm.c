/* Debug Symbol: set_alarm (from o.bin, high match) */
/* Address: 0x800A2880 (48 bytes) */
/* Likely Symbol: set_alarm (from o.bin, offset +92 bytes from 0x800A2824) */
/* @PostProc: strip_leading_instructions --count 4 */
#include "common.h"

extern s32 libgpu_alarm_deadline_vcount;
extern s32 libgpu_alarm_spin_count;
extern s32 VSync(s32 arg0);

void set_alarm(void) {
    libgpu_alarm_deadline_vcount = VSync(-1) + 240;
    libgpu_alarm_spin_count = 0;
}
