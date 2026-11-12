/* Address: 0x80074D68  (real entry 0x80074D5C: the two hoisted global */
/* Size: 128 bytes (32 instructions) over the widened window */
/* Region: Game (0x80041000-0x80085FFF) */
/* @SymbolStart: 0x80074D5C */
/* @SymbolSize: 128 */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:0 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"

extern u8 now_sub_sub_step;
extern u32 step_counter;
extern s32 vacation_day_screen_init(void);
extern s32 vacation_day_bustup_read(void);
extern s32 vacation_day_bustup_init(void);

s32 vacation_day_init(void) {
    s32 mode;
    s32 ret;

    ret = now_sub_sub_step;
    step_counter = step_counter + 1;
    mode = ret;
    switch (mode) {
    case 0:
        ret = vacation_day_screen_init();
        break;
    case 1:
        ret = vacation_day_bustup_read();
        break;
    case 2:
        ret = vacation_day_bustup_init();
        break;
    }
    return ret;
}
