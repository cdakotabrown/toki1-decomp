/* Address: 0x80062EAC */
/* Size: 176 bytes (44 instructions) */
/* Region: Game (IDO 5.2) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

typedef struct {
    u8 pad0[3];
    u8 flags;
    u8 pad4[64];
} Chr68;

typedef struct {
    u32 attr;
    u8 pad[32];
} Obj36;

extern Chr68 class_w[];
extern Obj36 D_80129CB0[];

void bustup_disp_switch(s32 on)
{
    if (on == 0) {
        class_w[31].flags &= 0x7F;
        class_w[30].flags &= 0x7F;
        D_80129CB0[0].attr |= 0x80000000;
        D_80129CB0[1].attr |= 0x80000000;
        D_80129CB0[2].attr |= 0x80000000;
        D_80129CB0[3].attr |= 0x80000000;
        D_80129CB0[4].attr |= 0x80000000;
        D_80129CB0[5].attr |= 0x80000000;
    }
}
