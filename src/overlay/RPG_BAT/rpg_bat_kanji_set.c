/* @SymbolSize: 72 */
/* rpg_bat_kanji_set - RPG_BAT overlay, Address: 0x8013E934, Size: 72 bytes (18 insns) */
#include "types.h"

extern void set_kanji_string();

void rpg_bat_kanji_set(s32 a0, s32 a1, s32 a2)
{
    set_kanji_string((s16)(a0 - 160), (s16)(a1 - 120), 0, a2, 0);
}
