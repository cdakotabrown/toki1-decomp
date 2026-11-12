/* Address: 0x80081330, Size: 104 bytes */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"
extern s16 sound_seek_time;
extern u8 sound_seek_wait;
extern u8 xcut;
extern s32 pad_trig;
extern void device_cmd_send(s32);

void serifu_skip(void) {
    s16 v = sound_seek_time;
    if (v < 513) return;
    if ((pad_trig & 0x00600060) == 0) return;
    device_cmd_send(180);
    sound_seek_wait = 0;
    sound_seek_time = 0;
    xcut = 1;
}
