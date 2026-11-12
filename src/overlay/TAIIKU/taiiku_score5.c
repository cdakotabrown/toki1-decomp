/* @SymbolStart: 0x80139858 */
/* @SymbolSize: 1040 */
/* @RodataBase: 0x80148D90 */
/* @PostProc: patch_stack_complete --old 48 --new 64 */
/* @PostProc: remap_stack_slots --map '44:52,36:56' */
/* Address: 0x80139860 (window starts at 0x80139858 -- see ABSORPTION below) */
/* Size: 1032 bytes (258 insns) + 8-byte absorbed head = 1040 (260 insns) */
#include "types.h"

typedef struct {
    u8 b[56];
} Rec56;

extern void sub_step_up(void);
extern void sub_sub_step_up(void);
extern void device_cmd_send(s32 a0);
extern void game_reset(void);
extern void class_clear_f(void);
extern void Vblnk_Timer_Init(void);
extern s32  Vblnk_Timer(void);
extern void scroll_poly_disp(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4);
extern void Fade_Out_Init(void);
extern s32  bri_fade_out(void);
extern void fade_timer_init(void);
extern s32  Fade_In_Main(void);
extern void taiiku_result_sprite_init(void);
extern void taiiku_xa_cell_set(s32 a0, u8 a1);
extern void taiiku_run_speed(void);
extern void taiiku_course_disp(void);
extern void taiiku_mtx_set(void);
extern void taiiku_plate_init(void);
extern void taiiku_y_step(void);
extern void taiiku_result_frame_add(void);
extern void taiiku_result_frame_blink(void);

extern Rec56 screen_w[];
extern u8  screen_bri;
extern s16 girl0_tokimeki;
extern u8  now_sub_sub_step;
extern s32 taiiku_tama_girl;
extern s32 taiiku_tama_scale;
extern u8 taiiku_tama_rank[];
extern u8  taiiku_tama_rtick;

void taiiku_score5(void)
{
    u8 *p;
    s32 i;
    s32 k;
    s32 v;

    switch ((u32)now_sub_sub_step) {
    case 0:
        if (taiiku_tama_scale < 128) {
            class_clear_f();
            taiiku_tama_scale = 0;
            Fade_Out_Init();
            screen_bri = 0;
            sub_sub_step_up();
        }
        taiiku_mtx_set();
        taiiku_course_disp();
        taiiku_run_speed();
        return;

    case 1:
        taiiku_plate_init();
        fade_timer_init();
        taiiku_result_sprite_init();
        sub_sub_step_up();
        return;

    case 2:
        if (Fade_In_Main() != 0) {
            if (taiiku_tama_rank[0] == 0 || taiiku_tama_rank[1] == 0) {
                device_cmd_send(517);
            } else {
                device_cmd_send(518);
            }
            Vblnk_Timer_Init();
            sub_sub_step_up();
        }
        scroll_poly_disp(0, 15, 0, 240, 192);
        return;

    case 3:
        if ((u32)Vblnk_Timer() >= 91U) {
            k = 0;
            p = &taiiku_tama_rank[0];
            do {
                if (*p == 0) {
                    taiiku_xa_cell_set(taiiku_tama_girl, (u8)k);
                }
                k = k + 1;
                p = p + 1;
            } while (k != 4);
            if (k) { }
            taiiku_y_step();
            taiiku_result_frame_add();
            taiiku_tama_rtick = 0;
            Vblnk_Timer_Init();
            sub_sub_step_up();
        }
        scroll_poly_disp(0, 15, 0, 240, 192);
        return;

    case 4:
        taiiku_tama_rtick = taiiku_tama_rtick + 1;
        taiiku_result_frame_blink();
        if ((u32)Vblnk_Timer() >= 181U) {
            Vblnk_Timer_Init();
            sub_sub_step_up();
        }
        scroll_poly_disp(0, 15, 0, 240, 192);
        return;

    case 5:
        if ((u32)Vblnk_Timer() >= 241U) {
            Fade_Out_Init();
            sub_sub_step_up();
        }
        scroll_poly_disp(0, 15, 0, 240, 192);
        return;

    case 6:
        if (bri_fade_out() != 0) {
            if (taiiku_tama_rank[0] == 0) {
                girl0_tokimeki = girl0_tokimeki + 4;
                for (i = 1; i < 13; i++) {
                    *(s16 *)((u8 *)&screen_w[i] + 446) =
                        *(s16 *)((u8 *)&screen_w[i] + 446) + 4;
                }
                v = taiiku_tama_girl;
                *(s16 *)((u8 *)&screen_w[v] + 446) =
                    *(s16 *)((u8 *)&screen_w[v] + 446) + 2;
                *(s16 *)((u8 *)&screen_w[v] + 450) =
                    *(s16 *)((u8 *)&screen_w[v] + 450) + 10;
            } else if (taiiku_tama_rank[1] == 0) {
                girl0_tokimeki = girl0_tokimeki + 2;
                for (i = 1; i < 13; i++) {
                    *(s16 *)((u8 *)&screen_w[i] + 446) =
                        *(s16 *)((u8 *)&screen_w[i] + 446) + 2;
                }
                v = taiiku_tama_girl;
                *(s16 *)((u8 *)&screen_w[v] + 446) =
                    *(s16 *)((u8 *)&screen_w[v] + 446) + 1;
                *(s16 *)((u8 *)&screen_w[v] + 450) =
                    *(s16 *)((u8 *)&screen_w[v] + 450) + 5;
            }
            sub_step_up();
        }
        scroll_poly_disp(0, 15, 0, 240, 192);
        return;

    default:
        game_reset();
        return;
    }
}
