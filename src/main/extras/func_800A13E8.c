/* Address: 0x800A13E8 */
/* Size: 84 bytes (21 instructions) */
/* Compiler: IDO 5.2 */
/* Region: IDO (0x80081000-0x800BFFFF) */
#include "types.h"

typedef struct {
    u32 field0;
    u32 field4;
    u32 field8;
} Struct800A13E8;

extern s32 get_tw(s32 arg);

void func_800A13E8(Struct800A13E8 *arg0, s32 arg1)
{
    u32 temp;
    Struct800A13E8 *ptr;

    ptr = arg0;
    temp = ptr->field0;
    temp = (temp & 0x00FFFFFF) | 0x02000000;
    ptr->field0 = temp;
    ptr->field4 = get_tw(arg1);
    ptr->field8 = 0;
}
