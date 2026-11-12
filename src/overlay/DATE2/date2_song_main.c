/* @SymbolStart: 0x8013850C */
/* @SymbolSize: 96 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v0:v1 0x14:rs:v0:v1 0x14:rt:v0:v1 0x1c:rs:v0:v1 0x28:rs:v0:v1 */
/* Address: 0x80138514 */
#include "types.h"

extern s32 step_counter;

void date2_song_menu_step(void);
void date2_xa_main(void);
void game_reset(void);

void
date2_song_main(void)
{
    s32 v1_in;

    v1_in = step_counter;
    switch (v1_in) {
    case 0:
        date2_song_menu_step();
        break;
    case 1:
        date2_xa_main();
        break;
    default:
        game_reset();
        break;
    }
}
