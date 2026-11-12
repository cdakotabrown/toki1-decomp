/* @SymbolStart: 0x80132FCC */
/* @SymbolSize: 244 */
/* @RodataBase: 0x80143668 */
/* Address: 0x80132FD4 */
/* Size: 236 bytes (59 instructions) */
#include "types.h"

extern void sub_sub_step_up(void);
extern void bustup_cd_read(s32);
extern u8 now_girl;

void bustup_cd_read_girl_step(void)
{
    switch (now_girl) {
        case 0:
            bustup_cd_read(22298);
            break;
        case 1:
            bustup_cd_read(24098);
            break;
        case 2:
            bustup_cd_read(21263);
            break;
        case 3:
            bustup_cd_read(22928);
            break;
        case 4:
            bustup_cd_read(26528);
            break;
        case 5:
            bustup_cd_read(24908);
            break;
        case 6:
            bustup_cd_read(25358);
            break;
        case 7:
            bustup_cd_read(23513);
            break;
        case 8:
            bustup_cd_read(20678);
            break;
        case 9:
            bustup_cd_read(25988);
            break;
        case 10:
            bustup_cd_read(27158);
            break;
        default:
            break;
    }
    sub_sub_step_up();
}
