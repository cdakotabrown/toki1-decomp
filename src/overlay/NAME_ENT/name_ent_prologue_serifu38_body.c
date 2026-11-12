/* @SymbolStart: 0x8014A8C0 */
/* @RodataBase: 0x8014CB94 */
/* Address: 0x8014A8C8 */
/* Size: 320 bytes (80 insns) */
/* @SymbolSize: 352 */
#include "types.h"

extern void name_ent_serifu_set(void);
extern s32  get_CD_State(void);
extern void name_ent_text_scroll(void);
extern void sub_step_up(void);
extern void dec_bg_show(s32);

extern u8  name_ent_serifu_no;
extern s32 name_ent_step_cnt;
extern u8  step_mode;
extern u8 step_mode;

void name_ent_prologue_serifu38_body(void) {
    switch (step_mode) {
    case 0:
        name_ent_serifu_no = 38;
        name_ent_serifu_set();
        step_mode++;
        break;
    case 1:
        if (get_CD_State() & 4) {
            step_mode++;
        }
        break;
    case 2:
        name_ent_text_scroll();
        break;
    case 3:
        name_ent_serifu_no = 39;
        name_ent_serifu_set();
        step_mode++;
        break;
    case 4:
        name_ent_text_scroll();
        break;
    case 5:
        name_ent_serifu_no = 40;
        name_ent_serifu_set();
        step_mode++;
        break;
    case 6:
        name_ent_text_scroll();
        break;
    case 7:
        name_ent_serifu_no = 41;
        name_ent_serifu_set();
        step_mode++;
        break;
    case 8:
        name_ent_text_scroll();
        break;
    case 9:
        name_ent_step_cnt = 0;
        sub_step_up();
        break;
    }
    dec_bg_show(1);
}
