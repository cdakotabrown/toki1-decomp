/* @SymbolSize: 76 */
/* Function: date_serifuev_in7_face1 */
/* Overlay: DATE */
/* Address: 0x80153EFC */
#include "types.h"

extern void bustup_face_change(s32 a0);
extern void func_8014C530(void);
extern s32 step_counter;
extern u16 now_serifuev_in;

void date_serifuev_in7_face1(void)
{
    func_8014C530();
    if (step_counter != 1) return;
    if (now_serifuev_in != 7) return;
    bustup_face_change(1);
}
