/* @SymbolStart: 0x8013C600 */
/* @SymbolSize: 92 */
/* Address: 0x8013C600 (cross-prologue head) / 0x8013C608 (body) */
/* Size: 92 bytes (23 instructions) */
#include "types.h"

extern u8 machiawase_flag;
extern s32 serifu_no;
extern u8 D_800D0000[];

extern void bustup_face_change(s32 a0);
extern void sub_sub_step_up(void);

void date_kao2_face_step(void)
{
    if (machiawase_flag != 0) {
        bustup_face_change(1);
    } else {
        s32 idx = serifu_no;
        bustup_face_change(D_800D0000[idx * 2 + 1828]);
    }
    sub_sub_step_up();
}
