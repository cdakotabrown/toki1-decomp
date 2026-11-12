/* @SymbolStart: 0x8013B140 */
/* @SymbolSize: 304 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v0:v1 0x14:rs:v0:v1 0x14:rt:v0:v1 0x1c:rs:v0:v1 0x28:rs:v0:v1 0x30:rs:v0:v1 0x48:rt:v0:v1 0x4c:rs:v0:v1 0x4c:rt:v0:v1 0x60:rt:v0:v1 0x64:rs:v0:v1 0x64:rt:v0:v1 0x78:rt:v0:v1 0x7c:rs:v0:v1 0x7c:rt:v0:v1 0x84:rs:v0:v1 */
/* Size: 296 + 8 hoisted = 304 bytes (76 insns). */
#include "types.h"

extern s32 frame_counter;
extern u8 now_sub_sub_step;
extern u8 taco_round_no;
extern s16 taco_time_left;
extern u8 taco_main2d_str0[];
extern u8 taco_main2d_str2[];
extern u8 taco_main2d_str1[];

extern void taco_game_init();
extern void taco_main2d_main1();
extern void taco_main2d_main2();
extern void taco_screen_zoom_disp();
extern void taco_time_disp();
extern void taco_str_disp2();
extern void taco_cd_load_step();

void taco_main2d(void)
{
    s32 sel;

    sel = now_sub_sub_step;
    switch (sel) {
    case 0:
        taco_game_init();
        sel = now_sub_sub_step;
        break;
    case 1:
        taco_main2d_main1();
        sel = now_sub_sub_step;
        break;
    case 2:
        taco_main2d_main2();
        sel = now_sub_sub_step;
        break;
    }

    if (sel != 0) {
        if ((frame_counter & 0x10) != 0) {
            if ((u32)taco_round_no >= 3U) {
                taco_str_disp2(taco_main2d_str0, -64, 0, 2);
            } else if (taco_time_left == 0) {
                taco_str_disp2(taco_main2d_str1, -64, 0, 1);
            } else {
                taco_str_disp2(taco_main2d_str2, -64, 0, 1);
            }
        }
        taco_screen_zoom_disp(100);
    } else {
        taco_time_disp();
    }
    taco_cd_load_step();
}
