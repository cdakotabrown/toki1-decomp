/* Address: 0x8014741C */
/* Size: 68 bytes (17 insns) */
/* @PostProc: patch_stack_complete --old 32 --new 56 */
/* @PostProc: remap_stack_slots --from 24 --to 48 --from 26 --to 50 --from 28 --to 52 --from 30 --to 54 */
/* @PostProc: swap_registers_auto t0:t6 */
/* @PostProc: swap_registers_auto t1:t7 */
#include "types.h"

typedef struct {
    s16 field_0;
    s16 field_2;
    s16 field_4;
    s16 field_6;
} Struct8014741C;

extern void LoadImage();

void taco_image_load(s32 arg0, s32 arg1, s16 arg2, s16 arg3)
{
    Struct8014741C sp;

    sp.field_0 = (s16)arg1;
    sp.field_2 = arg2;
    sp.field_4 = 256;
    sp.field_6 = arg3;
    LoadImage(&sp, arg0);
}
