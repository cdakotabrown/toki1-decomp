/* Address: 0x80061480 */
/* Size: 152 bytes (38 instructions) */
/* Region: Game (0x80041000-0x80085FFF), IDO 5.2 */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:10 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"

extern u32 step_counter;
extern u8 now_sub_sub_step;
extern s32 pre_syogatu_init0(void);
extern s32 pre_syogatu_init1(void);
extern s32 pre_syogatu_init2(void);
extern s32 pre_syogatu_init3(void);

s32 pre_syogatu_init(void) {
    step_counter++;
    {
        s32 mode = now_sub_sub_step;
        s32 ret = mode;

        switch (mode) {
        case 0:
            ret = pre_syogatu_init0();
            break;
        case 1:
            ret = pre_syogatu_init1();
            break;
        case 2:
            ret = pre_syogatu_init2();
            break;
        case 3:
            ret = pre_syogatu_init3();
            break;
        }
        return ret;
    }
}
