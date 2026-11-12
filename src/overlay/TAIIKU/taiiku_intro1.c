/* @SymbolSize: 1712 */
/* @RodataBase: 0x80148CE4 */
/* @PostProc: swap_instructions --offset1 0x124 --offset2 0x128 */
/* @PostProc: patch_insn_reg_field 0x11c:rt:v1:t1 0x120:rd:t1:t0 0x124:rd:t0:t3 0x124:rt:t1:t0 0x128:rs:v1:t1 0x128:rt:v1:t1 0x12c:rs:t0:t3 0x12c:rt:v1:t1 0x138:rt:v1:t1 0x164:rd:t3:t8 0x16c:rd:t7:t4 0x16c:rt:t3:t8 0x174:rs:t7:t4 0x1b0:rd:t8:t5 0x1b8:rd:t4:t6 0x1b8:rt:t8:t5 0x1c0:rs:t4:t6 0x1fc:rd:t5:t9 0x200:rd:t6:t2 0x200:rt:t5:t9 0x20c:rs:t6:t2 0x264:rt:t9:t0 0x268:rs:t9:t0 0x268:rt:t9:t0 0x270:rs:t9:t0 0x280:rt:t2:t1 0x288:rt:t2:t1 0x290:rt:t1:t3 0x294:rt:t1:t3 0x2a8:rd:t0:t7 0x2ac:rd:t3:t8 0x2ac:rt:t0:t7 0x2b0:rs:t3:t8 0x2b0:rt:t7:t4 0x2b4:rd:t8:t5 0x2b4:rt:t0:t7 0x2b8:rt:t7:t4 0x2bc:rs:t8:t5 0x2bc:rt:t4:t6 0x2c4:rt:t4:t6 0x2d0:rd:t5:t9 0x2d0:rt:t0:t7 0x2dc:rt:t5:t9 0x2e0:rt:t6:t2 0x2e4:rs:t6:t2 0x2e4:rt:t6:t2 0x2ec:rs:t6:t2 0x2ec:rt:t9:t0 0x2f0:rs:t9:t0 0x2f4:rt:t2:t1 0x308:rt:t2:t1 0x30c:rt:t2:t1 0x32c:rt:t1:t3 0x330:rs:t1:t3 0x330:rt:t1:t3 0x338:rs:t1:t3 0x340:rd:t1:t3 0x340:rt:t1:t3 0x348:rt:t1:t3 0x34c:rt:t1:t3 0x354:rs:t1:t3 0x37c:rt:t0:t7 0x380:rs:t0:t7 0x380:rt:t0:t7 0x388:rs:t0:t7 0x388:rt:t3:t8 0x390:rt:t3:t8 0x3a4:rt:t7:t4 0x3a8:rs:t7:t4 0x3a8:rt:t7:t4 0x3b0:rs:t7:t4 0x3b0:rt:t8:t5 0x3b8:rt:t8:t5 0x3ec:rt:t4:t6 0x3f0:rs:t4:t6 0x3f0:rt:t4:t6 0x3f8:rs:t4:t6 0x3f8:rt:t5:t9 0x400:rt:t5:t9 0x434:rt:t6:t2 0x438:rs:t6:t2 0x438:rt:t6:t2 0x440:rs:t6:t2 0x440:rt:t9:t0 0x448:rt:t9:t0 0x45c:rt:t0:t7 0x468:rt:t2:t1 0x46c:rs:t2:t1 0x46c:rt:t2:t1 0x474:rs:t2:t1 0x474:rt:t1:t3 0x47c:rt:t1:t3 0x498:rt:t0:t7 0x49c:rt:t0:t7 0x4b4:rt:t3:t8 0x4b8:rs:t3:t8 0x4b8:rt:t3:t8 0x4c0:rs:t3:t8 0x4d0:rt:t7:t4 0x4d8:rt:t7:t4 0x4e0:rt:t8:t5 0x4e4:rt:t8:t5 0x4f8:rd:t4:t6 0x4fc:rd:t5:t9 0x4fc:rt:t4:t6 0x500:rs:t5:t9 0x500:rt:t6:t2 0x504:rd:t9:t0 0x504:rt:t4:t6 0x508:rt:t6:t2 0x50c:rs:t9:t0 0x50c:rt:t2:t1 0x514:rt:t2:t1 0x520:rd:t1:t3 0x520:rt:t4:t6 0x52c:rt:t1:t3 0x530:rt:t0:t7 0x534:rs:t0:t7 0x534:rt:t0:t7 0x53c:rs:t0:t7 0x53c:rt:t3:t8 0x540:rs:t3:t8 0x568:rt:t7:t4 0x56c:rt:t7:t4 0x5b8:rt:t8:t5 0x5bc:rt:t8:t5 0x5ec:rt:t4:t6 0x5f0:rs:t4:t6 0x600:rt:t5:t9 0x60c:rt:t5:t9 0x2d4:rs:t5:t9 0x2d4:rt:t5:t9 0x524:rs:t1:t3 0x524:rt:t1:t3 */
/* Address: 0x80134060 */
/* Size: 1712 bytes (428 insns) */
#include "types.h"

