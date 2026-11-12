/* @CompileEnv: IDO52_FORCE_WEB_COLOR=40:s3,37:s4,39:s5 */
#include "types.h"

extern s32 tt_rec_target_claim();
extern s32 tt_rec_timer(void *);
extern s32 tt_recs_move(void *);

extern u8 *tt_recs_buf;

void tt_recs_step(void)
{
    u8 *p;
    s32 i;

    p = tt_recs_buf;
    i = 0;
    do {
        if (*(u8 *)(p + 0) != 0) {
            switch (*(u16 *)(p + 4)) {
            case 32:
                tt_rec_target_claim(p, i);
                *(s16 *)(p + 4) = 64;
                *(u16 *)(p + 10) = 0;
                *(u16 *)(p + 8) = 0;
                *(u16 *)(p + 6) = 0;
                *(u8 *)(p + 15) = 0;
                *(u8 *)(p + 14) = 0;
                *(u8 *)(p + 13) = 0;
                break;
            case 64:
                tt_rec_timer(p);
                break;
            case 128:
                *(u8 *)(p + 0) = 0;
                break;
            }
            tt_recs_move(p);
        }
        i++;
        p += 88;
    } while (i != 8);
}
