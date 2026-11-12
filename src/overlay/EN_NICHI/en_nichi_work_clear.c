/* @CompileEnv: IDO52_NO_LOOP_TEST_REPLACEMENT=bb:2 */
/* @SymbolSize: 216 */
/* en_nichi_work_clear - EN_NICHI overlay, Address: 0x8013275C, Size: 216 bytes (54 insns) */
#include "types.h"

extern void en_nichi_obj_spawn(s32, s32);
extern void en_nichi_grid_clear(void);

extern s32 en_nichi_marker_timer[11];
extern s32 en_nichi_poi_wear;
extern s32 en_nichi_action_step;
extern s32 en_nichi_action_timer;
extern s32 en_nichi_get_cnt;
extern s32 en_nichi_rare_cnt;
u8  en_nichi_near_tbl[10] = {0};
extern s32 en_nichi_konami_cnt;
extern s32 en_nichi_fade_lv;

void en_nichi_work_clear(void)
{
    s32 s0;
    s32 *v0;
    s32 i;

    s0 = 0;
    do {
        en_nichi_obj_spawn(s0, 0);
        s0++;
    } while (s0 < 10);

    en_nichi_grid_clear();

    en_nichi_marker_timer[10] = 0;
    for (v0 = en_nichi_marker_timer; v0 < &en_nichi_marker_timer[10]; v0++) {
        *v0 = 0;
    }

    en_nichi_fade_lv = 0;
    en_nichi_poi_wear = 0;
    en_nichi_action_step = 0;
    en_nichi_action_timer = 0;
    en_nichi_get_cnt = 0;
    en_nichi_rare_cnt = 0;

    for (i = 0; i < 10; i++) {
        en_nichi_near_tbl[i] = 0;
    }

    en_nichi_konami_cnt = 0;
}
