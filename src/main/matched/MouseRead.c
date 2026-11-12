/* Address: 0x80059BE0 */
/* Size: 216 bytes (54 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* (2026-08-29: a dead `@PostProc: strip_instructions` was removed here. */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern u32 mouse_rec_tbl[];
extern u32 mouse_x_acc[];
extern u32 mouse_y_acc[];
extern u16 libapi_mouse_sense_x;
extern u16 libapi_mouse_sense_y;
extern void mouse_pos_clamp(void);

void MouseRead(s32 a0, s32 *a1) {
    s8 *ptr;

    mouse_pos_clamp();

    a1[0] = mouse_x_acc[a0] / libapi_mouse_sense_x;
    a1[1] = mouse_y_acc[a0] / libapi_mouse_sense_y;
    a1[2] = -256;

    ptr = (s8 *)mouse_rec_tbl[a0];
    if (ptr[0] == 0) {
        if (ptr[1] == 18) {
            a1[2] = ~ptr[3] & 0xC;
        }
    }
}
