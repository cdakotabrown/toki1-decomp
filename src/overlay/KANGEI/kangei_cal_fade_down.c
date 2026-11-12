/* @CompileEnv: IDO52_FORCE_WEB_COLOR=2:v0,5:v1 IDO52_AS1_NO_FALLTHRU_FILL=occ:1 */
/* @SymbolSize: 104 */
/* Address: 0x80135C84, Size: 104 bytes (26 instructions) */
#include "types.h"

extern void fade_down_main(void);

extern u8 now_girl;
extern u8 bustup2_cw_bright;
extern u8 cal_cw_disp;
extern u8 cal_cw_bright;

void kangei_cal_fade_down(void)
{
    u8 cur_char;
    u8 byte_val;

    fade_down_main();
    cur_char = now_girl;
    if (cur_char == 10 || cur_char == 3) {
        byte_val = bustup2_cw_bright;
        cal_cw_bright = byte_val;
        if ((u32)byte_val < 8) {
            cal_cw_disp = cal_cw_disp & 0x7F;
        }
    }
}
