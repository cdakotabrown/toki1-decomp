/* @SymbolSize: 76 */
/* @PostProc: swap_registers_auto v0:t7 */
/* @PostProc: swap_registers_auto t0:at */
/* kangei_choice_face_set - KANGEI overlay, Address: 0x801387C0, Size: 76 bytes (19 insns) */
#include "types.h"

extern void bustup_face_change(s32 a0);
extern void sub_sub_step_up(void);
extern s32 serifu_no;

void
kangei_choice_face_set(void)
{
    u32 a0 = serifu_no;

    u8 idx;
    s32 val;
    s16 result;

    idx = (u8)(a0 + 1);
    bustup_face_change(idx);
    val = *(s32 *)(0x80130000 - 20036);
    result = (s16)(val * 2 + 13);
    *(s16 *)(0x80140000 - 25876) = result;
    sub_sub_step_up();
}
