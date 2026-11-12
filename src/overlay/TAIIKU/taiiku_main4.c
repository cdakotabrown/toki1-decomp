/* Address: 0x801429E8 (real entry 0x801429E0, cross-prologue selector hoist) */
/* Size: 1412 bytes (353 insns incl. 2 hoisted selector-load insns) */
/* TEN WORDS TOO LONG.  `@SymbolSize: 1412` sliced the overrun away, */
/* @CompileFlags: -Wo,-loopunroll,0 -Wc,-pic0 */
/* @CompileEnv: IDO52_COPYPROP_VAR=-24 IDO52_AS1_XBB_SKIP_CAND=occ:1,2 IDO52_AS1_TIE_REJECT=248 */
/* @PostProc: swap_instructions --offset1 0x4e8 --offset2 0x4f0 */
/* @PostProc: remap_stack_slots --map '36:48,44:48' */
/* @RodataBase: 0x80148FF0 */
/* @PostProc: swap_instructions --offset1 0x52c --offset2 0x530 */
/* @PostProc: patch_insn_reg_field 0x1d4:rt:v1:v0 0x1e0:rs:v1:v0 0x1e0:rt:v1:v0 0x1e4:rs:v1:v0 0x1f8:rs:v1:v0 0x2e8:rt:t4:v0 0x2ec:rs:t4:v0 0x2ec:rt:t4:v0 0x2f4:rs:t4:v0 0x334:rt:t5:t4 0x338:rs:t5:t4 0x338:rt:t5:t4 0x340:rs:t5:t4 0x340:rt:t6:t5 0x348:rt:t6:t5 0x35c:rt:t7:v0 0x360:rs:t7:v0 0x360:rt:t7:v0 0x368:rs:t7:v0 0x3a8:rt:t8:t6 0x3ac:rs:t8:t6 0x3ac:rt:t8:t6 0x3b4:rs:t8:t6 0x3b4:rt:t9:t7 0x3bc:rt:t9:t7 0x3dc:rt:t0:t8 0x3e0:rs:t0:t8 0x3e0:rt:t0:t8 0x3e8:rs:t0:t8 0x3e8:rt:t1:t9 0x3f0:rt:t1:t9 0x40c:rt:t2:t0 0x410:rs:t2:t0 0x410:rt:t2:t0 0x418:rs:t2:t0 0x418:rt:t3:t1 0x41c:rs:t3:t1 0x444:rt:t4:t2 0x448:rt:t4:t2 0x494:rt:t5:t3 0x498:rt:t5:t3 0x4c8:rt:t6:t4 0x4cc:rs:t6:t4 0x4f8:rt:t7:t5 0x4fc:rs:t7:t5 0x4fc:rt:t7:t5 0x504:rt:t7:t5 0x524:rt:v1:v0 0x530:rs:v1:v0 0x530:rt:v1:v0 0x534:rt:v0:v1 0x538:rs:v1:v0 0x538:rt:v1:v0 0x53c:rs:v1:v0 0x540:rs:v1:v0 0x540:rt:v0:v1 */
/* @SymbolStart: 0x801429E0 */
/* @SymbolSize: 1412 */
#include "types.h"

extern s32 sub_step_up();
extern s32 sub_sub_step_up();
extern s32 goto_sub_sub_step();
extern s32 device_cmd_send();
extern s32 device_port_init();
extern s32 get_CD_State2();
extern s32 get_CD_State();
extern s32 set_XAsector();
extern s32 cd_read_start2();
extern s32 game_reset();
extern s32 search_load_tpage_buf_lock();
extern s32 class_clear_n();
extern s32 Vblnk_Timer_Init();
extern s32 Vblnk_Timer();
extern s32 scroll_poly_disp();
extern s32 Fade_Out_Init();
extern s32 bri_fade_out();
extern s32 fade_timer_init();
extern s32 Fade_In_Main();
extern void taiiku_seat_init();
extern void taiiku_race_vram_init();
extern s32 taiiku_ptr4_init();
extern void taiiku_sprite_init2();

extern u8 now_sub_sub_step;
extern u8 step_mode;
extern s32 pad_trig;
extern s32 tpage_buf[];
extern s32 screen_w[];
extern s32 bukatu_tbl[];
extern u8 taiiku_ninin_girl1;
extern s16 cal_cw_anim;
extern s16 cal_cw_y;
extern s32 class_w[];
extern s32 taiiku_com_tpg[];
extern s32 taiiku_com_tpg_src[];
extern s32 taiiku_ninin_sprite_tpg[];
extern s32 taiiku_ninin_rank_tpg[];

