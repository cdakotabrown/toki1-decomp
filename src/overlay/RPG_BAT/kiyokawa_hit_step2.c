/* @SymbolStart: 0x80150B90 */
/* @SymbolSize: 328 */
/* @CompileEnv: IDO52_COPYPROP_VAR=-8 IDO52_AS1_PEEPREG_KEEP=0 */
/* Address: 0x80150B98 (window starts at the absorbed cross-prologue head */
/* Size: 328 bytes (82 insns) including the head */
#include "types.h"

extern s32 rpg_bat_anim_set();
extern s32 rpg_bat_anim_set2();
extern s32 rpg_bat_sprite_pos_set();
extern s32 rpg_bat_se_set2();

extern s32 rpg_bat_seq_work;
extern s32 rpg_bat_seq_wait_cnt;

s32 kiyokawa_hit_step2(void)
{
    s32 step;

    step = rpg_bat_seq_work;
    switch (step) {
    case 0:
        rpg_bat_anim_set2(50, 6, 5, 1);
        rpg_bat_seq_work += 1;
        break;
    case 1:
        rpg_bat_seq_wait_cnt += 1;
        if (rpg_bat_seq_wait_cnt >= 15)
            rpg_bat_seq_work += 1;
        break;
    case 2:
        rpg_bat_anim_set(52, 12, 1, 1);
        rpg_bat_sprite_pos_set(51, 0, 0);
        rpg_bat_anim_set(51, 8, 1, 1);
        rpg_bat_se_set2(1299, 0, 5);
        rpg_bat_seq_work += 1;
        break;
    case 3:
        rpg_bat_seq_wait_cnt += 1;
        if (rpg_bat_seq_wait_cnt >= 31) {
            rpg_bat_anim_set(51, 8, 1, 0);
            rpg_bat_seq_work += 1;
        }
        break;
    }
}
