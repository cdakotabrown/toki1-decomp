/* @SymbolSize: 64 */
/* Address: 0x8013c65c */
/* Size: 64 bytes (16 insns) */
#include "types.h"

extern s32 serifu_no;
extern u8 kao2[];
extern void bustup_face_change(s32);
extern void sub_sub_step_up(void);

void date_basyo17_kao2_face_step(void)
{
    bustup_face_change(kao2[serifu_no * 2]);
    sub_sub_step_up();
}