typedef struct {
    s16 v[9];
} Tbl9;

typedef struct {
    u8  pad[4648];
    s32 slot[64];
} Big;

extern Big screen_w;
extern Tbl9 taiiku_kari_intro_x;
extern Tbl9 taiiku_kari_intro_y;
extern Tbl9 taiiku_kari_intro_anim;
extern u8 now_sub_sub_step;
extern u8 step_mode;
extern s32 taiiku_kari_crowd_tpg[];
extern s32 taiiku_kari_runner_tpg[];
extern s32 taiiku_kari_runner_tpg_6;
extern s32 taiiku_kari_sprite_tpg[];
extern s32 taiiku_com_tpg[];
extern s32 pad_trig;
extern u8 cal_cw_mode;
extern u8 cal_cw_disp;
extern s16 cal_cw_anim;
extern s16 cal_cw_frame;
extern s16 cal_cw_x;
extern s16 cal_cw_y;
extern s32 tpage_buf[];

extern void sub_step_up(void);
extern void sub_sub_step_up(void);
extern void goto_sub_sub_step(s32);
extern void device_cmd_send(s32);
extern s32 get_CD_State2(void);
extern s32 get_CD_State(void);
extern void set_XAsector(s32, s32, s32);
extern void cd_read_start2(s32, s32, s32);
extern void game_reset(void);
extern s32 search_load_tpage_buf_lock(s32, s32);
extern void class_clear_n(s32);
extern void Vblnk_Timer_Init(void);
extern s32 Vblnk_Timer(void);
extern void scroll_poly_disp(s32, s32, s32, s32, s32);
extern void Fade_Out_Init(void);
extern s32 bri_fade_out(void);
extern void fade_timer_init(void);
extern s32 Fade_In_Main(void);
extern s32 rand(void);
extern void taiiku_seat_init(void);
extern void taiiku_race_vram_init(void);
extern void taiiku_ptr4_init(void);
extern void taiiku_runner_init(void);
extern void taiiku_crowd_init(void);
extern void taiiku_crowd_set(void);
extern void taiiku_racer_target_set(void);

