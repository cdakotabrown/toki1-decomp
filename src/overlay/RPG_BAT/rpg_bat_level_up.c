/* @CompileEnv: IDO52_FORCE_WEB_COLOR=4:v1,5:a0,11:a1,30:a2,31:a3,0:a0 */
/* @SymbolStart: 0x80153A60 */
/* @SymbolSize: 168 */
#include "types.h"
#include "tokimeki/class_w.h"

extern s32 rpg_bat_seq_work;
extern s32 rpg_bat_mode;
extern volatile u8 screen_bri;
extern u8 class_w[];

extern s32 rpg_bat_anim_set();

void rpg_bat_level_up(void)
{
    s32 i;

    for (i = 0; i < 96; i++) {
        class_w[i * 68 + 4359] = rpg_bat_seq_work;
        screen_bri = rpg_bat_seq_work;
        if (rpg_bat_mode == 0x20000 && rpg_bat_seq_work >= 97) {
            ((CLASS_W *)class_w)[127].bright = 96;
        }
    }

    rpg_bat_anim_set(31, 0, 1, 0);
    rpg_bat_seq_work += 2;
    if (rpg_bat_seq_work >= 129) {
        rpg_bat_seq_work = 128;
    }
}
