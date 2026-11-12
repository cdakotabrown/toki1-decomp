/* Address: 0x8015600C */
/* Size: 92 bytes (23 instructions) */
#include "types.h"

extern void bustup_face_change(s32 arg);
extern void func_8014C530(void);

extern u16 now_serifuev_in;
extern volatile s32 step_counter;

void date_girl5_serifu_face(void)
{
    func_8014C530();
    if (now_serifuev_in == 2 && step_counter == 1) {
        bustup_face_change(1);
    }
    step_counter = step_counter + 1;
}
