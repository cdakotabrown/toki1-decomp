/* Function: don_move */
/* Address: 0x800478A0 */
/* Size: 352 bytes (88 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @SymbolStart: 0x800478A0 */
/* @SymbolSize: 352 */
/* @CompileFlags: -Wc,-pic1 -cckr */
/* @CompileEnv: IDO52_AS1_TIE_TAKE=15,17 IDO52_UGEN_TEMP_REG=5:t5,6:t1,7:t2,8:t3,15:t0,16:t1 */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"

extern s32 rsin(s32);

extern s16 disp_ofs_x[];
extern s16 disp_ofs_y[];
extern s16 scr_half_w;
extern s16 scr_half_h;
extern u8 don_timer;

void don_move(void) {
    s32 i;
    s32 v1;
    s32 divResult;
    s16 temp_a2;
    s16 temp_a3;

    if (don_timer != 0) {
        i = 0;
        v1 = 0;
        while ((u32)i < don_timer) {
            v1 += i;
            i++;
        }
        divResult = rsin(v1 << 2);
        divResult = (s16)divResult / 4 / ((int)don_timer + 256);
        don_timer += 4;
        disp_ofs_x[0] = ((divResult << 16) >> 16) + 160;
        temp_a2 = (s16)divResult + 160;
        disp_ofs_x[1] = temp_a2;
        disp_ofs_y[0] = (s16)divResult + 360;
        temp_a3 = (s16)divResult + 120;
        disp_ofs_y[1] = temp_a3;
        scr_half_w = temp_a2;
        scr_half_h = temp_a3;
        if (don_timer >= 129) {
            don_timer = 0;
        }
        v1 = (s16)divResult;
    } else {
        scr_half_w = 160;
        scr_half_h = 120;
        disp_ofs_x[0] = 160;
        disp_ofs_x[1] = 160;
        disp_ofs_y[0] = 360;
        disp_ofs_y[1] = 120;
    }
}
