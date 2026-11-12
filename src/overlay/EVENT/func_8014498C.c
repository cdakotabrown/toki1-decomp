/* Address: 0x8014498C */
/* Size: 112 bytes (28 instructions) */
#include "types.h"

extern s32 D_800EECC0;
extern s32 ev_flag00;
extern u8 D_800E96AB;
extern u16 D_80094714;
extern u16 D_80094718;

extern void func_800F7074(void);
extern void func_80011DFC(void);

void func_8014498C(void) {
    func_800F7074();
    if (D_800EECC0 != 0) {
        D_800EECC0 = 0;
        ev_flag00 = 0;
        D_800E96AB &= 0xFF7F;
        D_80094714 = D_80094714 + 1;
        D_80094718 = 0;
        func_80011DFC();
    }
}
