/* @SymbolStart: 0x8013CB7C */
/* @SymbolSize: 340 */
/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:6 IDO52_UGEN_TEMP_REG=1:v1,3:v1 IDO52_UGEN_TEMP_REG_INPLACE=occ:1,3 */
#include "types.h"

extern u8 step_mode;
extern u32 step_counter;
extern u8 card_sys_present;
extern u8 name_ent_card_search0_str0[];
extern u8 album_work[];

extern s32 card_step_init();
extern s32 card_file_scan_main();
extern s32 printf();
extern s32 text_sys_init();
extern s32 save_flag_clear();
extern s32 goto_sub_step();
extern s32 card_state_clear();
extern s32 goto_sub_sub_step();
extern s32 sub_sub_step_up();

s32 name_ent_card_search0()
{
    s32 sel;
    s32 ret;
    u8 *p;
    s32 n;
    u8 *end;

    sel = step_mode;
    ret = sel;

    switch (sel) {
    case 0:
        ret = card_step_init();
        step_mode++;
        break;
    case 1:
        ret = card_file_scan_main();
        if (ret == 2) {
            ret = printf(name_ent_card_search0_str0);
            step_mode++;
        } else if (step_counter >= 513) {
            save_flag_clear();
            goto_sub_step(1);
            ret = card_state_clear();
        }
        break;
    case 2:
        n = 0;
        p = &(&album_work[1])[0];
        end = &(&album_work[1])[14];
        do {
            if (p[4]) {
                n++;
            }
            p++;
        } while (p != end);
        text_sys_init();
        if (card_sys_present != 0) {
            ret = sub_sub_step_up();
        } else if (n != 0) {
            ret = goto_sub_sub_step(32);
        } else {
            ret = goto_sub_sub_step(64);
        }
        break;
    case 3:
        break;
    }
    return ret;
}
