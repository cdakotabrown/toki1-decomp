/* @SymbolStart: 0x8013CD84 */
/* @SymbolSize: 892 */
/* date_exit - DATE overlay, Address: 0x8013CD8C (window 0x8013CD84:892, */
/* @CompileEnv: IDO52_AS1_XBB_VOLATILE_OK=all IDO52_AS1_NO_SCHED_LOAD_HOIST=all IDO52_FORM_SITE=const:56:64:0 IDO52_FORCE_WEB_COLOR=14:v1,19:v0,94:v1,189:a1,188:a2,196:a1 IDO52_FORCE_WEB_NOSPILL=196 IDO52_FORCE_WEB_COLOR_NOSPLIT=196 IDO52_CONST_DEF_BB=189:37:-,189:30:+ IDO52_UGEN_DEST_TEMP=23,37 */
/* @PostProc: swap_branch_operands --offsets 0x240 */
#include "types.h"

extern void season_bgm_set(void);
extern void CDQ_init(void);
extern void device_state_init(void);
extern s32 device_load_poll(void);
extern void param_clamp(void);
extern void device_cmd_send(s32 a0);
extern s32 return_schedule(s32 a0);

extern volatile u8 step_mode;
extern volatile s32 step_counter;
typedef struct { u32 c:29; u32 b:1; u32 a:2; } EcBits;
typedef struct { u32 lo:4; u32 flag:1; u32 hi:27; } RecFlags;
extern EcBits reserved_date;
extern s32 now_date_spot;
extern u8 date_event_flag;
extern volatile u8 now_girl;
extern volatile s16 para_you;
extern u8 girl_no;
extern volatile u8 now_basyo;
extern u8 now_month;
extern u8 now_day;
extern s32 rpg_bat_arg;
extern u8 machiawase_basyo;
extern s32 date_disp_flag;
extern u8 machiawase_mode;
extern u8 basyo_hist[];
extern u8 screen_w[];

s32 date_exit(void)
{
    s32 param_v0;
    s32 v0;
    s32 v1;
    s32 w;
    s32 e876;

    param_v0 = step_mode;

    if (param_v0 == 0) {
        season_bgm_set();
        param_v0 = (step_mode + 1) & 0xFF;
    }
    step_mode = (u8)param_v0;
    if (param_v0 != 1) goto L_CE30;

    v1 = step_counter;
    v0 = (u32)v1 >= 1024U;
    v1 = v1 + 1;
    step_counter = v1;
    if (v0 != 0) {
        CDQ_init();
        device_state_init();
        step_mode = 0;
    }
    if (device_load_poll() == 1) {
        param_v0 = step_mode;
        step_mode = (u8)(param_v0 + 1);
        goto L_CE30;
    }
    return 0;

L_CE30:
    if (reserved_date.b) goto L_D088;
    v1 = now_date_spot;
    if (v1 > 0) {
        if (date_event_flag == 0) {
            screen_w[*(u8 *)&now_girl * 56 + v1 + 464]++;
        }
    }

    e876 = *(s16 *)&para_you;
    if (e876 < 35) {
        if (e876 < 30) goto L_CF24;
        if (now_girl == 7) goto L_CF24;
    }

    if (v1 > 0) {
        *(u8 *)(screen_w + now_girl + 1836) = (u8)v1;
        *(u8 *)(screen_w + now_girl + 1852) = 12;
    }

    if (v1 == 8) {
        *(u8 *)(screen_w + now_girl + 1836) = 7;
    }

    if (v1 == 6) {
        *(u8 *)(screen_w + now_girl + 1836) = 5;
    }

L_CF24:
    e876 = para_you;
    if (e876 < 35) {
        if (e876 < 30) goto L_D088;
        if (girl_no == 7) goto L_D088;
    }

    w = now_basyo;
    if (w == 18) {
        v0 = (s32)(screen_w + (s32)now_girl * 56);
        *(s16 *)(v0 + 446) = (s16)(*(s16 *)(v0 + 446) + 1);
    }

    if (5 == now_month) {
        v0 = now_day;
        if (v0 == 4) goto L_CFBC;
        if (5 != v0) goto L_CFF4;
L_CFBC:
        v0 = (s32)(screen_w + (s32)now_girl * 56);
        if (((RecFlags *)(v0 + 456))->flag) {
            *(s16 *)(v0 + 446) = (s16)(*(s16 *)(v0 + 446) + 2);
        }
    }

L_CFF4:
    if (rpg_bat_arg != 0) goto L_D088;
    if (w != machiawase_basyo) goto L_D088;
    v0 = (s32)(screen_w + (s32)now_girl * 56);
    *(s16 *)(v0 + 454) = (s16)(*(s16 *)(v0 + 454) - 40);
    if (5 != now_month) goto L_D088;
    v0 = now_day;
    if (v0 == 3) goto L_D064;
    if (v0 != 4) goto L_D088;
L_D064:
    v0 = (s32)(screen_w + (s32)now_girl * 56);
    ((RecFlags *)(v0 + 456))->flag = 1;

L_D088:
    param_clamp();
    w = now_basyo;
    machiawase_mode = (u8)w;
    basyo_hist[3] = basyo_hist[2];
    basyo_hist[2] = basyo_hist[1];
    basyo_hist[1] = basyo_hist[0];
    basyo_hist[0] = (u8)w;
    date_disp_flag = 0;
    device_cmd_send(207);
    return return_schedule(1);
}
