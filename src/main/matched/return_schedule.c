/* Function: return_schedule */
/* Address: 0x80072C1C */
/* Size: 100 bytes (25 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern u8 return_schedule_tbl0;
extern u8 schedule_cursor;
extern void goto_step(s32);
extern void device_port_init(s32, s32);

void return_schedule(s32 arg) {
    if (return_schedule_tbl0 != 0) {
        goto_step(34);
        return;
    }
    if (arg == 1) {
        device_port_init(0, 0);
    }
    schedule_cursor++;
    goto_step(49);
}
