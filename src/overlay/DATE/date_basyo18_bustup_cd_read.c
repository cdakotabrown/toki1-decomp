/* @SymbolStart: 0x8013C040 */
/* @SymbolSize: 236 */
/* @RodataBase: 0x8015AFA8 */
#include "types.h"

extern u8 now_girl;

extern void bustup_cd_read(s32 a0);

void date_basyo18_bustup_cd_read(void) {
    switch (now_girl) {
    case 0:
        bustup_cd_read(22253);
        break;
    case 1:
        bustup_cd_read(24233);
        break;
    case 2:
        bustup_cd_read(21173);
        break;
    case 3:
        bustup_cd_read(23063);
        break;
    case 4:
        bustup_cd_read(26708);
        break;
    case 5:
        bustup_cd_read(24638);
        break;
    case 6:
        bustup_cd_read(25538);
        break;
    case 7:
        bustup_cd_read(23738);
        break;
    case 8:
        bustup_cd_read(20588);
        break;
    case 9:
        bustup_cd_read(26123);
        break;
    case 10:
        bustup_cd_read(27068);
        break;
    }
}
