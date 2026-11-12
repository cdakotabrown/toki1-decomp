/* Address: 0x801534B0 */
#include "types.h"

extern u8 *taco_actor_rec;

extern s32 rand(void);

void taco_reel_result_set(s32 sel)
{
    s32 r;
    s32 m;

    r = rand();
    if (sel == 1) {
        if (taco_actor_rec[2446] != 0) {
            taco_actor_rec[2582] = 0;
        } else {
            m = r % 100;
            if (m < 50) {
                taco_actor_rec[2582] = 1;
            } else if (m >= 50 && m < 70) {
                taco_actor_rec[2582] = 2;
            } else if (m >= 70) {
                taco_actor_rec[2582] = 3;
            }
        }
    } else if (sel == 2) {
        if (taco_actor_rec[2446] != 0) {
            taco_actor_rec[2718] = 0;
        } else {
            m = r % 100;
            if (m < 50) {
                taco_actor_rec[2718] = 1;
            } else if (m >= 50 && m < 70) {
                taco_actor_rec[2718] = 2;
            } else if (m >= 70) {
                taco_actor_rec[2718] = 3;
            }
        }
    } else if (sel == 3) {
        if (taco_actor_rec[2446] != 0) {
            taco_actor_rec[2854] = 0;
        } else {
            m = r % 100;
            if (m < 50) {
                taco_actor_rec[2854] = 1;
            } else if (m >= 50 && m < 70) {
                taco_actor_rec[2854] = 2;
            } else if (m >= 70) {
                taco_actor_rec[2854] = 3;
            }
        }
    }
}
