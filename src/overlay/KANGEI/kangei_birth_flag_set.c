/* @SymbolSize: 40 */
/* kangei_birth_flag_set - KANGEI overlay, Address: 0x80138264, Size: 40 bytes (10 insns) */
#include "types.h"

extern u8 kangei_birth_flag;
extern void sub_sub_step_up(void);

void kangei_birth_flag_set(void)
{
    kangei_birth_flag = 1;
    sub_sub_step_up();
}
