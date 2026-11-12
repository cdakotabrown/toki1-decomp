/* @SymbolSize: 1036 */
/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=2:v1 */
#include "types.h"

typedef struct { s16 h[10]; } Blk20h;
typedef struct { s32 w[8]; } Blk32;
typedef struct { s32 w[11]; } Blk44;

extern Blk20h taiiku_setup0e_tbl0;
extern Blk20h taiiku_setup0e_tbl1;
extern Blk32 taiiku_setup0e_tbl2;
extern Blk44 taiiku_setup0e_tbl3;

extern u8 now_sub_sub_step;
extern u8 taiiku_ninin_girl1;
extern s32 screen_w[];
extern s16 taiiku_ninin_pfrm_tbl[];
s16 taiiku_ninin_pfrm_tbl1[10] = {0};
extern s32 taiiku_ninin_pl_tpg[];
extern s32 taiiku_ninin_sprite_tpg[];
extern s32 taiiku_ninin_rank_tpg[];
extern s32 taiiku_ninin_buf_2;

void sub_step_up();
void sub_sub_step_up();
void palette_upload_enqueue(s32 a0, u8 a1, s32 a2, s32 a3, s32 a4);
void device_cmd_send();
void device_load_start();
void cd_read_start2();
void game_reset();
void search_load_tpage_buf_lock();
void class_clear_all();
void set_flag_b3_high_bit();
void date_x_taku_set();
void Fade_Out_Init();
void taiiku_actor_init2();
void taiiku_race_init3();
void taiiku_module_init4();
void taiiku_buf_init();
void taiiku_tbl_init();

void taiiku_setup0e()
{
    u8 sel;
    s32 i;
    Blk20h b164;
    Blk20h b144;
    Blk32 b112;
    Blk44 b68;

    sel = now_sub_sub_step;
    b164 = taiiku_setup0e_tbl0;
    b144 = taiiku_setup0e_tbl1;
    b112 = taiiku_setup0e_tbl2;
    b68 = taiiku_setup0e_tbl3;

    switch (sel) {
    case 0:
        if (taiiku_ninin_girl1 == 1) {
            device_cmd_send(191);
        }
        device_load_start(0, 0xBFA6, 0xBF87, 0xC21D, 0xC1FD, 0xD6C5);
        date_x_taku_set(1, 1);
        for (i = 0; i < 11; i++) {
            screen_w[1167 + i] = 8;
        }
        for (i = 0; i < 3; i++) {
            screen_w[1199 + i] = 8;
        }
        for (i = 0; i < 8; i++) {
            taiiku_ninin_sprite_tpg[i] = b112.w[i];
        }
        for (i = 0; i < 11; i++) {
            taiiku_ninin_rank_tpg[i] = b68.w[i];
        }
        for (i = 0; i < 10; i++) {
            taiiku_ninin_pfrm_tbl[i] = b164.h[i];
        }
        for (i = 0; i < 10; i++) {
            taiiku_ninin_pfrm_tbl1[i] = b144.h[i];
        }
        class_clear_all(0);
        set_flag_b3_high_bit(0);
        sub_sub_step_up();
        break;
    case 1:
        if (device_load_poll() == 1) {
            cd_read_start2(30, 0x80180000, 0xBAD7);
            sub_sub_step_up();
        }
        break;
    case 2:
        if (get_CD_State2() & 1) {
            cd_read_start2(79, 0x8018F000, 0xB868);
            Fade_Out_Init();
            sub_sub_step_up();
        }
        break;
    case 3:
        if (bri_fade_out() != 0) {
            if (get_CD_State2() & 1) {
                taiiku_tbl_init();
                taiiku_buf_init();
                for (i = 0; i < 3; i++) {
                    search_load_tpage_buf_lock(taiiku_ninin_pl_tpg[i], 1);
                }
                for (i = 0; i < 16; i++) {
                    palette_upload_enqueue(taiiku_ninin_buf_2, i + 16, 1, 1, i);
                }
                taiiku_race_init3();
                taiiku_module_init4();
                taiiku_actor_init2();
                cd_read_start2(161, 0x8018F000, 0xB7C7);
                sub_step_up();
            }
        }
        break;
    default:
        game_reset();
        break;
    }
}
