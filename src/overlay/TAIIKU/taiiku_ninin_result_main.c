/* @CompileFlags: -Wc,-pic1 */
/* @SymbolSize: 992 */
/* @PostProc: patch_stack_complete --old 64 --new 72 */
/* @RodataBase: 0x8014902C */
/* Address: 0x8014318C */
/* Size: 992 bytes (248 insns) */
#include "types.h"

typedef struct {
    u8 b[56];
} Rec56;

extern void sub_step_up(void);
extern void sub_sub_step_up(void);
extern void device_cmd_send(s32 a0);
extern void game_reset(void);
extern void Vblnk_Timer_Init(void);
extern s32  Vblnk_Timer(void);
extern void scroll_poly_disp(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4);
extern void Fade_Out_Init(void);
extern s32  bri_fade_out(void);
extern void fade_timer_init(void);
extern s32  Fade_In_Main(void);
extern void taiiku_xa_cell_set(u8 a0, u8 a1);
extern void taiiku_anim_offset(void);
extern void taiiku_result_frame_step(void);
extern void taiiku_blink_toggle(void);

extern Rec56 screen_w[];
extern u8  now_sub_sub_step;
extern u8  taiiku_ninin_girl1;
extern u8  taiiku_ninin_blink;
extern u8 taiiku_ninin_rank[];

void taiiku_ninin_result_main(void)
{
    u8 *p;
    s32 i;

    scroll_poly_disp(0, 15, 0, 240, 640);

    switch ((u32)now_sub_sub_step) {
    case 0:
        fade_timer_init();
        sub_sub_step_up();
        return;

    case 1:
        if (Fade_In_Main() != 0) {
            if (taiiku_ninin_rank[0] == 1 || taiiku_ninin_rank[1] == 1) {
                device_cmd_send(517);
            } else {
                device_cmd_send(518);
            }
            Vblnk_Timer_Init();
            sub_sub_step_up();
        }
        return;

    case 2:
        if ((u32)Vblnk_Timer() >= 61U) {
            taiiku_anim_offset();
            Vblnk_Timer_Init();
            sub_sub_step_up();
        }
        return;

    case 3:
        if ((u32)Vblnk_Timer() >= 61U) {
            taiiku_result_frame_step();
            if (taiiku_ninin_girl1 != 0) {
                p = &taiiku_ninin_rank[0];
                i = 0;
                do {
                    if (*p == 1) {
                        taiiku_xa_cell_set(1, (u8)i);
                    }
                    i = i + 1;
                    p = p + 1;
                } while (i != 4);
            } else {
                i = 0;
                p = &taiiku_ninin_rank[0];
                do {
                    if (*p == 1) {
                        taiiku_xa_cell_set(0, (u8)i);
                    }
                    i = i + 1;
                    p = p + 1;
                } while (i != 4);
            }
            if (i) { }
            if (p) { }
            Vblnk_Timer_Init();
            taiiku_ninin_blink = 0;
            sub_sub_step_up();
        }
        return;

    case 4:
        taiiku_ninin_blink = taiiku_ninin_blink + 1;
        taiiku_blink_toggle();
        if ((u32)Vblnk_Timer() >= 241U) {
            Fade_Out_Init();
            sub_sub_step_up();
        }
        return;

    case 5:
        if (bri_fade_out() != 0) {
            if (taiiku_ninin_rank[0] == 1) {
                *(s16 *)((u8 *)&screen_w[0] + 446) = *(s16 *)((u8 *)&screen_w[0] + 446) + 4;
                for (i = 1; i < 13; i++) {
                    *(s16 *)((u8 *)&screen_w[i] + 446) =
                        *(s16 *)((u8 *)&screen_w[i] + 446) + 4;
                }
                if (taiiku_ninin_girl1 != 0) {
                    *(s16 *)((u8 *)&screen_w[1] + 446) = *(s16 *)((u8 *)&screen_w[1] + 446) + 2;
                    *(s16 *)((u8 *)&screen_w[1] + 450) = *(s16 *)((u8 *)&screen_w[1] + 450) + 10;
                } else {
                    *(s16 *)((u8 *)&screen_w[0] + 446) = *(s16 *)((u8 *)&screen_w[0] + 446) + 2;
                    *(s16 *)((u8 *)&screen_w[0] + 450) = *(s16 *)((u8 *)&screen_w[0] + 450) + 10;
                }
            } else if (taiiku_ninin_rank[1] == 1) {
                *(s16 *)((u8 *)&screen_w[0] + 446) = *(s16 *)((u8 *)&screen_w[0] + 446) + 2;
                for (i = 1; i < 13; i++) {
                    *(s16 *)((u8 *)&screen_w[i] + 446) =
                        *(s16 *)((u8 *)&screen_w[i] + 446) + 2;
                }
                if (taiiku_ninin_girl1 != 0) {
                    *(s16 *)((u8 *)&screen_w[1] + 446) = *(s16 *)((u8 *)&screen_w[1] + 446) + 1;
                    *(s16 *)((u8 *)&screen_w[1] + 450) = *(s16 *)((u8 *)&screen_w[1] + 450) + 5;
                } else {
                    *(s16 *)((u8 *)&screen_w[0] + 446) = *(s16 *)((u8 *)&screen_w[0] + 446) + 1;
                    *(s16 *)((u8 *)&screen_w[0] + 450) = *(s16 *)((u8 *)&screen_w[0] + 450) + 5;
                }
            }
            sub_step_up();
        }
        return;

    default:
        game_reset();
        return;
    }
}
