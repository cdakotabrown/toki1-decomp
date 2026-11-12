/* kangei_girl6_wait - KANGEI overlay, Address: 0x80135190, Size: 56 bytes (14 insns) */
#include "types.h"

extern void step_wait128(void);
extern void sub_sub_step_up(void);
extern u8 now_girl;

void kangei_girl6_wait(void)
{
    s32 t6 = now_girl;

    if (t6 == 6) {
        step_wait128();
    } else {
        sub_sub_step_up();
    }
}
