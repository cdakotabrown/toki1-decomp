/* @RodataBase: 0x801398C8 */
/* @SymbolSize: 196 */
/* kangei_frame_check_bustup_read - KANGEI overlay, Address: 0x80135B68 */
#include "types.h"

extern void bustup_cd_read(s32);
extern void sub_sub_step_up(void);
extern u8 now_girl;

void kangei_frame_check_bustup_read(void)
{
    u32 idx = now_girl;

    switch (idx) {
    case 0: bustup_cd_read(22343); break;
    case 1: bustup_cd_read(24278); break;
    case 2: bustup_cd_read(21398); break;
    case 3: bustup_cd_read(23108); break;
    case 4: bustup_cd_read(26753); break;
    case 5: bustup_cd_read(24953); break;
    case 6: bustup_cd_read(25583); break;
    default: bustup_cd_read(27338); break;
    }
    sub_sub_step_up();
}
