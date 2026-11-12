/* Address: 0x8013A634 (true symbol start 0x8013A5EC, cross-prologue) */
/* Size: 260 bytes (65 insns). */
/* @SymbolStart: 0x8013A5EC */
/* @SymbolSize: 260 */
#include "types.h"

typedef struct { u8 pad[56]; } S56;

extern void shugaku_cmd_send(void);
extern void param_clamp(void);
extern void sub_sub_step_up(void);

extern s32 serifu_no;
extern u8  shugaku_place;
extern s16 now_serifu_shugaku;
extern u8  date_place;
extern u8  menu_x;
extern u8  now_girl;
extern S56 screen_w[];

void shugaku_sel_branch(void) {
    s32 v0;

    v0 = serifu_no;
    now_serifu_shugaku = (s16)((now_serifu_shugaku - shugaku_place) + (v0 == 1)
                       + (v0 == 2) * 3 + 2);
    menu_x = (v0 > 0);
    if (v0 == 0) {
        shugaku_cmd_send();
    } else {
        u8 idx = now_girl;
        *(s16 *)((u8 *)&screen_w[idx] + 446) += 5;
    }
    param_clamp();

    if (date_place != 0) {
        v0 = serifu_no;
        if (v0 == 2) {
            now_girl = 13;
        } else if (v0 == 1) {
            now_girl = 255;
        }
    }
    sub_sub_step_up();
}
