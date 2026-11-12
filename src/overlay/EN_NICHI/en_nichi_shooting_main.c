/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=102:a1,106:a2,110:a3,114:t0 */
/* @RodataBase: 0x80139554 */
/* @SymbolStart: 0x801376A4 */
/* @SymbolSize: 2340 */
#include "types.h"

typedef struct {
    u8  pad00;
    u8  f01;
    u8  f02;
    u8  f03;
    u8  f04;
    u8  f05;
    u8  f06;
    u8  f07;
    u8  pad08[16];
    s16 f24;
    u8  pad26[12];
    s16 f38;
    u8  pad28[2];
    s16 f42;
    u8  pad2C[24];
} Rec68;

extern Rec68 class_w[];
extern s32 pad_trig;
extern u8  en_nichi_shot_left;
extern u8  en_nichi_shot_state[];
extern u8  en_nichi_shot_spd[];
extern s16 en_nichi_shot_hit_no[];
extern u16 en_nichi_bar_len;
extern s32 en_nichi_res;
extern u8  now_girl;
extern u8  ret_step[];
extern u8  step_mode;
extern u8  now_sub_sub_step;

en_nichi_shooting_main()
{
    s32 i;
    s32 k;
    s32 n;

    switch (now_sub_sub_step) {
    case 0:
        step_directional_scroll_once();
        class_w[103].f38 = class_w[0].f38;
        class_w[103].f42 = class_w[0].f42;
        if (pad_trig & 0x20) {
            if (en_nichi_shot_left != 0) {
                en_nichi_shot_left--;
                en_nichi_shot_state[en_nichi_shot_left] = 1;
                en_nichi_shot_spd[en_nichi_shot_left] = (en_nichi_bar_len >> 4) + 1;
                class_w[98 + en_nichi_shot_left].f38 = class_w[103].f38;
                class_w[98 + en_nichi_shot_left].f42 = class_w[103].f42;
                class_w[98 + en_nichi_shot_left].f01 = 4;
                device_cmd_send(1292);
            }
        }
        en_nichi_shot_main();
        en_nichi_lane_move();
        if (en_nichi_shot_left == 0 && en_nichi_shot_state[0] == 0 && en_nichi_shot_state[1] == 0 && en_nichi_shot_state[2] == 0
            && en_nichi_shot_state[3] == 0 && en_nichi_shot_state[4] == 0) {
            sub_sub_step_up();
        }
        break;
    case 1:
        class_w[117].f03 |= 0x80;
        k = 0;
        for (i = 0; i < 5; i++) {
            if (en_nichi_shot_hit_no[i] != 255) {
                class_w[98 + k].f24 = en_nichi_shot_hit_no[i];
                k++;
            }
            class_w[98 + i].f01 = 1;
        }
        for (; k < 5; k++) {
            class_w[98 + k].f24 = 255;
        }
        Vblnk_Timer_Init();
        en_nichi_shot_left = 0;
        sub_sub_step_up();
        break;
    case 2:
        if (Vblnk_Timer() < 46U) {
            break;
        }
        if (class_w[98 + en_nichi_shot_left].f24 != 255 && en_nichi_shot_left < 5) {
            goto place;
        }
        if (en_nichi_shot_hit_no[0] == 31 && en_nichi_shot_hit_no[1] == 31 && en_nichi_shot_hit_no[2] == 31
            && en_nichi_shot_hit_no[3] == 31 && en_nichi_shot_hit_no[4] == 31) {
            goto_sub_sub_step(5);
        } else if (en_nichi_shot_hit_no[0] == 32 && en_nichi_shot_hit_no[1] == 32 && en_nichi_shot_hit_no[2] == 32
                   && en_nichi_shot_hit_no[3] == 32 && en_nichi_shot_hit_no[4] == 32) {
            goto_sub_sub_step(6);
        } else {
            Vblnk_Timer_Init();
            sub_sub_step_up();
        }
        device_cmd_send(1291);
        break;
    place:
        device_cmd_send(1290);
        class_w[98 + en_nichi_shot_left].f03 = 132;
        class_w[98 + en_nichi_shot_left].f38 = en_nichi_shot_left * 64 - 128;
        class_w[98 + en_nichi_shot_left].f42 = 56;
        en_nichi_shot_left++;
        Vblnk_Timer_Init();
        break;
    case 3:
        if (Vblnk_Timer() < 121U) {
            break;
        }
        if (class_w[96].f07 < 4) {
            class_clear_all(0);
            sub_sub_step_up();
        } else {
            class_w[96].f07 -= 4;
        }
        for (i = 1; i < 22; i++) {
            class_w[i + 96].f07 = class_w[96].f07;
        }
        break;
    case 4:
        if (en_nichi_shot_hit_no[0] < 16 && en_nichi_shot_hit_no[1] < 16 && en_nichi_shot_hit_no[2] < 16
            && en_nichi_shot_hit_no[3] < 16 && en_nichi_shot_hit_no[4] < 16) {
            en_nichi_res = 0;
        } else if (en_nichi_shot_hit_no[0] < 33 && en_nichi_shot_hit_no[1] < 33 && en_nichi_shot_hit_no[2] < 33
                   && en_nichi_shot_hit_no[3] < 33 && en_nichi_shot_hit_no[4] < 33) {
            en_nichi_res = 1;
        } else if (en_nichi_shot_hit_no[0] == 255 && en_nichi_shot_hit_no[1] == 255 && en_nichi_shot_hit_no[2] == 255
                   && en_nichi_shot_hit_no[3] == 255 && en_nichi_shot_hit_no[4] == 255) {
            en_nichi_res = 2;
        } else if (en_nichi_shot_hit_no[0] == 16 && en_nichi_shot_hit_no[1] == 16 && en_nichi_shot_hit_no[2] == 16
                   && en_nichi_shot_hit_no[3] == 16 && en_nichi_shot_hit_no[4] == 16 && now_girl == 2) {
            en_nichi_res = 3;
        } else if (en_nichi_shot_hit_no[0] == now_girl + 20 && en_nichi_shot_hit_no[1] == now_girl + 20
                   && en_nichi_shot_hit_no[2] == now_girl + 20 && en_nichi_shot_hit_no[3] == now_girl + 20
                   && en_nichi_shot_hit_no[4] == now_girl + 20 && now_girl != 2) {
            en_nichi_res = 3;
        } else {
            en_nichi_res = 4;
        }
        goto_step(ret_step[0]);
        goto_sub_step(ret_step[1]);
        goto_sub_sub_step(ret_step[2]);
        break;
    case 5:
        switch (step_mode) {
        case 0:
            set_XAsector(0x1A0000B1, 0x170000B7, 14);
            device_cmd_send(768);
            step_mode++;
            break;
        case 1:
            if (get_CD_State() == 4) {
                step_mode++;
            }
            break;
        case 2:
            if (get_CD_State() == 2) {
                device_cmd_send(116);
                step_mode++;
            }
            break;
        case 3:
            Vblnk_Timer_Init();
            step_mode++;
            break;
        case 4:
            if (Vblnk_Timer() >= 121U) {
                Vblnk_Timer_Init();
                goto_sub_sub_step(3);
            }
            break;
        default:
            game_reset();
            break;
        }
        break;
    case 6:
        switch (step_mode) {
        case 0:
            set_XAsector(0x03000000, 0x01000007, 14);
            device_cmd_send(768);
            step_mode++;
            break;
        case 1:
            if (get_CD_State() == 4) {
                step_mode++;
            }
            break;
        case 2:
            if (get_CD_State() == 2) {
                device_cmd_send(116);
                step_mode++;
            }
            break;
        case 3:
            Vblnk_Timer_Init();
            step_mode++;
            break;
        case 4:
            if (Vblnk_Timer() >= 121U) {
                Vblnk_Timer_Init();
                goto_sub_sub_step(3);
            }
            break;
        default:
            game_reset();
            break;
        }
        break;
    default:
        game_reset();
        break;
    }
}
