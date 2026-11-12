/* @CompileEnv: IDO52_CONST_NOINREG_VALUES=56:2:2 IDO52_FORCE_WEB_SPILL=22 IDO52_FORCE_WEB_COLOR=159:s1 IDO52_CONST_DEF_BB=159:27:-,159:20:+ IDO52_UGEN_TEMP_REG=17:t4,18:t8,52:t3,49:t2,50:t5,51:t9,48:t1,47:t4,24:t8,44:t6,45:t7,46:t0 */
/* @CompileFlags: -cckr -Wc,-pic0 */
/* @PostProc: patch_insn_reg_field 0xc0:rt:t8:t4 0xc4:rt:t8:t4 */
/* Address: 0x8013E7B4 */
/* Size: 952 bytes (238 instructions) */
#include "types.h"

typedef struct {
    s16 h[7];
} Vec7;

typedef struct {
    u8 pad00[6];
    s16 f06;
    u8 pad08[2];
    s16 f0A;
    u8 pad0C[4];
    u32 b0 : 1;
    u32 drawn : 1;
    u32 rest : 30;
    u8 pad14[36];
} Rec;

typedef struct {
    u8 hdr[440];
    Rec r[11];
    u8 pad420[0x720 - 0x420];
    u8 e720;
    u8 e721;
    u8 e722;
    u8 pad723[0xF5F - 0x723];
    u8 sel;
    u8 padF60[0x1108 - 0xF60];
    u8 f1108;
    u8 f1109;
} Save;

extern Save screen_w;
extern s16 para_you;
extern u8 girl0_basyo;
extern u8 now_girl;
extern s32 frame_counter;
extern s32 rec_res;
extern u8 shougatu_num_girl_fmt[];
extern s16 shougatu_menu_now_serifu;
extern u8 shougatu_girl9_appear;
extern u8 shougatu_menu_girl1;
extern Vec7 shougatu_girl_select_tbl0;
extern Vec7 shougatu_girl_select_tbl1;

shougatu_girl_select()
{
    s32 f;
    s32 r;
    Vec7 a;
    Vec7 b;
    s16 i;
    s16 cnt;
    u32 t;
    s32 row;

    a = shougatu_girl_select_tbl0;
    b = shougatu_girl_select_tbl1;
    cnt = 0;
    for (i = 0; i < 11; i++) {
        cnt += (screen_w.r[i].drawn == 0) ^ 1;
    }
    i = 0;
    do {
        now_girl = ((u32)frame_counter + i) % 10 + 1;
        printf(shougatu_num_girl_fmt, cnt, now_girl);
        i++;
        f = (*(u32 *)((u8 *)&screen_w + now_girl * 56 + 456) << 30) >> 31;
    } while ((f == 0 && now_girl >= 9U) || (now_girl == 5 && girl0_basyo >> 4 == 8)
             || (now_girl == 6 && girl0_basyo >> 4 == 9) || (f == 0 && cnt >= 2));
    if (shougatu_girl9_appear != 0) {
        screen_w.sel = 9;
    }
    t = screen_w.sel;
    shougatu_menu_girl1 = t;
    shougatu_menu_now_serifu = 0;
    r = get_g_zyotai_s(t);
    if (screen_w.sel == 6 && (u32)(r & 0x7F) >= 2) {
        shougatu_menu_now_serifu++;
    }
    if ((u8)(r & 0x7F) == 4) {
        shougatu_menu_now_serifu++;
    }
    shougatu_girl_slot_set();
    date_name_set(screen_w.sel);
    if (!screen_w.r[screen_w.sel].drawn || (shougatu_girl9_appear != 0 && screen_w.sel == 9)) {
        screen_w.r[screen_w.sel].drawn = 1;
        screen_w.e720 = screen_w.f1108;
        screen_w.e721 = screen_w.f1109;
        screen_w.e722 = 17;
        rec_res = 3;
        if (shougatu_girl9_appear != 0) {
            event_log_add(8, (u8)(screen_w.sel * 10));
        } else {
            event_log_add(4, 0);
        }
        goto_sub_step(5);
        return;
    }
    screen_w.r[screen_w.sel].f0A += 4;
    for (i = 0; i < 7; i++) {
        if (para_you < a.h[i]) {
            break;
        }
    }
    screen_w.r[screen_w.sel].f06 += b.h[i];
    event_log_add(4, 0);
    param_clamp();
    sub_sub_step_up();
}
