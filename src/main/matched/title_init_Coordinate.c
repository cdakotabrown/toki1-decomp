/* Likely Symbol: title_init_Light (from o.bin, offset +244 bytes from 0x800619EC) */
/* Address: 0x80061AE0 - 0x80061B3C (92 bytes) */
/* Region: GCC (OR encoding) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

typedef struct {
    s32 field_0x00;
    u8 pad[28];
    s32 field_0x20;
    u8 pad2[44];
} Struct80_61AE0;

extern Struct80_61AE0 coord_buf_r[16];

extern void GsInitCoordinate2(s32 a0, Struct80_61AE0 *a1);

void title_init_Coordinate(void) {
    s32 i;

    for (i = 0; i < 16; i++) {
        GsInitCoordinate2(0, &coord_buf_r[i]);
        coord_buf_r[i].field_0x20 = 4000;
        coord_buf_r[i].field_0x00 = 0;
    }
}
