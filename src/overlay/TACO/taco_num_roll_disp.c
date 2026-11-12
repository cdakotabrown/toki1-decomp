/* @SymbolSize: 456 */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:4 IDO52_AS1_NO_PASS1_PROP=12 IDO52_UGEN_TEMP_REG=1:t5 */
/* Address: 0x80138944 */
#include "types.h"

extern s32 frame_counter;
extern s32 taco_num_roll_goal;
extern s32 taco_num_roll_cur;

extern s32 taco_num_disp();

s32
taco_num_roll_disp(s32 a0, s32 a1, s32 a2, s32 a3)
{
    s32 v0;

    if (a3 == 0) {
        taco_num_roll_goal = a0;
        taco_num_roll_cur = 0;
        return a0;
    }
    v0 = taco_num_roll_goal;
    if (taco_num_roll_cur < v0) {
        if ((frame_counter & 1) == 0) {
            if (taco_num_roll_cur < v0 % 10) {
                taco_num_roll_cur += 1;
            } else if (taco_num_roll_cur < v0 % 100) {
                taco_num_roll_cur += 10;
            } else if (taco_num_roll_cur < v0 % 1000) {
                taco_num_roll_cur += 100;
            } else if (taco_num_roll_cur < v0 % 10000) {
                taco_num_roll_cur += 1000;
            } else if (taco_num_roll_cur < v0 % 100000) {
                taco_num_roll_cur += 10000;
            } else if (taco_num_roll_cur < v0 % 1000000) {
                taco_num_roll_cur += 100000;
            } else if (taco_num_roll_cur < v0 % 10000000) {
                taco_num_roll_cur += 1000000;
            }
        }
    }
    if (v0 < taco_num_roll_cur) {
        taco_num_roll_cur = v0;
    }
    taco_num_disp(taco_num_roll_cur, a1, a2, 0);
    return taco_num_roll_cur;
}
