/* Address: 0x8007E960 */
/* Size: 104 bytes (26 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: swap_registers_liveness --address 0x8007E960 */
/* @CompileEnv: IDO52_AS1_NO_COPY_PROP=all IDO52_AS1_NO_LA_PULL=all IDO52_AS1_NO_FALLTHRU_FILL=1 */
#include "types.h"

extern u8 fade_bri;
extern s32 fade_flag;
extern void sub_sub_step_up(void);

void fade_in_step(void) {
    u32 a;
    u32 v;

    a = ((u32)fade_bri + 4) & 0xFF;
    fade_flag = 1;
    v = a;
    if (a >= 128U) {
        v = 128;
    }
    fade_bri = (u8)v;
    if (v >= 125U) {
        v = 128;
        fade_bri = (u8)v;
        sub_sub_step_up();
    }
}
