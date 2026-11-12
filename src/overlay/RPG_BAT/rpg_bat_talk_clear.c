/* Size: 48 bytes */
/* @SymbolSize: 48 */
#include "types.h"

typedef struct {
    s32 a;
    s32 b;
    s32 c;
    s32 d;
} Quad;

extern Quad rpg_bat_talk_work[3];

void rpg_bat_talk_clear(void)
{
    s32 i;

    for (i = 0; i < 3; i++) {
        rpg_bat_talk_work[i].a = 0;
        rpg_bat_talk_work[i].b = 0;
        rpg_bat_talk_work[i].c = 0;
        rpg_bat_talk_work[i].d = 0;
    }
}
