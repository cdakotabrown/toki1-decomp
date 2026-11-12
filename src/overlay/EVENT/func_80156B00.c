/* Address: 0x80156B00 */
/* Size: 148 bytes (37 instructions) */
#include "types.h"

typedef struct {
    u16 a[21];
} Rec42;

extern Rec42 D_801254D8;
extern u8 D_801243B0;

extern s32 func_8001496C(void);
extern void func_800433D0();
extern void func_80011DFC();

void func_80156B00(void) {
    s32 pad_hi;
    Rec42 buf;

    buf = D_801254D8;
    if (func_8001496C() == 1) {
        func_800433D0(buf.a[D_801243B0]);
        func_80011DFC();
    }
}
