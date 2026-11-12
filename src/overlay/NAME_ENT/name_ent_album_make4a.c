/* @SymbolStart: 0x8013DF20 */
/* @SymbolSize: 500 */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:6 IDO52_UGEN_TEMP_REG=5:v1,7:v1,9:v1,11:v1,13:v1 IDO52_UGEN_TEMP_REG_INPLACE=occ:5,7,9,11,13 */
#include "types.h"

extern s32 text_sys_init();
extern s32 strcpy();
extern s32 strcat();
extern s32 set_kanji_string();
extern s32 k_disp_start();
extern s32 card_file_make();
extern s32 goto_sub_step();
extern s32 card_step_init();
extern s32 _game_work_init_set();
extern s32 game_work_init2();
extern s32 card_slot_write_main();
extern s32 card_state_clear();
extern s32 save_flag_clear();
extern s32 goto_step();
extern s32 name_ent_option_restore();

extern u8   step_mode;
extern u8   now_album;
extern u8   card_result;
extern u32  card_cnt;
extern u8   album_status[];
extern s32 *card_file_save_tbl0;
extern u8   name_ent_album_make4a_str0[];

s32 name_ent_album_make4a()
{
    s32 ret;
    s32 step;
    u8 buf[256];

    step = step_mode;
    ret = step;
    switch (step) {
    case 0:
        text_sys_init();
        strcpy(buf, card_file_save_tbl0[now_album - 1]);
        strcat(buf, name_ent_album_make4a_str0);
        set_kanji_string(-64, 32, 15, buf, 0);
        k_disp_start(2);
        ret = album_status[now_album];
        if (ret == 255) {
            step_mode++;
            break;
        }
        if (ret == 1) {
            step_mode++;
            break;
        }
        ret = card_file_make(now_album);
        if (ret == 1) {
            step_mode++;
            break;
        }
        ret = goto_sub_step(4);
        break;
    case 1:
        card_step_init();
        _game_work_init_set();
        ret = game_work_init2();
        step_mode++;
        break;
    case 2:
        ret = card_slot_write_main();
        if (ret == 20) {
            step_mode++;
            break;
        }
        if (card_cnt < 513 && card_result != 240) {
            break;
        }
        card_state_clear();
        save_flag_clear();
        ret = goto_sub_step(1);
        break;
    case 3:
        card_state_clear();
        name_ent_option_restore();
        ret = goto_step(20);
        break;
    }
    return ret;
}
