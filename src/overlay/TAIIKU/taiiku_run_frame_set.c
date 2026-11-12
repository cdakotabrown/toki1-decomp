/* @SymbolStart: 0x80136CB0 */
/* @SymbolSize: 496 */
/* @CompileFlags: -Wo,-loopunroll,0 -Wc,-pic0 */
#include "types.h"
#include "tokimeki/class_w.h"

typedef struct { u16 h[11]; } Rec22;
typedef struct { s16 f; u8 pad[66]; } Rec68;

extern Rec22 taiiku_run_frame_set_tbl;
extern s32 taiiku_konami_mode;
extern u8  taiiku_kari_girl;
extern u8  taiiku_kari_run_0_seat;
extern u8  taiiku_kari_run_0_frm;
extern u8  taiiku_kari_run_0_col;
extern s32 taiiku_kari_vmax;
extern u16 taiiku_kari_run_0_cnt0c;
extern u16 taiiku_kari_run_0_cnt0e;
extern s32 taiiku_kari_run_0_accel;
extern s32 taiiku_kari_run_0_speed;
extern s32 taiiku_kari_scrv;
extern s16 taiiku_kari_pfrm_tbl[];
extern s16 cal_cw_frame;
extern CLASS_W class_w[];

void taiiku_run_frame_set(void) {
    Rec22 buf;
    u32 v;
    u32 lim;
    s32 w;
    u8 c;

    buf = taiiku_run_frame_set_tbl;

    if (taiiku_konami_mode == 1) {
        v = buf.h[taiiku_kari_girl];
    } else {
        v = buf.h[((Rec68 *)&class_w[46].anim)[taiiku_kari_run_0_seat].f >> 3];
    }

    if (v < taiiku_kari_run_0_cnt0e) {
        if (taiiku_kari_run_0_speed != 0) {
            if (taiiku_kari_run_0_col >= 5U) {
                taiiku_kari_run_0_col = 0;
            } else {
                taiiku_kari_run_0_col = taiiku_kari_run_0_col + 1;
            }
            taiiku_kari_run_0_cnt0e = 0;
        }
    }

    lim = taiiku_kari_run_0_cnt0c;
    w = taiiku_kari_run_0_speed;
    if (lim >= 6U && taiiku_kari_run_0_accel > 0) {
        goto setblk;
    }
    if (lim >= 8U && (taiiku_kari_vmax >> 1) < w) {
        goto setblk;
    }
    if (lim < 11U || w == 0) {
        goto tail;
    }
setblk:
    if (taiiku_kari_run_0_frm >= 5U) {
        taiiku_kari_run_0_frm = 0;
    } else {
        taiiku_kari_run_0_frm++;
    }
    taiiku_kari_run_0_cnt0c = 0;
tail:
    cal_cw_frame = taiiku_kari_pfrm_tbl[taiiku_kari_run_0_frm];
    if (taiiku_kari_scrv != 0) {
        return;
    }
    if (taiiku_kari_run_0_accel > 0) {
        return;
    }
    taiiku_kari_run_0_frm = 5;
    cal_cw_frame = 144;
}
