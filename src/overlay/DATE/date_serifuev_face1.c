/* Address: 0x8014EBDC */
/* Size: 92 bytes (23 instructions) */
#include "types.h"

extern void bustup_face_change(s32 arg);
extern void func_8014C530(void);

extern u16 now_serifuev_in;
extern volatile s32 step_counter;

void date_serifuev_face1(void)
{
    func_8014C530();
    if (now_serifuev_in == 5 && step_counter == 1) {
        bustup_face_change(1);
    }
    step_counter = step_counter + 1;
}
