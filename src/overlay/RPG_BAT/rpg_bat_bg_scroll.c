/* Address: 0x80153784 (absorbed cross-prologue head) .. 0x80153A60 */
/* Size: 732 bytes (183 instructions) */
/* @SymbolStart: 0x80153784 */
/* @SymbolSize: 732 */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:6 */
#include "types.h"

typedef struct {
    s16 x;
    s16 y;
    s16 w;
    s16 h;
} Rect;

extern void MoveImage(Rect *, s32, s32);
extern void rpg_bat_anim_set(s32, s32, s32, s32);
extern s32  sub_sub_step_up(void);
extern s32  sub_step_up(void);
extern void rpg_bat_bg_wave_disp(void);
extern void rpg_bat_bg_wave2_disp(void);
extern s32  rpg_bat_level_up(void);

extern u8  now_sub_sub_step;
extern s32 rpg_bat_target_no[];
extern s32 rpg_bat_seq_work[];

s32 rpg_bat_bg_scroll(void)
{
    s32 r;
    s32 sel;
    s32 i;
    Rect r1;
    Rect r2;
    Rect r3;
    Rect r4;

    sel = now_sub_sub_step;
    r = sel;
    switch (sel) {
    case 0:
        r1.x = 576; r1.y = 256;
        r1.w = 64; r1.h = 40;
        MoveImage(&r1, 384, 384);
        r2.x = 576; r2.y = 296;
        r2.w = 64; r2.h = 40;
        MoveImage(&r2, 448, 384);
        r3.x = 544; r3.y = 256;
        r3.w = 64; r3.h = 40;
        MoveImage(&r3, 512, 384);
        r4.x = 543; r4.y = 256;
        r4.w = 1; r4.h = 128;
        MoveImage(&r4, 544, 256);
        rpg_bat_target_no[37] = 32;
        rpg_bat_target_no[38] = 64;
        rpg_bat_target_no[39] = 1;
        rpg_bat_target_no[96] = -128;
        rpg_bat_target_no[97] = 32;
        rpg_bat_target_no[98] = 64;
        rpg_bat_target_no[99] = 1;
        rpg_bat_target_no[35] = 0;
        rpg_bat_target_no[36] = 128;
        rpg_bat_target_no[95] = 0;
        r = sub_sub_step_up();
        break;
    case 1:
        rpg_bat_target_no[35] += rpg_bat_target_no[39];
        rpg_bat_target_no[95] += rpg_bat_target_no[99];
        if (rpg_bat_target_no[35] >= rpg_bat_target_no[37]) {
            rpg_bat_target_no[35] = 0;
        }
        if (rpg_bat_target_no[95] >= rpg_bat_target_no[97]) {
            rpg_bat_target_no[95] = 0;
        }
        rpg_bat_bg_wave_disp();
        rpg_bat_bg_wave2_disp();
        r = rpg_bat_level_up();
        rpg_bat_target_no[96] = rpg_bat_target_no[96] + 1;
        rpg_bat_target_no[36] = rpg_bat_target_no[36] - 1;
        if (rpg_bat_target_no[36] == 0) {
            r = sub_sub_step_up();
        }
        break;
    case 2:
        r3.x = 512; r3.y = 384;
        r3.w = 1; r3.h = 40;
        MoveImage(&r3, 544, 256);
        rpg_bat_anim_set(31, 0, 1, 1);
        r = sub_sub_step_up();
        break;
    case 3:
        for (i = 0; i < 50; i++) { rpg_bat_seq_work[i] = 0; }
        r = sub_step_up();
        break;
    }
    return r;
}
