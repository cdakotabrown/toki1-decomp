/* Address: 0x8014F014 */
#include "types.h"

extern u8 *taco_actor_rec;

extern s32 rand(void);

void taco_action_set(void)
{
    s32 r;
    s32 m;
    s32 one;

    one = 1;
    r = rand();
    if (taco_actor_rec[2314] == one) {
        m = r % 100;
        if (m < 20) {
            taco_actor_rec[2310] = 0;
        } else if (m >= 40 && m < 55) {
            if (taco_actor_rec[2989] != 0 || taco_actor_rec[3533] != 0) {
                taco_actor_rec[2310] = one;
            } else {
                taco_actor_rec[2310] = 7;
            }
        } else if (m >= 55 && m < 70) {
            taco_actor_rec[2310] = 2;
        } else if (m >= 70 && m < 90) {
            taco_actor_rec[2310] = 3;
        } else if (m >= 20 && m < 40) {
            taco_actor_rec[2310] = 4;
        } else if (m >= 90) {
            taco_actor_rec[2310] = 7;
        }
    } else if (taco_actor_rec[2314] == 29) {
        m = r % 100;
        if (m >= 0 && m < 40) {
            taco_actor_rec[2310] = 2;
        } else if (m >= 40 && m < 70) {
            taco_actor_rec[2310] = 5;
        } else if (m >= 70 && m < 95) {
            taco_actor_rec[2310] = 6;
        } else if (m >= 95) {
            taco_actor_rec[2310] = 8;
        }
    }
}
