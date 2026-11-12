/* @SymbolSize: 1392 */
/* @CompileEnv: IDO52_AS1_NO_LA_PULL=0 IDO52_AS1_XBB_VOLATILE_OK=6 IDO52_FORCE_WEB_COLOR=2:v1 */
/* @RodataBase: 0x80139540 */
/* @PostProc: patch_insn_reg_field 0x220:rt:t3:t2 0x224:rt:t2:t5 0x228:rt:t5:t4 0x22c:rt:t4:t3 0x234:rt:t2:t5 0x23c:rt:t5:t4 0x240:rt:t4:t3 0x244:rt:t3:t2 0x27c:rt:t8:t9 0x280:rt:t9:t2 0x284:rt:t2:t5 0x290:rt:t5:t8 0x2a0:rt:t5:t8 0x2c0:rt:t8:t9 0x2c4:rt:t9:t2 0x2cc:rt:t2:t5 */
/* en_nichi_stage_setup_main - EN_NICHI overlay, Address: 0x80137134, 348 insns. */
#include "types.h"

typedef struct {
    u8  u00;
    u8  u01;
    u8  u02;
    u8  u03;
    u8  u04;
    u8  u05;
    u8  u06;
    u8  u07;
    u8  pad08[4];
    u32 u0C;
    u32 u10;
    s16 u14;
    s16 u16;
    s16 u18;
    s16 u1A;
    s16 u1C;
    u8  pad1E[8];
    s16 u26;
    u8  pad28[2];
    s16 u2A;
    u8  pad2C[8];
    u32 u34;
    u32 u38;
    u8  pad3C[8];
} EnNichiSpr;

typedef struct { s16 v[22]; } Tbl16;
typedef struct { u8  v[22]; } Tbl8;

extern EnNichiSpr class_w[];
extern Tbl16 en_nichi_main1_tbl0;
extern Tbl16 en_nichi_main1_tbl1;
extern Tbl16 en_nichi_main1_tbl2;
extern Tbl8  en_nichi_main1_tbl3;

extern s32 *en_nichi_tpage_tbl_ptr;
extern u32 en_nichi_cels_ptr;
extern u32 en_nichi_anim_ptr;
extern s16 en_nichi_aux_wk;

extern u8 now_sub_sub_step;
extern u8 step_mode;
extern u8 screen_bri;
extern volatile u8 cal_cw_bright;
extern s16 cursor_cw_x;
extern s16 cursor_cw_y;
extern u8  en_nichi_shot_left;
extern u8  en_nichi_shot_state[];
extern u8  en_nichi_shot_spd[];
extern u8  en_nichi_shot_wait[];
extern s16 en_nichi_bar_len;

extern void load_tpage_buf_lock(s32, s32, s32);
extern void search_load_tpage_buf_lock(s32, s32);
extern void sub_sub_step_up(void);
extern void sub_step_up(void);
extern s32  get_CD_State(void);
extern void game_reset(void);
extern void set_XAsector(s32, s32, s32);
extern void device_cmd_send(s32);
extern void class_clear_n(s32);

void en_nichi_stage_setup_main(void)
{
    s32 i;
    u8 st;
    Tbl16 d;
    Tbl16 c;
    Tbl16 b;
    Tbl8  a;

    st = now_sub_sub_step;

    d = en_nichi_main1_tbl0;
    c = en_nichi_main1_tbl1;
    b = en_nichi_main1_tbl2;
    a = en_nichi_main1_tbl3;

    switch ((u32)st) {
    case 0:
        load_tpage_buf_lock(5, en_nichi_tpage_tbl_ptr[7], 1);
        for (i = 0; i < 7; i++) {
            search_load_tpage_buf_lock(en_nichi_tpage_tbl_ptr[i], 1);
        }
        for (i = 8; i < 10; i++) {
            search_load_tpage_buf_lock(en_nichi_tpage_tbl_ptr[i], 1);
        }
        sub_sub_step_up();
        break;
    case 1:
        for (i = 0; i < 22; i++) {
            class_clear_n(96 + i);
            class_w[96 + i].u38 = 0x01000000;
            class_w[96 + i].u07 = 0;
            class_w[96 + i].u02 = 64;
            class_w[96 + i].u03 = 4;
            class_w[96 + i].u01 = a.v[i];
            class_w[96 + i].u0C = en_nichi_cels_ptr;
            class_w[96 + i].u10 = en_nichi_anim_ptr;
            class_w[96 + i].u34 = (u32)en_nichi_tpage_tbl_ptr;
            class_w[96 + i].u14 = en_nichi_aux_wk;
            class_w[96 + i].u18 = d.v[i];
            class_w[96 + i].u26 = c.v[i];
            class_w[96 + i].u2A = b.v[i];
            class_w[96 + i].u04 = 0;
            class_w[96 + i].u05 = 128;
            class_w[96 + i].u1A = 4096;
            class_w[96 + i].u1C = 4096;
            class_w[96 + i].u06 = 0;
        }
        for (i = 0; i < 21; i++) {
            class_w[i + 96].u03 |= 0x80;
        }
        sub_sub_step_up();
        break;
    case 2:
        if (cal_cw_bright >= 128U) {
            screen_bri = 128;
            cal_cw_bright = 128;
            sub_sub_step_up();
        } else {
            cal_cw_bright = (u8)(cal_cw_bright + 4);
        }
        for (i = 1; i < 22; i++) {
            class_w[i + 96].u07 = cal_cw_bright;
        }
        break;
    case 3:
        switch ((u32)step_mode) {
        case 0:
            set_XAsector(0x07000002, 0x03000008, 14);
            device_cmd_send(768);
            step_mode = (u8)(step_mode + 1);
            break;
        case 1:
        case 3:
            if (get_CD_State() == 4) {
                step_mode = (u8)(step_mode + 1);
            }
            break;
        case 2:
            if (get_CD_State() == 2U) {
                set_XAsector(0x08000001, 0x02000004, 14);
                device_cmd_send(768);
                step_mode = (u8)(step_mode + 1);
            }
            break;
        case 4:
            if (get_CD_State() == 2U) {
                en_nichi_bar_len = 0;
                en_nichi_shot_left = 5;
                for (i = 0; i < 5; i++) {
                    en_nichi_shot_state[i] = 0;
                    en_nichi_shot_spd[i] = 0;
                    en_nichi_shot_wait[i] = 0;
                }
                cursor_cw_x = 0;
                cursor_cw_y = 0;
                sub_step_up();
            }
            break;
        }
        break;
    default:
        game_reset();
        break;
    }
}
