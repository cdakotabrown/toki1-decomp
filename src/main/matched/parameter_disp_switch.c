/* Address: 0x80063AD0 */
/* Size: 168 bytes (42 instructions) */
/* Region: Game (IDO 5.2) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

typedef struct {
    u8 pad0[3];
    u8 flags;
    u8 pad4[64];
} Chr68;

extern Chr68 class_w[];

void parameter_disp_switch(s32 on)
{
    s32 i;

    if (on == 1) {
        for (i = 0; i < 28; i++) { class_w[i + 32].flags |= 0x80; }
    } else {
        for (i = 0; i < 28; i++) { class_w[i + 32].flags &= 0x7F; }
    }
}
