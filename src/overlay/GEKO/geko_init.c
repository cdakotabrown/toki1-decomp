/* @SymbolStart: 0x801325C0 */
/* @SymbolSize: 492 */
/* @PostProc: patch_insn_reg_field 0x19c:rt:v0:t0 0x1ac:rd:v0:t0 0x1ac:rs:v0:t0 0x1b0:rs:v0:t0 0x1b0:rt:v0:t0 0x1b4:rt:t0:t1 0x1b8:rs:v0:t0 0x1c8:rt:t0:t1 */
/* geko_init -- GEKO overlay, Address: 0x801325D8, Size: 468 bytes */
#include "types.h"

extern void prim_init(void);
extern void sub_sub_step_up(void);
extern void draw2d3d(s32 a0, s32 a1);
extern void back_clear_switch(s32 a0);
extern void cdq_command_enqueue(s32, s32, s32);
extern void tpage_buf_clear(void);
extern void class_switch(s32 a0);
extern void class_clear_all(s32 a0);
extern void text_sys_init(void);
extern void k_reset(s32 a0);
extern void hizuke_init(void);
extern void message_window_init(void);
extern void bustup_ptr_init(void);
extern void date_name_set(s32 a0);
extern void yazirushi_off(void);
extern void shared_system_state_reset(void);
extern s32 rand(void);
extern void strcpy(s32 *a0, s32 *a1);
extern void geko_ptrs_set(void);
extern void geko_cell_scan(void);

typedef struct {
    s16 field_00;

    char pad[54];
} CharacterStats;

extern CharacterStats girl0_syoushin[];

extern u8 bg_change_tbl2;
extern u8 screen_bri;
extern u8 now_girl;
extern u8 schedule_kind[];
extern u8 schedule_cursor;
extern u8 vblank_flag;
extern s32 vb_ready;
extern u8 fade_bri;
extern s32 geko_ss_serifu;
extern s32 geko_ss_serifu_kind;
extern s32 geko_sss_sound;
extern s16 geko_now_serifu;
extern s16 geko_now_serifu_in;
extern u8 geko_serifu_state;
extern u8 geko_param_up_flag;
extern u8 geko_stat_hi;

extern u8 ba_string[];
extern u8 ba_string2[];
extern s32 geko_init_str0;
extern s32 geko_init_str1;
extern s32 geko_serifu_tbl[];
extern s32 geko_serifu_kind_tbl[];
extern s32 geko_sound_tbl[];

void geko_init(void)
{
    u8 idx;
    s16 stat;

    now_girl = schedule_kind[schedule_cursor * 4];
    draw2d3d(1, 0);
    class_switch(1);
    prim_init();
    class_clear_all(0);
    back_clear_switch(1);
    tpage_buf_clear();
    text_sys_init();
    strcpy(ba_string, &geko_init_str0);
    strcpy(ba_string2, &geko_init_str1);
    k_reset(1);
    vblank_flag = 0;
    vb_ready = 1;
    shared_system_state_reset();
    screen_bri = 128;
    fade_bri = 0;
    bg_change_tbl2 = 0;
    bustup_ptr_init();
    hizuke_init();
    message_window_init();
    yazirushi_off();
    geko_param_up_flag = 0;
    geko_now_serifu = 0;
    geko_now_serifu_in = 0;
    date_name_set(now_girl);
    geko_ptrs_set();
    geko_ss_serifu = geko_serifu_tbl[10];
    geko_ss_serifu_kind = geko_serifu_kind_tbl[10];
    geko_sss_sound = geko_sound_tbl[10];
    geko_cell_scan();

    if (geko_serifu_state == 0 && now_girl == 7) {
        if ((rand() & 3) == 0) {
            geko_serifu_state = 100;
        }
    }

    idx = now_girl;
    stat = girl0_syoushin[idx].field_00;
    if (stat >= 76) {
        geko_stat_hi = 1;
    } else {
        geko_stat_hi = 0;
    }

    sub_sub_step_up();
    cdq_command_enqueue(9, 0, 0);
}
