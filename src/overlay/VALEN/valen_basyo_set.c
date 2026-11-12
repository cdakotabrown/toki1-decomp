/* @SymbolStart: 0x80133074 */
/* @SymbolSize: 1040 */
/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_COPYPROP_SITE=lod:3:deny IDO52_AS1_NO_PASS1_PROP=19 IDO52_AS1_TIE_TAKE=243,323 */
#include "types.h"

extern u32 frame_counter;
extern u8 rec_idx;
extern u8 now_girl;
extern s16 girl0_tokimeki;
extern s32 ba_string2;
extern s32 rec_res;
extern s16 valen_list_no;
extern s16 valen_girl_list[];
extern s16 valen_girl_list_cnt;
extern u8 valen_girl9_flag;
extern u8 valen_basyo_mode;
extern s32 valen_basyo_set_str0;
extern s32 valen_basyo_set_str1;
extern s32 valen_basyo_set_str2;
extern s32 valen_basyo_set_str3;
extern s32 valen_basyo_set_str4;
extern s32 valen_basyo_set_str5;
extern s32 valen_basyo_set_str6;
extern s32 valen_basyo_set_str7;

extern void strcpy();
extern void event_log_add();
extern void dec_bg_cache_step();
extern u32 get_h_tokimeki();
extern void bustup_face_change();
extern void sub_sub_step_up();

void valen_basyo_set()
{
    u32 mode;
    s32 msg;
    u32 seed;

    seed = frame_counter;
    mode = seed & 7;
    if (mode == 0) {
        mode = 3;
    } else if (mode < 4) {
        mode = 0;
    } else {
        mode = 1;
    }
    if (now_girl == 0) {
        mode = seed;
        mode = mode & 1;
    }
    if (mode == 0) goto c0;
    if (mode == 1) goto c1;
    valen_basyo_mode = mode;
    if (mode != 2) goto cd;
    goto c2;
c0:
    valen_basyo_mode = mode;
    strcpy(&ba_string2, &valen_basyo_set_str0);
    msg = 16453;
    goto done;
c1:
    valen_basyo_mode = mode;
    strcpy(&ba_string2, &valen_basyo_set_str1);
    msg = 16469;
    goto done;
c2:
    strcpy(&ba_string2, &valen_basyo_set_str2);
    msg = 16506;
    goto done;
cd:
    strcpy(&ba_string2, &valen_basyo_set_str3);
    msg = 16516;
done:
    if (now_girl != 0) {
        if (valen_list_no + 1 == valen_girl_list_cnt) {
            strcpy(&ba_string2, &valen_basyo_set_str4);
            msg = 16434;
        }
        if (valen_list_no + 2 == valen_girl_list_cnt && valen_girl_list[valen_list_no + 1] == 0) {
            msg = 16434;
            strcpy(&ba_string2, &valen_basyo_set_str5);
        }
    }
    if (valen_list_no + 1 == valen_girl_list_cnt && now_girl == 0 &&
        (girl0_tokimeki >= 70 || valen_girl9_flag != 0)) {
        strcpy(&ba_string2, &valen_basyo_set_str6);
        msg = 16716;
    }
    if (now_girl == 9 && valen_girl9_flag == 1) {
        msg = 16469;
        strcpy(&ba_string2, &valen_basyo_set_str7);
    }
    if (now_girl == 0) {
        if (girl0_tokimeki >= 70) {
            rec_res = 1;
        } else {
            rec_res = 2;
        }
    } else {
        if (get_h_tokimeki(now_girl) >= 70) {
            rec_res = 1;
        } else {
            rec_res = 2;
        }
    }
    switch (msg) {
    case 16453:
        event_log_add(6, 0);
        break;
    case 16469:
        event_log_add(6, 1);
        break;
    case 16716:
        event_log_add(6, 5);
        break;
    case 16434:
        event_log_add(6, 4);
        break;
    case 16506:
        event_log_add(6, 3);
        break;
    default:
        event_log_add(6, 2);
        break;
    }
    if (now_girl == 9 && valen_girl9_flag == 1) {
        rec_idx--;
        rec_res = 1;
        event_log_add(6, 3);
    }
    dec_bg_cache_step(msg);
    if (get_h_tokimeki(now_girl) >= 70) {
        bustup_face_change(0);
    } else {
        bustup_face_change(1);
    }
    sub_sub_step_up();
}
