/* @SymbolStart: 0x8013EB78 */
/* @RodataBase: 0x8014C1DC */
/* @SymbolSize: 496 */
/* Address: 0x8013EB80, Size: 488 bytes (122 insns) */
#include "types.h"

extern u8 step_mode;
extern u8 card_result;
extern u8 now_album;
extern u32 card_cnt;
extern u8 album_status[];
extern u8 name_ent_rpl_nosys_new_fmt[];
extern u8 name_ent_rpl_nosys_go_fmt[];

extern void card_step_init();
extern void printf();
extern s32 card_file_read_main2();
extern void card_state_clear();
extern void save_flag_clear();
extern void goto_sub_step();
extern s32 save_check();
extern s32 save_game_restore();
extern void name_ent_param_set();
extern void name_ent_card_io_noop();
extern void _game_work_init_set();
extern void game_work_init2();
extern void name_ent_option_restore();
extern void goto_step();

void name_ent_loada7(void)
{
    s32 v;

    switch (step_mode) {
        case 0:
            card_step_init();
            printf(name_ent_rpl_nosys_new_fmt, now_album);
            step_mode = step_mode + 1;
            break;
        case 1:
            if (card_file_read_main2() == 4) {
                card_state_clear();
                step_mode = step_mode + 1;
            } else {
                v = card_result;
                if (v == 240) {
                    save_flag_clear();
                    goto_sub_step(1);
                } else if (card_cnt >= 513 || v == 208) {
                    card_state_clear();
                    goto_sub_step(255);
                }
            }
            break;
        case 2:
            if (save_check(0) == 1) {
                step_mode = step_mode + 1;
            } else {
                album_status[now_album] = 255;
                goto_sub_step(255);
            }
            break;
        case 3:
            if (save_game_restore(0) == 1) {
                name_ent_param_set();
                name_ent_card_io_noop();
                step_mode = step_mode + 1;
            } else {
                album_status[now_album] = 255;
                goto_sub_step(255);
            }
            break;
        case 4:
            printf(name_ent_rpl_nosys_go_fmt, now_album);
            _game_work_init_set();
            game_work_init2();
            name_ent_option_restore();
            goto_step(20);
            break;
    }
}