void taiiku_intro1(void)
{
    s32 i;
    s32 v;
    s32 val;
    Tbl9 a;
    Tbl9 b;
    Tbl9 c;

    a = taiiku_kari_intro_x;
    b = taiiku_kari_intro_y;
    c = taiiku_kari_intro_anim;

    switch (now_sub_sub_step) {
    case 0:
        if ((get_CD_State2() & 1) != 0) {
            v = search_load_tpage_buf_lock(taiiku_kari_runner_tpg_6, 1);
            screen_w.slot[v] = 0x80000206;
            taiiku_kari_runner_tpg_6 = 0x80000206;
            for (i = 0; i < 5; i++) {
                v = search_load_tpage_buf_lock(taiiku_kari_crowd_tpg[i], 1);
                val = 0x80000100 + i;
                screen_w.slot[v] = val;
                taiiku_kari_crowd_tpg[i] = val;
            }
            for (i = 0; i < 6; i++) {
                v = search_load_tpage_buf_lock(taiiku_kari_runner_tpg[i], 1);
                val = 0x80000200 + i;
                screen_w.slot[v] = val;
                taiiku_kari_runner_tpg[i] = val;
            }
            for (i = 0; i < 10; i++) {
                v = search_load_tpage_buf_lock(taiiku_kari_sprite_tpg[i], 1);
                val = 0x80000300 + i;
                screen_w.slot[v] = val;
                taiiku_kari_sprite_tpg[i] = val;
            }
            sub_sub_step_up();
        }
        break;
    case 1:
        fade_timer_init();
        device_cmd_send(516);
        sub_sub_step_up();
        break;
    case 2:
        if (Fade_In_Main() != 0) {
            sub_sub_step_up();
        }
        if (cal_cw_mode == 0) {
            v = rand();
            cal_cw_mode = 5;
            cal_cw_disp = 164;
            v = v % 9;
            cal_cw_x = a.v[v];
            cal_cw_y = b.v[v];
            cal_cw_frame = 0;
            cal_cw_anim = c.v[v];
        }
        if ((pad_trig & 0xF0) != 0) {
            Fade_Out_Init();
            goto_sub_sub_step(4);
        }
        scroll_poly_disp(0, 15, 0, 240, 640);
        break;
    case 3:
        switch (step_mode) {
        case 0:
            set_XAsector(0x1F000361, 0x4100037C, 0);
            device_cmd_send(768);
            step_mode++;
            break;
        case 1:
        case 3:
        case 5:
            if (get_CD_State() == 4) {
                step_mode++;
            }
            break;
        case 2:
            if (get_CD_State() == 2) {
                set_XAsector(0x1F00037D, 0x42000389, 0);
                device_cmd_send(768);
                step_mode++;
            }
            break;
        case 4:
            if (get_CD_State() == 2) {
                set_XAsector(0x1F00038A, 0x43000393, 0);
                device_cmd_send(768);
                step_mode++;
            }
            break;
        case 6:
            if (get_CD_State() == 2) {
                Vblnk_Timer_Init();
                step_mode++;
            }
            break;
        case 7:
            if ((u32)Vblnk_Timer() >= 181U) {
                step_mode = 0;
            }
            break;
        }
        scroll_poly_disp(0, 15, 0, 240, 640);
        if (cal_cw_mode == 0) {
            v = rand();
            cal_cw_mode = 5;
            cal_cw_disp = 164;
            v = v % 9;
            cal_cw_x = a.v[v];
            cal_cw_y = b.v[v];
            cal_cw_frame = 0;
            cal_cw_anim = c.v[v];
        }
        if ((pad_trig & 0xF0) != 0) {
            Fade_Out_Init();
            device_cmd_send(116);
            sub_sub_step_up();
        }
        break;
    case 4:
        scroll_poly_disp(0, 15, 0, 240, 640);
        if (get_CD_State() == 2) {
            cd_read_start2(135, 0x8018F000, 0xB996);
            sub_sub_step_up();
        }
        break;
    case 5:
        scroll_poly_disp(0, 15, 0, 240, 640);
        if (bri_fade_out() != 0) {
            if ((get_CD_State2() & 1) != 0) {
                class_clear_n(96);
                tpage_buf[21] = -1;
                taiiku_ptr4_init();
                for (i = 0; i < 6; i++) {
                    search_load_tpage_buf_lock(taiiku_com_tpg[i], 1);
                }
                sub_sub_step_up();
            }
        }
        break;
    case 6:
        taiiku_race_vram_init();
        taiiku_seat_init();
        taiiku_runner_init();
        taiiku_crowd_init();
        taiiku_crowd_set();
        taiiku_racer_target_set();
        device_cmd_send(1285);
        fade_timer_init();
        sub_step_up();
        break;
    default:
        game_reset();
        break;
    }
}
