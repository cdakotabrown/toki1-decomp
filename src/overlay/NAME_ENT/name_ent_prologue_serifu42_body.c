/* @CompileEnv: IDO52_FORCE_WEB_COLOR=2:v1 */
/* @CompileFlags: -cckr */
/* @SymbolStart: 0x8014AC78 */
/* @SymbolSize: 188 */
/* name_ent_prologue_serifu42_body -- NAME_ENT overlay, Address: 0x8014AC80 */
#include "types.h"

extern void name_ent_serifu_set(void);
extern void name_ent_text_scroll(void);
extern void device_cmd_send(s32 arg);
extern void sub_step_up(void);
extern void dec_bg_show(s32 arg);

extern u8 name_ent_serifu_no;
extern s32 name_ent_step_cnt;
extern u8 step_mode;
extern u8 step_mode;

void name_ent_prologue_serifu42_body(void)
{
    switch (step_mode) {
    case 0:
        name_ent_serifu_no = 42;
        name_ent_step_cnt = 0;
        name_ent_serifu_set();
        step_mode = step_mode + 1;
        break;
    case 1:
        name_ent_text_scroll();
        break;
    case 2:
        if (name_ent_step_cnt++ >= 61) {
            device_cmd_send(5);
            device_cmd_send(209);
            sub_step_up();
        }
        break;
    }
    dec_bg_show(1);
}
