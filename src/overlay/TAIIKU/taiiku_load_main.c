/* @SymbolSize: 868 */
/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=3:v1 */
#include "types.h"

typedef struct { s32 w[1]; } Blk4;
typedef struct { s32 w[5]; } Blk20;
typedef struct { s32 w[7]; } Blk28;
typedef struct { s32 w[10]; } Blk40;

extern Blk4 taiiku_load_main_tbl0;
extern Blk20 taiiku_load_main_tbl1;
extern Blk28 taiiku_load_main_tbl2;
extern Blk40 taiiku_load_main_tbl3;

extern u8 now_sub_sub_step;
extern s32 screen_w[];
extern s32 taiiku_kari_pl_tpg;
extern s32 taiiku_kari_crowd_tpg[];
extern s32 taiiku_kari_runner_tpg[];
s32 taiiku_kari_sprite_tpg[10] = {0};
extern s32 taiiku_kari_buf_1;
extern s32 tpage_buf[];

void sub_step_up();
void sub_sub_step_up();
void palette_upload_enqueue(s32 a0, u8 a1, s32 a2, s32 a3, s32 a4);
void cd_read_start2();
void game_reset();
void search_load_tpage_buf_lock();
void class_clear_all();
void set_flag_b3_high_bit();
void date_x_taku_set();
void Fade_Out_Init();
void taiiku_race_init();
void taiiku_module_init3();
void taiiku_actor_init();
void taiiku_bufptr_init();
void taiiku_tbl_init();

void taiiku_load_main()
{
    u8 sel;
    s32 i;
    Blk4 b156;
    Blk20 b136;
    Blk28 b108;
    Blk40 b68;

    b156 = taiiku_load_main_tbl0;
    sel = now_sub_sub_step;
    b136 = taiiku_load_main_tbl1;
    b108 = taiiku_load_main_tbl2;
    b68 = taiiku_load_main_tbl3;

    switch (sel) {
    case 0:
        date_x_taku_set(1, 1);
        class_clear_all(0);
        set_flag_b3_high_bit(0);
        for (i = 0; i < 11; i++) {
            screen_w[1167 + i] = 8;
        }
        for (i = 0; i < 3; i++) {
            screen_w[1199 + i] = 8;
        }
        tpage_buf[28] = 8;
        tpage_buf[63] = 8;
        taiiku_kari_pl_tpg = b156.w[0];
        for (i = 0; i < 5; i++) {
            taiiku_kari_crowd_tpg[i] = b136.w[i];
        }
        for (i = 0; i < 7; i++) {
            taiiku_kari_runner_tpg[i] = b108.w[i];
        }
        for (i = 0; i < 10; i++) {
            taiiku_kari_sprite_tpg[i] = b68.w[i];
        }
        sub_sub_step_up();
        break;
    case 1:
        cd_read_start2(30, 0x80180000, 0xBAD7);
        Fade_Out_Init();
        sub_sub_step_up();
        break;
    case 2:
        bri_fade_out();
        if (get_CD_State2() & 1) {
            cd_read_start2(38, 0x8018F000, 0xB79F);
            sub_sub_step_up();
        }
        break;
    case 3:
        if (bri_fade_out() != 0) {
            if (get_CD_State2() & 1) {
                taiiku_tbl_init();
                taiiku_bufptr_init();
                search_load_tpage_buf_lock(taiiku_kari_pl_tpg, 1);
                for (i = 0; i < 16; i++) {
                    palette_upload_enqueue(taiiku_kari_buf_1, i + 16, 1, 1, i);
                }
                taiiku_race_init();
                taiiku_module_init3();
                taiiku_actor_init();
                cd_read_start2(193, 0x8018F000, 0xB6DE);
                sub_step_up();
            }
        }
        break;
    default:
        game_reset();
        break;
    }
}
