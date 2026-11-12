/* @PostProc: swap_instructions --offset1 0x2c --offset2 0x30 */
/* Address: 0x80140824 */
/* Size: 92 bytes (23 insns) */
#include "types.h"

extern s32 coord_buf[16][20];
extern s32 daen_obj;

extern void GsInitCoordinate2(s32, s32*);

void taiiku_coord_init(void)
{
    s32 *ptr;
    s32 c;

    ptr = (s32*)coord_buf;
    c = -4000;
    do {
        GsInitCoordinate2(0, ptr);
        ptr[8] = c;
        ptr[0] = 0;
        ptr += 20;
    } while (ptr != &daen_obj);
}
