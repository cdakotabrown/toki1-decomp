/* @SymbolStart: 0x80138A3C */
/* @SymbolSize: 244 */
/* @RodataBase: 0x80139910 */
/* Address: 0x80138A3C (cross-prologue head) / 0x80138A44 (prologue) */
/* Size: 244 bytes (61 instructions), head included */
#include "types.h"

extern void bustup_cd_read(s32);
extern void sub_sub_step_up(void);
extern u8 now_girl;

void
kangei_player_birthday_bustup_read(void)
{
    u32 sel = now_girl;

    switch (sel) {
    case 0:
        bustup_cd_read(21533);
        break;
    case 1:
        bustup_cd_read(23828);
        break;
    case 2:
        bustup_cd_read(20993);
        break;
    case 3:
        bustup_cd_read(22658);
        break;
    case 4:
        bustup_cd_read(26258);
        break;
    case 5:
        bustup_cd_read(24458);
        break;
    case 6:
        bustup_cd_read(25088);
        break;
    case 7:
        bustup_cd_read(23423);
        break;
    case 8:
        bustup_cd_read(20588);
        break;
    case 9:
        bustup_cd_read(25898);
        break;
    default:
        bustup_cd_read(26888);
        break;
    }

    sub_sub_step_up();
}
