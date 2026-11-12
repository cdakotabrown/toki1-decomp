/* @SymbolSize: 1256 */
/* @RodataBase: 0x80148D20 */
/* @CompileFlags: -cckr */
#include "types.h"
#include "tokimeki/taiiku_kari_run.h"
#include "tokimeki/class_w.h"

typedef struct {
    s32 w[11];
} Blk44;

typedef struct {
    u8 pad00[38];
    s16 f26;
    u8 pad28[28];
} Actor;

typedef struct {
    s16 v;
    s16 pad2;
    s16 w;
    u8 pad6[50];
} GirlRec;

typedef struct {
    u8 pad0[446];
    GirlRec rec[20];
} SaveData;

extern Blk44 taiiku_card_girl_tbl;
extern Actor class_w[];
extern SaveData screen_w;

extern u8 now_sub_sub_step;
extern s32 taiiku_konami_mode;
extern u8 taiiku_kari_girl;
extern u8 taiiku_kari_rank[];
extern s16 taiiku_kari_card;
extern TAIIKU_KARI_RUN taiiku_kari_run[];

void taiiku_xa_cell_set(s32 a0, u8 a1);
void taiiku_rank_frame_step();
u32 Vblnk_Timer();

void taiiku_kari_result_main()
{
    s32 h;
    s32 i;
    Blk44 buf;

    buf = taiiku_card_girl_tbl;
    scroll_poly_disp(0, 15, 0, 112, 640);
    scroll_poly_disp(0, 15, 112, 128, 128);

    switch (now_sub_sub_step) {
    case 0:
        device_cmd_send(1547);
        fade_timer_init();
        sub_sub_step_up();
        break;
    case 1:
        if (Fade_In_Main() != 0) {
            if (taiiku_kari_rank[0] == 1 || taiiku_kari_rank[1] == 1) {
                device_port_init(1, 517);
            } else {
                device_port_init(1, 518);
            }
            taiiku_kari_run[0].frm = 0;
            taiiku_kari_run[0].cnt0c = 0;
            sub_sub_step_up();
        }
        break;
    case 2:
        taiiku_kari_run[0].cnt0c++;
        taiiku_rank_frame_step();
        for (i = 0; i < 4; i++) {
            class_w[96 + i].f26--;
        }
        for (i = 0; i < 4; i++) {
            class_w[107 + i].f26--;
        }
        if (class_w[107].f26 < -159 || class_w[108].f26 < -159 ||
            class_w[109].f26 < -159 || class_w[110].f26 < -159) {
            if (taiiku_konami_mode == 1) {
                taiiku_xa_start();
            } else {
                for (i = 0; i < 4; i++) {
                    if (taiiku_kari_rank[i] == 1) {
                        taiiku_xa_cell_set(buf.w[taiiku_kari_card], i);
                    }
                }
            }
            Vblnk_Timer_Init();
            sub_sub_step_up();
        }
        break;
    case 3:
        taiiku_rank_frame_step();
        if (taiiku_konami_mode == 1) {
            if (((CLASS_W *)class_w)[112].y.half[1] < -88) {
                ((CLASS_W *)class_w)[112].y.half[1] += 4;
            } else if (((CLASS_W *)class_w)[112].y.half[1] == -88) {
                device_cmd_send(1293);
                ((CLASS_W *)class_w)[112].y.half[1] = -84;
            }
        }
        if (Vblnk_Timer() >= 121) {
            sub_sub_step_up();
        }
        break;
    case 4:
        if (get_CD_State() == 2) {
            Vblnk_Timer_Init();
            sub_sub_step_up();
        }
        break;
    case 5:
        if (Vblnk_Timer() >= 121) {
            Fade_Out_Init();
            sub_sub_step_up();
        }
        break;
    case 6:
        if (bri_fade_out() != 0) {
            if (taiiku_kari_girl < 11) {
                screen_w.rec[taiiku_kari_girl].w += 2;
            } else {
                screen_w.rec[buf.w[taiiku_kari_card]].w++;
            }
            if (taiiku_kari_rank[0] == 1) {
                for (i = 0; i < 13; i++) {
                    screen_w.rec[i].v += 6;
                }
            } else if (taiiku_kari_rank[1] == 1) {
                for (i = 0; i < 13; i++) {
                    screen_w.rec[i].v += 3;
                }
            }
            sub_step_up();
        }
        break;
    default:
        game_reset();
        break;
    }
}
