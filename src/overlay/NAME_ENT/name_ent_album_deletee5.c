/* @SymbolStart: 0x80140108 */
/* @SymbolSize: 140 */
/* name_ent_album_deletee5 - NAME_ENT overlay, Address: 0x80140110, 33 insns. */
#include "types.h"

extern u8  now_album;
extern u8  step_mode;
extern u8  album_status[];
extern u8  name_ent_album_deletee5_str0[];

extern s32  card_file_delete(s32);
extern void printf(u8 *);
extern void goto_sub_sub_step(s32);
extern void card_state_clear(void);
extern void save_flag_clear(void);
extern void goto_sub_step(s32);
extern u8 step_mode;

void name_ent_album_deletee5(void)
{
    if (step_mode != 0) {
        return;
    }

    if (card_file_delete(now_album) == 1) {
        printf(name_ent_album_deletee5_str0);
        album_status[now_album] = 0;
        goto_sub_sub_step(0);
        step_mode = 2;
    } else {
        card_state_clear();
        save_flag_clear();
        goto_sub_step(1);
    }
}
