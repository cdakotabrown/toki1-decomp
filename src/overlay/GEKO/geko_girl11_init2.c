/* @PostProc: patch_insn_reg_field 0x144:rt:t8:t9 0x160:rs:t5:t6 0x168:rt:t6:t7 0x170:rs:t6:t7 0x170:rt:t7:t8 0x174:rt:t7:t8 0x188:rt:t0:t1 0x194:rt:t0:t1 0x140:rd:t8:t9 0x15c:rd:t5:t6 */
/* geko_girl11_init2 -- GEKO overlay, Address: 0x80141144, Size: 376 bytes (94 insns) */
#include "types.h"
#include "tokimeki/class_w.h"

typedef struct { s32 stride[14]; } GekoEntry;

extern void draw2d3d(s32 a0, s32 a1);
extern void class_switch(s32 a0);
extern void prim_init(void);
extern void class_clear_all(s32 a0);
extern void back_clear_switch(s32 a0);
extern void tpage_buf_clear(void);
extern void text_sys_init(void);
extern void basyo_init(void *a0);
extern void bustup_ptr_init(void);
extern void geko_ptr_tbl_init(void);
extern void hizuke_init(void);
extern void message_window_init(void);
extern void yazirushi_off(void);
extern void shared_system_state_reset(void);
extern void strcpy(void *a0, void *a1);
extern void param_clamp(void);
extern void sub_sub_step_up(void);

extern s16 now_serifuev;
extern s16 now_serifuev_in;
extern u8 now_event_no;
extern s8 now_girl;
extern s8 cw3_disp;
extern s8 fade_bri;
extern s16 para_str;
extern s32 work_tarao;
extern s32 geko_ss_serifuev;
extern s32 geko_ss_serifuev_kind;
extern s32 geko_sss_soundev;

extern u8 p_name[];
extern u8 p_sei[];
extern u8 geko_girl11_basyo_str[];
extern GekoEntry screen_w[];
extern CLASS_W class_w[];
extern s32 geko_serifuev_tbl[];
extern s32 geko_serifuev_kind_tbl[];
extern s32 geko_soundev_tbl[];

void geko_girl11_init2(void)
{
    s16 i;

    now_girl = 11;
    draw2d3d(1, 0);
    class_switch(1);
    prim_init();
    class_clear_all(0);
    back_clear_switch(1);
    tpage_buf_clear();
    text_sys_init();
    basyo_init(&geko_girl11_basyo_str);
    cw3_disp = 0;
    (*(s8 *)&class_w[4].disp_flag) = 0;
    now_serifuev = 0;
    now_serifuev_in = 0;
    bustup_ptr_init();
    geko_ptr_tbl_init();
    hizuke_init();
    message_window_init();
    yazirushi_off();
    fade_bri = 0;
    strcpy(&p_name, &p_sei);
    shared_system_state_reset();
    geko_ss_serifuev = geko_serifuev_tbl[75];
    geko_ss_serifuev_kind = geko_serifuev_kind_tbl[75];
    geko_sss_soundev = geko_soundev_tbl[75];

    if (work_tarao == 1) {
        para_str = para_str - 5;

        for (i = 0; i < 11; i++) {
            if (*(s32 *)((u8 *)&screen_w[i] + 456) << 30 < 0) {
                *(s16 *)((u8 *)&screen_w[i] + 446) =
                    *(s16 *)((u8 *)&screen_w[i] + 446) - 1;
            }
        }

        param_clamp();
    }

    now_event_no = 255;
    sub_sub_step_up();
}
