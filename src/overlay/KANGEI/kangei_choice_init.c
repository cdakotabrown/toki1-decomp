/* kangei_choice_init - KANGEI overlay, Address: 0x80136A78, Size: 328 bytes */
#include "types.h"

extern void srand(s32);
extern s32 rand(void);
extern void sub_sub_step_up(void);

extern s32 frame_counter;
extern u8 now_girl;
extern s16 kangei_now_s;
extern s16 kangei_now_s_in;
extern u8 kangei_choice_tbl[];
extern s32 kangei_choice_timer;

void kangei_choice_init(void)
{
    s32 rnd;

    srand(frame_counter);

    now_girl = 14;
    kangei_choice_timer = 0;
    kangei_now_s = 1;
    kangei_now_s_in = 0;
    srand(frame_counter);

    rnd = rand();
    kangei_choice_tbl[0] = rnd % 2 + 1;

    rnd = rand();
    kangei_choice_tbl[1] = rnd % 2 + 1;

    rnd = rand();
    kangei_choice_tbl[2] = rnd % 2 + 1;

    rnd = rand();
    if (rnd % 2) {
        kangei_choice_tbl[0] = 0;
    }

    rnd = rand();
    if (rnd % 2) {
        kangei_choice_tbl[1] = 0;
    }

    rnd = rand();
    if (rnd % 2) {
        kangei_choice_tbl[2] = 0;
    }

    sub_sub_step_up();
}
