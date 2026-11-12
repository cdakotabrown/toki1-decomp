/* @CompileFlags: -cckr */
#include "types.h"

extern unsigned char *taco_actor_rec;
extern int taco_rest_key_tbl[];
extern unsigned char taco_rest_mode_tbl[];

void taco_step_timer(timer, step, kind, arg3, arg4)
unsigned char *timer;
unsigned char *step;
int kind;
int arg3;
unsigned int arg4;
{
    int i;
    unsigned int v;

    v = *step;
    if (v < 12 || v >= 22) {
        if (arg4 < *timer) {
            taco_step_pose_set(kind, arg3, v);
            *step = *step + 1;
            *timer = 0;
            v = *step;
        }
    } else if (v >= 12 && v < 22) {
        if (*timer != 0) {
            taco_step_pose_set(kind, arg3, v);
            *step = *step + 1;
            *timer = 0;
            v = *step & 0xff;
        }
    }
    if (v >= 50) {
        *step = 0;
        *timer = 0;
        if (kind == 9) {
            taco_actor_rec[2582] = 0;
        } else {
            taco_actor_rec[2718] = 0;
        }
        for (i = kind; i < kind + 3; i++) {
            node_pose_set(i, 1, taco_rest_key_tbl[(i - 1) * 2], taco_rest_key_tbl[(i - 1) * 2 + 1],
                          taco_rest_mode_tbl[(i - 1) * 2], taco_rest_mode_tbl[(i - 1) * 2 + 1]);
        }
    } else {
        *timer = *timer + 1;
    }
}
