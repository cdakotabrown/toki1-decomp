/* Function: week_day_init */
/* Address: 0x800739D4 */
/* Size: 128 bytes (32 instructions) */
/* Region: Game (0x80041000-0x80085FFF), IDO 5.2 */
/* @SymbolStart: 0x800739D4 */
/* @SymbolSize: 128 */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:10 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"

extern u8 now_sub_sub_step;
extern u32 step_counter;
extern s32 week_day_init0(void);
extern s32 week_day_load(void);
extern s32 week_day_bustup_init(void);

s32 week_day_init(void) {
    step_counter++;
    {
        s32 mode = now_sub_sub_step;
        s32 ret = mode;

        switch (mode) {
        case 0:
            ret = week_day_init0();
            break;
        case 1:
            ret = week_day_load();
            break;
        case 2:
            ret = week_day_bustup_init();
            break;
        }
        return ret;
    }
}
