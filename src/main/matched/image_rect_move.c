/* Size: 80 bytes (20 instructions) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: swap_registers t8 t7 */
/* @PostProc: swap_registers t9 t8 */
/* @PostProc: swap_registers t0 t9 */
#include "types.h"

extern void MoveImage(u16 *arr, u16 arg4, u16 arg5);

void image_rect_move(s32 arg0, s32 arg1, s32 arg2, s16 arg3, u16 arg4, u16 arg5)
{
    u16 arr[4];
    s32 pad[4];

    arr[0] = *(u16 *)&arg0;
    arr[1] = *(u16 *)&arg1;
    arr[2] = *(u16 *)&arg2;
    arr[3] = arg3;

    MoveImage(arr, arg4, arg5);
}
