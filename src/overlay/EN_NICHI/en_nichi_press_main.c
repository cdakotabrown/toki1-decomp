/* en_nichi_press_main - EN_NICHI overlay, Address: 0x8013352C, Size: 476 bytes (119 insns) */
/* @PostProc: patch_insn_reg_field 0xa4:rt:v1:v0 0xa8:rs:v1:v0 0xb4:rt:a0:v1 0xf0:rs:a0:v1 */
#include "types.h"
#include "tokimeki/class_w.h"

extern void en_nichi_cursor_move();
extern void en_nichi_marker_add();
extern void en_nichi_cmd_send_once();
extern void en_nichi_near_set();

extern s16 cursor_cw_x;
extern s16 cursor_cw_y;
extern s32 pad_data;
extern s32 en_nichi_poi_wear;
extern s32 en_nichi_action_step;
extern s32 en_nichi_action_timer;
extern u8 en_nichi_near_tbl[10];
extern CLASS_W class_w[];

s32 en_nichi_press_main(void)
{
    s32 i;

    en_nichi_cursor_move();
    en_nichi_action_timer = en_nichi_action_timer + 1;
    if (pad_data & 0x0020) {
        if (en_nichi_action_timer < 4) {
            return;
        }
        en_nichi_marker_add(cursor_cw_x, cursor_cw_y);
        en_nichi_cmd_send_once(1281);
        en_nichi_action_step = 1;
        en_nichi_action_timer = 0;
        class_w[152].prio = 7;
        en_nichi_poi_wear += 2;
        en_nichi_near_set();

        for (i = 0; i < 10; i++) {
            if (en_nichi_near_tbl[i] == 2) {
                en_nichi_poi_wear += 32;
            } else if (en_nichi_near_tbl[i] == 1) {
                en_nichi_poi_wear += 16;
            }
        }

        if (en_nichi_poi_wear >= 80) {
            en_nichi_poi_wear = 100;
        }
    }
}
