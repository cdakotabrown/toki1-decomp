/* Address: 0x80147268 */
/* Size: 52 bytes (13 instructions) */
#include "types.h"

extern s32 D_800EECC0;
extern s32 ev_flag00;

extern void func_800201FC();
extern void func_80011DFC();

void func_80147268(void)
{
    func_800201FC(1);
    D_800EECC0 = 0;
    ev_flag00 = 0;
    func_80011DFC();
}
