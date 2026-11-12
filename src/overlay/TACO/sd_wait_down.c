/* @CompileFlags: -Wo,-loopunroll,0 */
/* Address: 0x80133694 */
/* Size: 164 bytes (41 insns) */
#include "types.h"

extern u8 taco_sd_wait[];

void sd_wait_down(void)
{
    u8 *p;
    u8 *e;
    s32 v;

    v = taco_sd_wait[0];
    if (v != 0) {
        taco_sd_wait[0] = v - 1;
    }

    v = taco_sd_wait[1];
    if (v != 0) {
        taco_sd_wait[1] = v - 1;
    }

    p = &taco_sd_wait[2];
    e = &taco_sd_wait[10];
    while (p != e) {
        v = p[0];
        if (v != 0) {
            p[0] = v - 1;
        }
        v = p[1];
        if (v != 0) {
            p[1] = v - 1;
        }
        v = p[2];
        if (v != 0) {
            p[2] = v - 1;
        }
        v = p[3];
        if (v != 0) {
            p[3] = v - 1;
        }
        p += 4;
    }
}
