/* @SymbolStart: 0x80153280 */
/* @SymbolSize: 160 */
/* @RodataBase: 0x8015B488 */
/* Address: 0x80153280 (cross-prologue head) / 0x80153288 (body) */
/* Size: 160 bytes (40 instructions) */
#include "types.h"

extern s32 work_tarao;

extern void date_girl7_mode2_dispatch();
extern void date_girl7_mode3_dispatch();
extern void date_girl7_mode4_dispatch();
extern void date_girl7_mode5_dispatch();
extern void date_girl7_mode6_dispatch();
extern void game_reset();

void date_scene_dispatch(void) {
    switch (work_tarao - 2) {
        case 0: date_girl7_mode2_dispatch(); break;
        case 1: date_girl7_mode3_dispatch(); break;
        case 2: date_girl7_mode4_dispatch(); break;
        case 3: date_girl7_mode5_dispatch(); break;
        case 4: date_girl7_mode6_dispatch(); break;
        default: game_reset(); break;
    }
}
