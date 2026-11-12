/* @SymbolStart: 0x8013CBC0 */
/* @SymbolSize: 900 */
/* @CompileEnv: IDO52_AS1_XBB_SKIP_CAND=17 IDO52_UGEN_TEMP_REG=14:t2,15:t7,17:t9,18:t3,19:t4,22:t0,25:t7,26:t1,27:t2,29:t4,30:t5,31:t6,32:t7,33:t8,36:t1 IDO52_AS1_TIE_TAKE=12,29,240,329,387 IDO52_AS1_TIE_REJECT=113 IDO52_FORCE_WEB_COLOR=0:a1 */
/* @CompileFlags: -Wc,-pic1 */
/* @RodataBase: 0x80148E18 */
/* 47/223 resolved; the file also needs `@RodataBase: 0x80148E18` (its */
#include "types.h"
#include "tokimeki/class_w.h"

extern void game_reset();

extern s16 taiiku_tama_sy;
extern s16 taiiku_tama_sx;
extern s16 cw97_y;
extern s16 cw97_x;
extern s32 taiiku_tama_spd;
extern s32 taiiku_tama_gfrm;
extern volatile s32 taiiku_tama_gstep;
extern s32 taiiku_tama_fall;
extern u8  cw97_disp;
extern s32 taiiku_tama_pstep;
extern s32 taiiku_tama_xf;
extern s32 taiiku_tama_vel;
extern s32 taiiku_course_no;
extern s32 taiiku_tama_girl;
extern CLASS_W class_w[];

void taiiku_cw97_recover_step(void)
{
    s32 sel = taiiku_tama_gstep;
    s32 y;
    s32 x;
    s32 mode;
    s32 lim;
    s32 cur;

    switch (sel) {
    case 0:
        y = (taiiku_tama_sy >> 3) + 16;
        if (cw97_y < y) {
            x = taiiku_tama_sx >> 3;
            if (cw97_x < x + 80 && x - 80 < cw97_x) {
                cw97_y = y;
            }
        }
        if (class_w[101].x.half[1] < -448) {
            if (taiiku_tama_spd == 0) {
                sel++;
                taiiku_tama_gfrm = 0;
                taiiku_tama_gstep = sel;
            }
        }
        break;
    case 1:
        sel++;
        taiiku_tama_gstep = sel;
        break;
    case 2:
        sel++;
        taiiku_tama_gstep = sel;
        break;
    case 3:
        if (taiiku_tama_fall == 1) {
            cw97_x = 144;
        } else {
            cw97_disp |= 1;
            cw97_x = -144;
        }
        cw97_y = 32;
        if (0) {
        }
        sel++;
        taiiku_tama_gstep = sel;
        break;
    case 4:
        mode = taiiku_tama_fall;
        if (mode == 1) {
            if (taiiku_tama_pstep >= 4) {
                cw97_y = taiiku_tama_sy >> 3;
                cw97_x = (taiiku_tama_sx >> 3) + 96;
                if (taiiku_tama_xf <= 0) {
                    taiiku_tama_gfrm = 0;
                    taiiku_tama_vel = 0;
                    sel++;
                }
            }
        }
        taiiku_tama_gstep = sel;
        if (mode == 2) {
            taiiku_tama_gstep = sel;
            if (taiiku_tama_pstep >= 4) {
                cw97_y = taiiku_tama_sy >> 3;
                cw97_x = (taiiku_tama_sx >> 3) - 96;
                if (0) {
                }
                taiiku_tama_gstep = sel;
                if (taiiku_tama_xf >= 0) {
                    taiiku_tama_gfrm = 0;
                    cw97_disp = 164;
                    taiiku_tama_vel = 0;
                    cw97_x = -80;
                    if (0) {
                    }
                    sel++;
                    taiiku_tama_gstep = sel;
                }
            }
        }
        break;
    case 5:
        sel++;
        if (cw97_y < 48) {
            cw97_y += 2;
        } else {
            cw97_y = 48;
            if (taiiku_tama_fall == 2) {
                cw97_disp |= 1;
            }
            taiiku_tama_gfrm = 0;
            taiiku_tama_gstep = sel;
        }
        break;
    case 6:
        if (taiiku_course_no % 3 == 0 || taiiku_tama_girl == 2) {
            lim = -32;
        } else {
            lim = 0;
        }
        mode = taiiku_tama_fall;
        if (mode == 1) {
            cur = cw97_x;
            sel++;
            if (lim < cur) {
                cw97_x = cur - 2;
            } else {
                cw97_x = lim;
                if (0) {
                }
                taiiku_tama_gstep = sel;
            }
        } else {
            cur = cw97_x;
            sel++;
            if (cur < lim) {
                cw97_x = cur + 2;
            } else {
                cw97_x = lim;
                if (0) {
                }
                taiiku_tama_gstep = sel;
            }
        }
        break;
    case 7:
        break;
    default:
        game_reset();
        break;
    }
}
