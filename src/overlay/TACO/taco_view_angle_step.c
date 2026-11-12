/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_COPYPROP_VAR=-4 IDO52_UGEN_TEMP_REG=3:t8,5:t0,6:t1,7:t4,8:t5,9:t3,10:t6,12:t8 */
/* Address: 0x80143EF0, Size: 180 bytes (45 insns) */
#include "types.h"

extern volatile s16 taco_grid_ofs;

s16 taco_grid_ofs_2[2] = {0, 0};

s32 taco_view_angle_step()
{
    s32 a, b;

    a = (s16)(taco_grid_ofs_2[0] + 128);
    if (a >= 4097) {
        a = (s16)(a - 4096);
        taco_grid_ofs_2[1] = taco_grid_ofs_2[1] + 1;
    }
    taco_grid_ofs_2[0] = a;
    if (a < 0) {
        taco_grid_ofs_2[1] = taco_grid_ofs_2[1] - 1;
        taco_grid_ofs_2[0] = a + 4096;
    }

    b = taco_grid_ofs;
    if (b >= 2049) {
        b = (s16)(b - 2048);
    }
    taco_grid_ofs = b;
    if (b < 0) {
        b = (s16)(b + 2048);
        taco_grid_ofs = b;
    }

}
