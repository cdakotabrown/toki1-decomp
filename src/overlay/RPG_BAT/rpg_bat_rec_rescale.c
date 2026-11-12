/* @SymbolSize: 232 */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=12:a0 */
#include "types.h"

extern s32 rpg_bat_anim_set2();
extern s32 rpg_bat_tgt_x;

typedef struct {
    u8 pad0[26];
    s16 f26;
    s16 f28;
    u8 pad1[8];
    s16 f38;
    u8 pad2[2];
    s16 f42;
    u8 pad3[24];
} BatRec;

extern BatRec class_w[];

void rpg_bat_rec_rescale()
{
    int i;
    int x;
    int result;

    for (i = 0; i < 8; i++) {
        if (class_w[i + 136].f38 < -180 || class_w[i + 136].f42 >= 141) {
            rpg_bat_anim_set2(i + 40, 7, 1, 0);
        }
        x = class_w[i + 136].f38;
        result = ((112 - x) << 12) / 240;
        class_w[i + 136].f26 = result;
        class_w[i + 136].f28 = result;
        rpg_bat_tgt_x = x;
    }
}
