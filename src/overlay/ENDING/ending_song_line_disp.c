/* @SymbolStart: 0x8013BE08 */
/* @SymbolSize: 140 */
/* @CompileFlags: -cckr */
#include "types.h"

extern s32 strlen();
extern s32 set_kanji_string();
extern void k_disp_start();

extern u8 ending_song_line_tbl[];

void ending_song_line_disp(idx)
    s16 idx;
{
    u8 *s;

    s = &ending_song_line_tbl[idx * 25];
    k_disp_start(set_kanji_string((s16)(-strlen(s) * 7 / 2), 80, 0, s, 0));
}
