/* @SymbolStart: 0x80140CF0 */
/* @SymbolSize: 296 */
/* name_ent_exit - NAME_ENT overlay, Address: 0x80140CF8 */
#include "types.h"

extern void name_ent_option_restore();
extern void set_dec_bri();
extern void name_ent_param_set();
extern void _game_work_init_set();
extern void work_clear();
extern void goto_step();
extern void season_bgm_set();
extern void goto_sub_step();

extern u8 step_mode;
extern u8 now_year;
extern u8 now_month;
extern u8 now_day;
extern u8 rec_idx;
extern u8 cursor_flag;

void name_ent_exit()
{
    u32 v = step_mode;
    s32 cond;

    cond = v >= 65;
    step_mode = v + 1;
    if (cond) {
        name_ent_option_restore();
        set_dec_bri(0);
        if (now_year == 98) {
            if (now_month == 3) {
                if (now_day == 1) {
                    name_ent_param_set();
                    _game_work_init_set();
                    work_clear();
                    rec_idx = 0;
                    goto_step(20);
                    goto done;
                }
            }
        }
        if (now_year == 95) {
            if (now_month == 4) {
                if (now_day == 4) {
                    name_ent_param_set();
                    _game_work_init_set();
                    work_clear();
                    rec_idx = 0;
                    goto_step(20);
                    goto done;
                }
            }
        }
        season_bgm_set();
        goto_step(52);
        goto_sub_step(255);
    }
done:
    cursor_flag = 1;
}
