/* Address: 0x8004905C */
/* Size: 168 bytes (42 instructions) */
/* Region: Game (IDO 5.2) */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

typedef struct {
    u8 pad0[3];
    u8 classFlag;
    u8 pad4[64];
} Slot68;

extern Slot68 class_w[160];
extern char class_clear_str0[];

extern void printf(char *, s32);

void class_clear(s32 n)
{
    s32 i;

    printf(class_clear_str0, n);
    if (n != 0) {
        for (i = 0; i < n && i < 160; i++) {
            class_w[i].classFlag = 0;
        }
    } else {
        for (i = 0; i < 160; i++) {
            class_w[i].classFlag = 0;
        }
    }
}
