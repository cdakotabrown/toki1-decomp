/* @RodataBase: 0x801398A0 */
/* @SymbolSize: 196 */
#include "types.h"

extern void bustup_cd_read(s32);
extern void sub_sub_step_up(void);
extern u8 now_girl;

void kangei_switch2_bustup_read(void)
{
    u32 idx = now_girl;

    switch (idx) {
    case 0: bustup_cd_read(21983); break;
    case 1: bustup_cd_read(24323); break;
    case 2: bustup_cd_read(21443); break;
    case 3: bustup_cd_read(23153); break;
    case 4: bustup_cd_read(26798); break;
    case 5: bustup_cd_read(24998); break;
    case 6: bustup_cd_read(25628); break;
    default: bustup_cd_read(27068); break;
    }
    sub_sub_step_up();
}
