/* name_ent_album_restore - NAME_ENT overlay, Address: 0x8013D980, Size: 128 bytes (32 insns) */
#include "types.h"

extern s32 save_game_restore(s32);
extern void name_ent_card_io_noop(void);
extern void printf(u8 *);
extern void card_state_clear(void);
extern void device_cmd_send(s32);
extern void goto_sub_step(s32);
extern void save_flag_clear(void);
extern void name_ent_status_disp(void);

extern u8 name_ent_load24_str0[];
extern u8 name_ent_load24_str1[];

void name_ent_album_restore(void)
{
    if (save_game_restore(0) == 1) {
        name_ent_card_io_noop();
        printf(name_ent_load24_str0);
        card_state_clear();
        device_cmd_send(0x501);
        goto_sub_step(3);
    } else {
        printf(name_ent_load24_str1);
        save_flag_clear();
        goto_sub_step(1);
    }
    name_ent_status_disp();
}
