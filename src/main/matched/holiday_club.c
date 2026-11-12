/* Address: 0x8007892C, Size: 132 bytes (33 instructions) */
/* @SymbolStart: 0x8007892C */
/* @SymbolSize: 132 */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:6 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"
extern u8 now_sub_sub_step;
extern s32 holiday_club_init(void);
extern s32 holiday_club_select(void);
extern s32 holiday_club_exit(void);
extern s32 holiday_taibu_club_exit(void);

s32 holiday_club(void) {
    s32 mode;
    s32 ret;

    mode = now_sub_sub_step;
    ret = mode;
    switch (mode) {
    case 0:
        ret = holiday_club_init();
        break;
    case 1:
        ret = holiday_club_select();
        break;
    case 2:
        ret = holiday_club_exit();
        break;
    case 3:
        ret = holiday_taibu_club_exit();
        break;
    }
    return ret;
}
