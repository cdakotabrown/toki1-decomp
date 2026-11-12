/* @CompileEnv: IDO52_CONST_NOINREG_VALUES=2,3,64,168,1024 IDO52_FORCE_WEB_COLOR=2:s5,7:s6,49:s7,71:s8 */
/* @CompileFlags: -cckr */
#include "types.h"

extern char *tt_shot_buf;

void tt_shot_fan_make(short x, short y, int spread, int speed, unsigned short kind)
{
    char *p;
    int i;
    int ang;
    int px;
    int py;

    px = x;
    py = y;
    p = tt_shot_buf;
    for (i = 0; i < 3; i++) {
        p = (char *)get_tt_shot_free(p, p + 13312);
        if (p == 0) {
            return;
        }
        *(int *)(p + 32) = px << 16;
        *(int *)(p + 36) = py << 16;
        ang = 1024;
        switch (i) {
        case 1:
            ang = 1024 - spread;
            break;
        case 2:
            ang = spread + 1024;
            break;
        }
        *(int *)(p + 40) = rcos(ang) * speed;
        *(int *)(p + 44) = rsin(ang) * speed;
        if (kind == 168) {
            tt_shot_sprite_set(p, 168, 21);
        }
        *(short *)(p + 4) = 64;
        *(short *)(p + 10) = 0;
        *(short *)(p + 8) = 0;
        *(short *)(p + 6) = 0;
        p[15] = 0;
        p[14] = 0;
        p[13] = 0;
    }
}
