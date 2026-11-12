/* @SymbolStart: 0x80145804 */
/* @SymbolSize: 1080 */
/* @RodataBase: 0x80149050 */
/* @PostProc: patch_stack_complete --old 48 --new 64 */
/* @PostProc: remap_stack_slots --map '32:48,44:56' */
/* Address: 0x8014580C (window starts at 0x80145804 -- see ABSORPTION below) */
/* Size: 1072 bytes (268 insns) + 8-byte absorbed head = 1080 (270 insns) */
#include "types.h"

extern void sub_step_up(void);
extern void sub_sub_step_up(void);
extern void goto_sub_sub_step(s32 a0);
extern void device_cmd_send(s32 a0);
extern s32  get_CD_State2(void);
extern s32  get_CD_State(void);
extern void set_XAsector(s32 a0, s32 a1, s32 a2);
extern void cd_read_start2(s32 a0, s32 a1, s32 a2);
extern void game_reset(void);
extern s32  search_load_tpage_buf_lock(s32 a0, s32 a1);
extern void class_clear_n(s32 a0);
extern void Vblnk_Timer_Init(void);
extern s32  Vblnk_Timer(void);
extern void scroll_poly_disp(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4);
extern void Fade_Out_Init(void);
extern s32  bri_fade_out(void);
extern void fade_timer_init(void);
extern s32  Fade_In_Main(void);
extern void taiiku_seat_init(void);
extern void taiiku_race_vram_init(void);
extern void taiiku_ptr4_init(void);
extern void taiiku_sprite_init3(void);

extern s32 pad_trig;
extern u8  now_sub_sub_step;
extern u8  step_mode;
extern s32 tpage_buf[];
extern s32 taiiku_com_tpg[];
extern s32 taiiku_spoon_sprite_tpg[];
extern s32 taiiku_spoon_scene_tpg[];

void taiiku_load3_main(void)
{
    s32 i;
    s32 j;
    s32 v;

    switch ((u32)now_sub_sub_step) {
    case 0:
        if ((get_CD_State2() & 1) == 0) {
            return;
        }
        for (i = 0; i < 12; i++) {
            j = search_load_tpage_buf_lock(taiiku_spoon_sprite_tpg[i], 1);
            v = i + 0x80000100;
            tpage_buf[j] = v;
            taiiku_spoon_sprite_tpg[i] = v;
        }
        for (i = 5; i != 9; i++) {
            j = search_load_tpage_buf_lock(taiiku_spoon_scene_tpg[i], 1);
            v = i + 0x80000200;
            tpage_buf[j] = v;
            taiiku_spoon_scene_tpg[i] = v;
        }
        sub_sub_step_up();
        return;

    case 1:
        device_cmd_send(513);
        fade_timer_init();
        sub_sub_step_up();
        return;

    case 2:
        if (Fade_In_Main() != 0) {
            sub_sub_step_up();
        }
        if ((pad_trig & 0xF0) != 0) {
            Fade_Out_Init();
            goto_sub_sub_step(4);
        }
        scroll_poly_disp(0, 15, 0, 240, 640);
        return;

    case 3:
        scroll_poly_disp(0, 15, 0, 240, 640);
        switch ((u32)step_mode) {
        case 0:
            set_XAsector(0x1F000330, 0x3F000356, 0);
            device_cmd_send(768);
            step_mode = step_mode + 1;
            break;
        case 1:
        case 3:
            if (get_CD_State() == 4) {
                step_mode = step_mode + 1;
            }
            break;
        case 2:
            if (get_CD_State() == 2) {
                set_XAsector(0x1F000357, 0x40000360, 0);
                device_cmd_send(768);
                step_mode = step_mode + 1;
            }
            break;
        case 4:
            if (get_CD_State() == 2) {
                Vblnk_Timer_Init();
                step_mode = step_mode + 1;
            }
            break;
        case 5:
            if ((u32)Vblnk_Timer() >= 181U) {
                step_mode = 0;
            }
            break;
        }
        if ((pad_trig & 0xF0) == 0) {
            return;
        }
        Fade_Out_Init();
        device_cmd_send(116);
        sub_sub_step_up();
        return;

    case 4:
        scroll_poly_disp(0, 15, 0, 240, 640);
        if (get_CD_State() == 2) {
            cd_read_start2(135, 0x8018F000, 0xB996);
            sub_sub_step_up();
        }
        return;

    case 5:
        scroll_poly_disp(0, 15, 0, 240, 640);
        if (bri_fade_out() == 0) {
            return;
        }
        if ((get_CD_State2() & 1) == 0) {
            return;
        }
        taiiku_ptr4_init();
        for (i = 0; i < 6; i++) {
            search_load_tpage_buf_lock(taiiku_com_tpg[i], 1);
        }
        sub_sub_step_up();
        return;

    case 6:
        class_clear_n(96);
        taiiku_race_vram_init();
        tpage_buf[21] = -1;
        taiiku_seat_init();
        taiiku_sprite_init3();
        device_cmd_send(1285);
        fade_timer_init();
        sub_step_up();
        return;

    default:
        game_reset();
        return;
    }
}
