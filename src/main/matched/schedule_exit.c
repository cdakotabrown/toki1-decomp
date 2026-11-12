/* Function: schedule_exit */
/* Address: 0x80071090 */
/* Size: 104 bytes (26 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern u8 step_mode;
extern void season_music_read(void);
extern s32 device_load_poll(void);
extern void device_cmd_send(s32);
extern void goto_step(s32);

void schedule_exit(void) {
    if (step_mode == 0) {
        season_music_read();
        step_mode++;
    } else {
        if (device_load_poll() == 1) {
            device_cmd_send(512);
            goto_step(99);
        }
    }
}
