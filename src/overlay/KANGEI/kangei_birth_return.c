/* kangei_birth_return - KANGEI overlay, Address: 0x8013812C, Size: 44 bytes (11 insns) */
#include "types.h"

extern u8 kangei_birth_done;
extern void goto_sub_sub_step(s32);

void kangei_birth_return(void)
{
    kangei_birth_done = 1;
    goto_sub_sub_step(0);
}
