/* Address: 0x80059230 - 0x8005928C (92 bytes) */
/* Region: GCC (OR encoding) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

typedef struct {
    s32 field_0x00;
    u8 pad[28];
    s32 field_0x20;
    u8 pad2[44];
} Struct80;

extern Struct80 coord_buf[16];

extern void GsInitCoordinate2(s32 a0, Struct80 *a1);

void initCoordinate(void) {
    s32 i;

    for (i = 0; i < 16; i++) {
        GsInitCoordinate2(0, &coord_buf[i]);
        coord_buf[i].field_0x20 = -4000;
        coord_buf[i].field_0x00 = 0;
    }
}
