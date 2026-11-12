/* @CompileFlags: -cckr */
#include "types.h"

typedef struct {
    u8 pad[19];
    u8 flag;
} Slot;

extern s32 taco_burst_flag;
extern s32 taco_burst_cnt;
extern Slot taco_effect_buf[16];
extern Slot taco_effect2_buf[16];
extern Slot taco_actor_buf[16];
extern Slot taco_actor2_buf[16];
extern Slot taco_throw_tbl[16];

void taco_sprite_init()
{
    s32 i;

    taco_burst_flag = 0;
    taco_burst_cnt = 0; for (i = 0; i < 16; i++) {
        taco_effect_buf[i].flag = 0;
        taco_effect2_buf[i].flag = 0;
        taco_actor_buf[i].flag = 0;
        taco_actor2_buf[i].flag = 0;
        taco_throw_tbl[i].flag = 0;
    }
}
