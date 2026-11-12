/* @SymbolStart: 0x8013C72C */
/* @SymbolSize: 228 */
#include "types.h"

extern int abs(int arg);
extern void xy_rotate(int theta, int *x, int *y, int *out_x, int *out_y);

extern short cw97_x;
extern int   taiiku_course_no;
extern short taiiku_tama_sx;
extern s32 taiiku_tama_vel[];
int taiiku_tama_gpush[2] = {0, 0};

void taiiku_cw97_steer_set(int arg0) {
    int v0;
    int t5;
    int a;
    int x;
    int y;
    int ox;
    int oy;

    v0 = abs((taiiku_tama_sx >> 3) - cw97_x);
    if (v0 < 80) {
        if ((*(int *)&taiiku_tama_vel[1]) < 0) {
            (*(int *)&taiiku_tama_vel[1]) = 0;
        }
        t5 = taiiku_course_no;
        v0 = -((int)(((taiiku_tama_sx >> 3) - cw97_x) << 10) >> 6);
        if ((t5 % 3) != 0) {
            v0 -= 256;
        }
        x = 0;
        a = arg0;
        y = a;
        xy_rotate(v0, &x, &y, &ox, &oy);
        taiiku_tama_gpush[0] = ox;
        taiiku_tama_gpush[1] = oy;
    } else {
        taiiku_tama_gpush[0] = 0;
        taiiku_tama_gpush[1] = 0;
    }
}
