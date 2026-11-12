/* @CompileFlags: -Wc,-pic0 */
/* @PostProc: patch_insn_reg_field 0x1c0:rt:s0:a1 0x1c4:rs:s0:a1 */
/* @PostProc: remap_stack_slots --map '48:60,52:64' */
/* @RodataBase: 0x80148D40 */
/* Address: 0x80138ED4 */
/* Size: 1700 bytes (425 insns) */
#include "types.h"
#include "tokimeki/class_w.h"

typedef struct {
    s32 w[11];
} Blk44;

typedef struct {
    s16 x;
    s16 y;
    s16 w;
    s16 h;
    s16 rest[6];
} Rect20;

typedef struct {
    u8  pad00[3];
    u8  f3;
    u8  pad04[34];
    s16 f38;
    s16 pad40;
    s16 f42;
    u8  pad44[24];
} Actor;

extern void sub_step_up(void);
extern void sub_sub_step_up(void);
extern void draw2d3d(s32 a0, s32 a1);
extern void device_cmd_send(s32 a0);
extern void device_load_start(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
extern s32  get_CD_State2(void);
extern s32  get_CD_State(void);
extern void set_XAsector(s32 a0, s32 a1, s32 a2);
extern void cd_read_start2(s32 a0, s32 a1, s32 a2);
extern void game_reset(void);
extern s32  search_load_tpage_buf_lock(s32 a0, s32 a1);
extern void srn_vram_set(s32 a0, s32 a1, s32 a2, s32 a3);
extern void srn_init(s32 a0, s32 a1, s32 a2);
extern void mod_init(s32 a0, s32 a1, s32 a2);
extern void Vblnk_Timer_Init(void);
extern s32  Vblnk_Timer(void);
extern void scroll_poly_disp(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4);
extern void Fade_Out_Init(void);
extern s32  bri_fade_out(void);
extern void fade_timer_init(void);
extern s32  Fade_In_Main(void);
extern void Yubi(s32 a0);
extern void load_done_flag_set(void);
extern void LoadImage(void *a0, s32 a1);
extern void taiiku_img_load(void);
extern void taiiku_ptr4_init(void);
extern void taiiku_plate_scroll(void);
extern void taiiku_field_init(void);
extern void taiiku_camera_mtx_set(void);
extern void taiiku_run_speed(void);
extern void taiiku_course_disp(void);
extern void taiiku_view_init(void);
extern void taiiku_light_init(void);
extern void taiiku_coord_init(void);
extern void taiiku_model_init(s32 a0);
extern void taiiku_model_slot_set(s32 a0, s32 a1);

extern Actor class_w[];
extern Blk44 taiiku_girl_select2_tbl0;
extern Blk44 taiiku_girl_select2_tbl1;

extern s32 pad_trig;
extern u8  now_sub_sub_step;
extern u8  step_mode;
extern s32 tpage_buf[];
extern s32 load_done_flag;
extern s32 taiiku_com_buf;
extern s32 taiiku_com_buf_1;
extern s32 taiiku_com_buf_2;
extern s32 taiiku_tama_girl;
extern s32 taiiku_tama_field_tpg[];
extern s32 taiiku_tama_plate_tpg[];
extern s32 taiiku_com_tpg[];

void taiiku_girl_select2(void)
{
    Rect20 r;
    Blk44 b;
    Blk44 a;
    s32 i;
    s32 j;
    s32 v;

    b = taiiku_girl_select2_tbl0;
    a = taiiku_girl_select2_tbl1;

    switch ((u32)now_sub_sub_step) {
    case 0:
        if ((get_CD_State2() & 1) == 0) {
            return;
        }
        for (i = 0; i < 18; i++) {
            j = search_load_tpage_buf_lock(taiiku_tama_field_tpg[i], 1);
            v = i + 0x80000100;
            tpage_buf[j] = v;
            taiiku_tama_field_tpg[i] = v;
        }
        r.x = 768;
        r.y = 128;
        r.w = 64;
        r.h = 128;
        LoadImage(&r, 0x801BB000);
        tpage_buf[44] = 8;
        for (i = 0; i != 5; i++) {
            j = search_load_tpage_buf_lock(taiiku_tama_plate_tpg[i], 1);
            v = i + 0x80000200;
            tpage_buf[j] = v;
            taiiku_tama_plate_tpg[i] = v;
        }
        cd_read_start2(3, 0x801F8000, 0xBE38);
        fade_timer_init();
        device_cmd_send(512);
        sub_sub_step_up();
        return;

    case 1:
        if (Fade_In_Main() != 0) {
            if ((get_CD_State2() & 1) != 0) {
                draw2d3d(1, 1);
                taiiku_view_init();
                taiiku_light_init();
                taiiku_coord_init();
                taiiku_model_init(0x801F8000);
                taiiku_model_slot_set(0x801F8000, 0);
                sub_sub_step_up();
            }
        }
        scroll_poly_disp(0, 15, 0, 240, 640);
        return;

    case 2:
        switch ((u32)step_mode) {
        case 0:
            set_XAsector(0x180003C4, 0x420003D0, 0);
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
                set_XAsector(0x0B0003B8, 0x390003D0, 0);
                device_cmd_send(768);
                step_mode = step_mode + 1;
            }
            break;
        case 4:
            if (get_CD_State() == 2) {
                set_XAsector(0x010003C8, 0x420003D0, 0);
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
            if ((u32)Vblnk_Timer() >= 181U) {
                step_mode = 0;
            }
            break;
        }
        scroll_poly_disp(0, 15, 0, 240, 640);
        taiiku_plate_scroll();
        Yubi(107);
        if ((pad_trig & 0xF0) == 0) {
            return;
        }
        for (i = 0; i != 11; i++) {
            if (((CLASS_W *)class_w)[107].x.half[1] + 8 >= class_w[96 + i].f38 &&
                class_w[96 + i].f38 + 32 >= ((CLASS_W *)class_w)[107].x.half[1] + 8 &&
                ((CLASS_W *)class_w)[107].y.half[1] + 16 >= class_w[96 + i].f42 &&
                class_w[96 + i].f42 + 64 >= ((CLASS_W *)class_w)[107].y.half[1] + 16 &&
                class_w[96 + i].f3 != 0) {
                taiiku_tama_girl = i;
                Fade_Out_Init();
                device_cmd_send(116);
                sub_sub_step_up();
            }
        }
        return;

    case 3:
        scroll_poly_disp(0, 15, 0, 240, 640);
        if (get_CD_State() == 2) {
            cd_read_start2(135, 0x8018F000, 0xB996);
            sub_sub_step_up();
        }
        return;

    case 4:
        scroll_poly_disp(0, 15, 0, 240, 640);
        if (bri_fade_out() != 0) {
            sub_sub_step_up();
        }
        return;

    case 5:
        if ((get_CD_State2() & 1) == 0) {
            return;
        }
        taiiku_ptr4_init();
        mod_init(0, taiiku_com_buf_1, taiiku_com_buf);
        srn_init(0, taiiku_com_buf_2, 0);
        srn_vram_set(0, 5, 320, 240);
        taiiku_img_load();
        for (i = 0; i < 6; i++) {
            search_load_tpage_buf_lock(taiiku_com_tpg[i], 1);
        }
        taiiku_field_init();
        j = taiiku_tama_girl;
        load_done_flag = 0;
        device_load_start(0, 0xBFA6, 0xBF87, b.w[j], a.w[j], 0xD6C5);
        fade_timer_init();
        sub_sub_step_up();
        return;

    case 6:
        load_done_flag_set();
        if (Fade_In_Main() != 0) {
            Vblnk_Timer_Init();
            sub_step_up();
        }
        taiiku_course_disp();
        taiiku_run_speed();
        taiiku_camera_mtx_set();
        return;

    default:
        game_reset();
        return;
    }
}
