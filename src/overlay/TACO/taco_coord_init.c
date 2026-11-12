/* Address: 0x8013475C */
/* Size: 92 bytes (23 insns) */
/* @PostProc: swap_registers s1:s2 --bidirectional --exclude-offsets 0x14,0x1c,0x5c,0x60 */
/* @PostProc: swap_instructions --offset1 0x14 --offset2 0x1c */
/* @PostProc: swap_instructions --offset1 0x2c --offset2 0x30 */
#include "types.h"

typedef struct {
    s32 field_00;
    u32 pad_04[7];
    s32 field_20;
    u32 pad_24[11];
} Struct_8012F4F0;

extern Struct_8012F4F0 taco_coord;
extern void GsInitCoordinate2();

void taco_coord_init(void) {
    Struct_8012F4F0 *iter = &taco_coord;
    Struct_8012F4F0 *end = iter + 64;
    s32 val = -4000;

    do {
        GsInitCoordinate2(0, iter);
        iter->field_20 = val;
        iter->field_00 = 0;
        iter++;
    } while (iter != end);
}
