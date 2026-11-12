/* @SymbolStart: 0x8013F13C */
/* @RodataBase: 0x80143A78 */
/* Address: 0x8013F144 */
/* Size: 272 bytes (68 instructions) */
/* @SymbolSize: 280 */
#include "types.h"

extern void bustup_cd_read(s32);
extern void sub_sub_step_up(void);

extern u8 shougatu_girl9_appear;
extern u8 now_girl;

void bustup_cd_read_girl_step2(void)
{
    switch (now_girl) {
    case 0:
        bustup_cd_read(22208);
        break;
    case 1:
        bustup_cd_read(24188);
        break;
    case 2:
        bustup_cd_read(21353);
        break;
    case 3:
        bustup_cd_read(23018);
        break;
    case 4:
        bustup_cd_read(26663);
        break;
    case 5:
        bustup_cd_read(24863);
        break;
    case 6:
        bustup_cd_read(25493);
        break;
    case 7:
        bustup_cd_read(23648);
        break;
    case 8:
        bustup_cd_read(20903);
        break;
    case 9:
        if (shougatu_girl9_appear != 0) {
            bustup_cd_read(26168);
        } else {
            bustup_cd_read(26078);
        }
        break;
    case 10:
        bustup_cd_read(27293);
        break;
    }

    sub_sub_step_up();
}
