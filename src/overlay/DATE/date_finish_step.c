/* @CompileEnv: IDO52_FORM_SITE=const:7:25:0 IDO52_NO_CSE_SITE=cvt:9 IDO52_COPYPROP_VAR=-8@8:1-2 IDO52_COPYPROP_SITE=lod:1:deny,lod:3:deny IDO52_FORCE_WEB_COLOR=140:v1 */
/* @SymbolStart: 0x8014C174 */
/* @SymbolSize: 836 */
/* @PostProc: swap_branch_operands --offsets 0x170,0x1cc */
/* Address: 0x8014C174 (cross-prologue head) / 0x8014C17C (body) */
/* Size: 836 bytes (209 instructions) */
#include "types.h"

extern void season_bgm_set(void);
extern void CDQ_init(void);
extern void device_state_init(void);
extern s32 device_load_poll(void);
extern void event_log_add(s32 a0, s32 a1);
extern void param_clamp(void);
extern s32 return_schedule(s32 a0);

extern u8 step_mode;
extern s32 step_counter;
extern s16 para_you;
extern s32 now_date_spot;
extern s32 event_girl_no;
extern s32 work_tarao;
extern u8 date_ev2f_ans;
extern s32 rec_res;
extern u8 now_event_no;
extern u8 now_basyo;
extern s32 date_disp_flag;
extern u8 screen_w[];

typedef struct {
    u8 pad[6];
    s16 f6;
    u8 pad2[6];
    s16 f14;
    u32 b0 : 4;
    u32 b4 : 1;
    u32 b5 : 27;
    u8 pad3[36];
} Ent;

#define IDX  screen_w[3935]
#define IDX2 screen_w[3934]
#define SEL  screen_w[63]
#define SUB  screen_w[64]
#define ENT  ((Ent *)(screen_w + 440))

s32 date_finish_step(void)
{
    s32 step;
    s32 v0;
    s32 v1;

    step = step_mode;
    if (step == 0) {
        season_bgm_set();
        step = (step_mode + 1) & 0xFF;
    }
    step_mode = step;
    if (step != 1) goto L_C220;
    if ((u32)step_counter++ >= 1024U) {
        CDQ_init();
        device_state_init();
        step_mode = 0;
    }
    if (device_load_poll() == 1) {
        step = step_mode;
        step_mode = step + 1;
        goto L_C220;
    }
    return 0;
L_C220:
    v0 = para_you;
    if (v0 >= 35 || (v0 >= 30 && IDX != 7)) {
        v0 = now_date_spot;
        if (v0 >= 0) {
            screen_w[IDX + 1836] = v0;
            screen_w[IDX + 1852] = 12;
        }
        if (v0 == 8) {
            screen_w[IDX + 1836] = 7;
        }
        if (v0 == 6) {
            *(s8 *)&screen_w[IDX + 1836] = 5;
        }
    }
    if (!((event_girl_no == 7 && work_tarao == 6) || event_girl_no == 11)) {
        v0 = screen_w[IDX2 * 8 + 1880];
        if (v0 == 1 || v0 == 0) {
            IDX2--;
        }
        if (event_girl_no == 7 && work_tarao == 2) {
            rec_res = date_ev2f_ans + 1;
        }
        event_log_add(2, now_event_no);
    }
    ENT[IDX].f14 -= 40;
    if (SEL == 5 && (SUB == 4 || SUB == 5)) {
        if (ENT[IDX].b4) {
            ENT[IDX].f6 += 2;
        }
    }
    v0 = SUB;
    if (SEL == 5 && (v0 == 3 || v0 == 4)) {
        ENT[IDX].b4 = 1;
    }
    param_clamp();
    v0 = now_basyo;
    screen_w[1828] = v0;
    screen_w[1835] = screen_w[1834];
    screen_w[1834] = screen_w[1833];
    screen_w[1833] = screen_w[1832];
    screen_w[1832] = v0;
    date_disp_flag = 0;
    return return_schedule(1);
}
