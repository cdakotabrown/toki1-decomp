/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:0 */
/* @SymbolStart: 0x8013D84C */
/* @SymbolSize: 308 */
#include "types.h"

extern s32 card_step_init();
extern s32 card_file_read_main2();
extern s32 save_flag_clear();
extern s32 goto_sub_step();
extern s32 card_state_clear();
extern s32 save_check();
extern s32 name_ent_album_disp_init();
extern s32 goto_sub_sub_step();

extern u8 step_mode;
extern u8 card_result;
extern u32 card_cnt;
extern u8 now_album;
extern u8 album_status[];

s32 name_ent_album_chosen_load()
{
    s32 ret;
    s32 state;

    ret = step_mode;
    switch (step_mode) {
    case 0:
        ret = card_step_init();
        step_mode = step_mode + 1;
        break;
    case 1:
        ret = card_file_read_main2();
        if (ret == 4) {
            step_mode = step_mode + 1;
        } else {
            state = card_result;
            if (state == 240) {
                save_flag_clear();
                goto_sub_step(1);
                return;
            }
            if (card_cnt >= 513 || state == 208) {
                card_state_clear();
                goto_sub_step(255);
            }
            return;
        }
        break;
    case 2:
        ret = save_check(0);
        if (ret == 1) {
            name_ent_album_disp_init();
            ret = goto_sub_sub_step(33);
        } else {
            album_status[now_album] = 255;
            ret = goto_sub_step(255);
        }
        break;
    }
    return ret;
}
