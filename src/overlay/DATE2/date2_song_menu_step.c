/* @SymbolStart: 0x8013856C */
/* @SymbolSize: 96 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v0:v1 0x14:rs:v0:v1 0x14:rt:v0:v1 0x1c:rs:v0:v1 0x28:rs:v0:v1 */
/* Address: 0x80138574, Size: 88 bytes (22 insns) */
#include "types.h"

extern u8 step_mode;

void date2_song_menu_init(void);
void date2_menu_select(void);
void game_reset(void);

void date2_song_menu_step(void)
{
    u8 a0;

    a0 = step_mode;
    switch (a0) {
        case 0:
            date2_song_menu_init();
            break;
        case 1:
            date2_menu_select();
            break;
        default:
            game_reset();
            break;
    }
}