void taiiku_main4(void)
{
    u8 sel;
    s32 i;
    s32 v0;
    s32 v1;
    s32 *p;
    s32 *q;

    sel = now_sub_sub_step;
    switch ((u32)sel) {
    case 0:
        if (get_CD_State2() & 1) {
            for (i = 0; i < 8; i++) {
                v0 = search_load_tpage_buf_lock(taiiku_ninin_sprite_tpg[i], 1);
                v1 = i + 0x80000100;
                tpage_buf[v0] = v1;
                taiiku_ninin_sprite_tpg[i] = v1;
            }
            for (i = 0; i != 11; i++) {
                v0 = search_load_tpage_buf_lock(taiiku_ninin_rank_tpg[i], 1);
                v1 = i + 0x80000200;
                tpage_buf[v0] = v1;
                taiiku_ninin_rank_tpg[i] = v1;
            }
            sub_sub_step_up();
        }
        break;
    case 1:
        fade_timer_init();
        device_port_init(1, 3);
        sub_sub_step_up();
        break;
    case 2:
        if (Fade_In_Main() != 0) {
            sub_sub_step_up();
        }
        if (pad_trig & 0xF0) {
            Fade_Out_Init();
            cd_read_start2(135, 0x8018F000, 0xB996);
            goto_sub_sub_step(5);
        }
        scroll_poly_disp(0, 15, 0, 240, 640);
        break;
    case 3:
        scroll_poly_disp(0, 15, 0, 240, 640);
        q = class_w;
        if (cal_cw_y >= 97) {
            q[1642] = q[1642] + 0xFFFE8000;
        } else {
            v0 = taiiku_ninin_girl1;
            if (v0 == 0) {
                cal_cw_anim = 1;
            } else {
                cal_cw_anim = 3;
            }
            switch ((u32)step_mode) {
            case 0:
                if (v0 != 0) {
                    set_XAsector(0x1E000050, 0x06000060, 1);
                } else {
                    set_XAsector(0x1F000394, 0x440003A4, 0);
                }
                device_cmd_send(768);
                step_mode = step_mode + 1;
                break;
            case 1:
            case 3:
            case 5:
                if (get_CD_State() == 4) {
                    step_mode = step_mode + 1;
                }
                break;
            case 2:
                if (get_CD_State() == 2) {
                    if (taiiku_ninin_girl1 != 0) {
                        set_XAsector(0x1F00009F, 0x140000A9, 1);
                    } else {
                        set_XAsector(0x1F0003A5, 0x450003B1, 0);
                    }
                    device_cmd_send(768);
                    step_mode = step_mode + 1;
                }
                break;
            case 4:
                if (get_CD_State() == 2) {
                    if (taiiku_ninin_girl1 != 0) {
                        set_XAsector(0x1F0000AA, 0x150000B7, 1);
                    } else {
                        set_XAsector(0x0A0003C3, 0x430003D0, 0);
                    }
                    device_cmd_send(768);
                    step_mode = step_mode + 1;
                }
                break;
            case 6:
                if (get_CD_State() == 2) {
                    Vblnk_Timer_Init();
                    step_mode = step_mode + 1;
                }
                break;
            case 7:
                if ((u32)Vblnk_Timer() >= 181) {
                    step_mode = 0;
                }
                break;
            }
        }
        if (pad_trig & 0xF0) {
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
            if (get_CD_State2() & 1) {
                taiiku_ptr4_init();
                p = taiiku_com_tpg;
                do {
                    search_load_tpage_buf_lock(*p, 1);
                    p++;
                } while (p != taiiku_com_tpg_src);
                sub_sub_step_up();
            }
        }
        break;
    case 6:
        class_clear_n(96);
        p = screen_w;
        do {
            p++;
            p[1182] = -1;
        } while (p != bukatu_tbl);
        taiiku_race_vram_init();
        taiiku_seat_init();
        taiiku_sprite_init2();
        device_cmd_send(1285);
        fade_timer_init();
        sub_step_up();
        break;
    default:
        game_reset();
        break;
    }
}
