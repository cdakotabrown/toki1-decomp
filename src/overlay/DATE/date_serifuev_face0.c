/* @SymbolSize: 108 */
/* Function: date_serifuev_face0 */
/* Overlay: DATE */
/* Address: 0x80152C3C */
/* Size: 108 bytes (27 instructions) */
#include "types.h"

extern void func_8014C530(void);
extern void bustup_face_change(s32 a);
extern u16 now_serifuev_in;
extern s32 step_counter;
extern s32 serifu_no;
void date_serifuev_face0(void)
{
  func_8014C530();
  if (((now_serifuev_in == 7) && (step_counter == 1)) && (serifu_no == (1 & 0xFFu)))
  {
    bustup_face_change(0);
  }
  step_counter = step_counter + 1;
}
