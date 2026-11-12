/* @SymbolSize: 844 */
/* @CompileEnv: IDO52_AS1_FORCE_NARROW_FOLD=3 */
/* @PostProc: swap_instructions --offset1 0x18 --offset2 0x24 */
/* @PostProc: patch_insn_reg_field 0x3c:rt:s5:s6 0x60:rd:t8:t9 0x64:rs:t8:t9 0x7c:rt:s5:s6 0x84:rt:s6:s5 0x88:rt:s5:s6 0xbc:rd:t9:t0 0xc0:rd:t0:t1 0xc4:rs:t9:t0 0xc4:rt:t0:t1 0xd4:rs:s6:s5 0xd4:rt:s5:s6 0xdc:rd:t1:t2 0xe0:rt:t1:t2 0x100:rt:t3:t5 0x104:rs:t3:t5 0x13c:rt:t4:t6 0x140:rs:t4:t6 0x140:rt:t4:t6 0x14c:rt:t5:t7 0x150:rs:t5:t7 0x150:rt:t5:t7 0x154:rt:t4:t6 0x15c:rt:t6:t8 0x160:rs:t6:t8 0x160:rt:t6:t8 0x164:rt:t5:t7 0x170:rt:t6:t8 0x188:rd:t7:t9 0x18c:rt:t7:t9 0x1bc:rt:t0:t2 0x1c0:rs:t0:t2 0x1c0:rt:t0:t2 0x1c4:rt:t9:t1 0x1cc:rt:t9:t1 0x1d4:rs:t0:t2 0x1d4:rt:t1:t3 0x1d8:rt:t1:t3 0x240:rt:t2:t4 0x244:rt:t2:t4 0x298:rt:t3:t5 0x29c:rs:t3:t5 0x29c:rt:t3:t5 0x2a4:rt:t4:t6 0x2a8:rs:t4:t6 0x2a8:rt:t4:t6 0x2ac:rt:t3:t5 0x2b4:rt:t5:t7 0x2b8:rs:t5:t7 0x2b8:rt:t5:t7 0x2bc:rt:t4:t6 0x2c4:rt:t5:t7 0x2f4:rt:t6:t8 0x2f8:rs:t6:t8 0x2f8:rt:t6:t8 0x2fc:rt:t7:t9 0x300:rs:t6:t8 0x308:rs:t7:t9 0x308:rt:t7:t9 0x30c:rt:t8:t0 0x310:rs:t7:t9 0x318:rs:t8:t0 0x318:rt:t8:t0 0x320:rs:t8:t0 0x320:rt:t9:t1 0x324:rt:t9:t1 */
/* Address: 0x80138F90 */
/* Size: 844 bytes (211 insns) */
#include "types.h"

extern s32 get_g_zyotai_s(s32);
extern s32 get_h_yuukou(s32);
extern s32 get_h_tokimeki(s32);
extern void event_log_add(s32, s32);
extern void shared_system_state_reset(void);
extern void icon_init(void);
extern void icon_can_use_set(s32, s32);
extern void icon_cursor_move(s32);
extern void parameter_show_init(void);
extern void date_name_set(s32);
extern void draw2d3d(s32, s32);
extern void class_switch(s32);
extern void prim_init(void);
extern void back_clear_switch(s32);
extern void class_clear_all(s32);
extern void message_window_init(void);
extern void hizuke_init(void);
extern void tpage_buf_clear(void);
extern void text_sys_init(void);
extern void parameter_disp_switch(s32);
extern void icon_disp_switch(s32);
extern void bustup_ptr_init(void);
extern void yazirushi_off(void);
extern void kangei_ptr_tbl2_init(void);
extern void kangei_girl_msg_set(void);
extern s32 sub_sub_step_up(void);

extern u8 girl0_flag[];
extern u8 now_girl;
extern u8 vblank_flag;
extern s32 vb_ready;
extern u8 now_sub_sub_step;
extern u8 fade_bri;
extern u8 kangei_icon_flag;
extern u8 kangei_birth_done;
extern s32 kangei_ss_s;
extern s32 kangei_ss_s_kind;
extern s32 kangei_sss_so;
extern s16 kangei_now_s;
extern s16 kangei_now_s_in;
extern u8 kangei_birth_flag;
extern s32 kangei_msg_res_tbl[][8];
extern s32 kangei_bank1_tbl[][13];

s32 kangei_girl_select(void)
{
    s16 i;
    s32 bestChar;
    s32 bestInput;
    u8  prevChar;
    s32 statA;
    s32 statB;
    s32 statC;
    u8 cur;
    s32 pick;
    s16 k;

    bestChar = 255;
    bestInput = 4;

    for (i = 0; i < 11; i++) {
        if ((*(s32 *)(girl0_flag + i * 0x38) << 30) < 0) {
            cur = get_g_zyotai_s(i) & 0x7F;
            prevChar = bestChar;
            if ((u32)cur < (u32)bestInput ||
                ((u32)cur == (u32)bestInput &&
                 (statA = (u8)get_h_yuukou(i),
                  statB = (u8)get_h_tokimeki(i),
                  statC = (u8)get_h_tokimeki(prevChar),
                  (u32)(statB + statA) >
                      (u32)(get_h_yuukou(prevChar) + statC)))) {
                bestChar = i & 0xFF;
                bestInput = cur & 0xFF;
            }
        }
    }

    pick = get_g_zyotai_s(bestChar) & 0x7F;

    if (bestChar != 255 && (u32)pick < 2) {
        now_girl = bestChar;
        event_log_add(13, 42);
    } else {
        shared_system_state_reset();

        kangei_now_s = 0;
        kangei_now_s_in = 0;

        kangei_ss_s = kangei_bank1_tbl[0][11];
        kangei_ss_s_kind = kangei_bank1_tbl[1][11];
        kangei_sss_so = kangei_bank1_tbl[2][11];

        icon_init();

        for (k = 0; k < 15; k++) {
            icon_can_use_set(k, 0);
        }

        icon_cursor_move(1);
        parameter_show_init();
        parameter_show_init();
        date_name_set(0);

        kangei_icon_flag = 1;
        now_sub_sub_step = now_sub_sub_step + 30;
        return 0;
    }

    now_girl = bestChar;
    draw2d3d(1, 0);
    class_switch(1);
    prim_init();
    back_clear_switch(1);
    class_clear_all(0);
    message_window_init();
    hizuke_init();
    tpage_buf_clear();
    text_sys_init();

    vblank_flag = 0;
    vb_ready = 1;
    fade_bri = 0;

    shared_system_state_reset();
    parameter_disp_switch(0);
    icon_disp_switch(0);

    kangei_now_s = 0;
    kangei_now_s_in = 0;

    bustup_ptr_init();
    yazirushi_off();

    date_name_set(now_girl);
    kangei_ptr_tbl2_init();

    kangei_ss_s = kangei_msg_res_tbl[0][6];
    kangei_ss_s_kind = kangei_msg_res_tbl[1][6];
    kangei_sss_so = kangei_msg_res_tbl[2][6];

    kangei_icon_flag = 0;
    kangei_now_s = 0;
    kangei_now_s_in = 0;

    date_name_set(now_girl);
    kangei_girl_msg_set();

    if (kangei_birth_done != 0 && kangei_birth_flag == 0) {
        now_sub_sub_step = now_sub_sub_step + 2;
    }

    return sub_sub_step_up();
}
