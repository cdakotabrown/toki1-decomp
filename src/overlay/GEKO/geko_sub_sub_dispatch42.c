/* @CompileEnv: IDO52_AS1_NO_LA_PULL=8 IDO52_AS1_NO_FALLTHRU_FILL=0 IDO52_AS1_NO_SCHED_LOAD_HOIST=2 */
/* @PostProc: patch_stack_complete --old 200 --new 216 */
/* @PostProc: remap_stack_slots --from 32 --to 40 */
/* Address: 0x8013B05C, Size: 388 bytes (97 insns) */
/* @PostProc: swap_instructions --offset1 0x94 --offset2 0x9c */
/* @PostProc: swap_instructions --offset1 0x98 --offset2 0x9c */
#include "types.h"

typedef void (*GekoHandler)(s32);

typedef struct {
    GekoHandler f[42];
} GekoHandlerTbl;

extern s32 get_CD_State(void);
extern s32 SD_CDLevelGet(void);

extern u8 voice_level;
extern u8 now_sub_sub_step;
extern u8 cw99_anim_flag;
extern u8 cw99_disp;
extern u8 cw99_bright;
extern s16 cw99_frame;
extern s32 date_sprite_on;
extern u8 fade_bri;
extern u8 geko_voice_latch_flag;

extern GekoHandlerTbl geko_sub_sub_dispatch42_tbl;

void geko_sub_sub_dispatch42(void)
{
    GekoHandlerTbl tbl;
    u32 latched;
    s32 res;

    tbl = geko_sub_sub_dispatch42_tbl;
    tbl.f[now_sub_sub_step](128);

    if (date_sprite_on == 0) {
        goto tail_clear;
    }

    cw99_bright = fade_bri;
    cw99_disp |= 0x80;

    if (voice_level == 0) {
        goto path_idle;
    }

    latched = geko_voice_latch_flag;
    if (latched == 0) {
        cw99_anim_flag = 3;
    }
    if (cw99_frame == 6) {
        if (latched == 0) {
            cw99_anim_flag = 0;
        }
    }

    if (get_CD_State() & 4) {
        res = SD_CDLevelGet();
    } else {
        res = 0;
    }
    if (res != 0) {
        if (geko_voice_latch_flag == 0) {
            cw99_anim_flag = 3;
            geko_voice_latch_flag = 1;
        }
    }
    return;

path_idle:
    if (geko_voice_latch_flag == 0) {
        cw99_anim_flag = 3;
        cw99_frame = 1;
        geko_voice_latch_flag = 1;
    }
    return;

tail_clear:
    cw99_disp &= 0xFF7F;
}
