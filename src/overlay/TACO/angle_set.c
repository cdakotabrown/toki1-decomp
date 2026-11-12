/* Size: 48 bytes */
/* @SymbolSize: 48 */
#include "types.h"

extern s16 taco_angle_tbl[];

void angle_set(s32 idx, s16 *src)
{
    s16 *d;

    d = (s16 *)((u8 *)taco_angle_tbl + idx * 8);
    d[0] = src[3];
    d[2] = src[4];
    src += 5;
    d[1] = src[0];
}
