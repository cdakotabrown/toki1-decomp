/* Address: 0x8013A94C (real entry 0x8013A944: 2-insn cross-prologue head */
/* Size: 1464 bytes (366 insns) + 8-byte head = 1472 */
/* @SymbolStart: 0x8013A944 */
/* @RodataBase: 0x80148DAC */
/* @SymbolSize: 1472 */
#include "types.h"
#include "tokimeki/class_w.h"

extern s32 taiiku_tama_zone;
extern s32 taiiku_tama_btn;
extern s32 taiiku_tama_pstep;
extern s32 taiiku_tama_gstep;
extern s32 taiiku_tama_spd;
extern s32 taiiku_tama_vel;
extern s32 taiiku_tama_xf;
extern s16 taiiku_tama_sx;
extern s16 taiiku_tama_sy;
extern s32 taiiku_tama_pfrm;
extern s32 taiiku_tama_fall;
extern u8 cal_cw_disp;
extern s16 cal_cw_x;
extern s16 cal_cw_y;
extern s32 pad_data;
extern s32 pad_trig;
extern CLASS_W class_w[];

extern void game_reset();
extern void Yubi();
extern void taiiku_pad_move();
extern void taiiku_state_reset();
extern void taiiku_cal_cw_frame_set();
extern void taiiku_pos_clamp();
extern s32 get_taiiku_zone();

void taiiku_zone_main(void)
{
    switch ((u32)taiiku_tama_zone) {
    case 0:
        Yubi(96);
        break;
    case 1: {
        s32 v1a;
        taiiku_pad_move(96);
        taiiku_pos_clamp();
        v1a = (taiiku_tama_sy >> 3) + 16;
        if (cal_cw_y < v1a) {
            cal_cw_y = v1a;
        }
        break;
    }
    case 2: {
        s32 x2;
        taiiku_pad_move(96);
        taiiku_pos_clamp();
        x2 = taiiku_tama_sx >> 3;
        if (x2 + 80 >= cal_cw_x) {
            cal_cw_x = x2 + 81;
        }
        break;
    }
    case 3: {
        s32 x3;
        taiiku_pad_move(96);
        taiiku_pos_clamp();
        x3 = taiiku_tama_sx >> 3;
        if (cal_cw_x >= x3 - 80) {
            cal_cw_x = x3 - 81;
        }
        break;
    }
    case 5: {
        s32 x5;
        s32 v5;
        taiiku_pad_move(96);
        taiiku_pos_clamp();
        x5 = taiiku_tama_sx >> 3;
        if (cal_cw_x >= x5 - 80) {
            v5 = (taiiku_tama_sy >> 3) + 16;
            if (cal_cw_y < v5) {
                cal_cw_x = x5 - 81;
                cal_cw_y = v5;
            }
        }
        break;
    }
    case 6: {
        s32 x6;
        s32 v6;
        taiiku_pad_move(96);
        taiiku_pos_clamp();
        x6 = taiiku_tama_sx >> 3;
        if (x6 + 80 >= cal_cw_x) {
            v6 = (taiiku_tama_sy >> 3) + 16;
            if (cal_cw_y < v6) {
                cal_cw_x = x6 + 81;
                cal_cw_y = v6;
            }
        }
        break;
    }
    case 7:
        switch ((u32)taiiku_tama_pstep) {
        case 0: {
            s32 vv;
            s32 xx;
            s32 hh;
            vv = (taiiku_tama_sy >> 3) + 16;
            if (cal_cw_y < vv) {
                xx = taiiku_tama_sx >> 3;
                hh = cal_cw_x;
                if (hh < xx + 80 && xx - 80 < hh) {
                    cal_cw_y = vv;
                }
            }
            class_w[101].x.half[1] -= 8;
            if (class_w[101].x.half[1] < -448) {
                if (taiiku_tama_spd == 0) {
                    taiiku_tama_pfrm = 0;
                    cal_cw_disp = 164;
                    taiiku_tama_pstep = taiiku_tama_pstep + 1;
                }
            }
            break;
        }
        case 1:
            taiiku_tama_pstep = taiiku_tama_pstep + 1;
            break;
        case 2:
            taiiku_tama_pstep = taiiku_tama_pstep + 1;
            break;
        case 3:
            if (taiiku_tama_fall == 1) {
                cal_cw_x = 144;
            } else {
                cal_cw_x = -144;
                cal_cw_disp |= 1;
            }
            cal_cw_y = 16;
            taiiku_tama_pstep = taiiku_tama_pstep + 1;
            break;
        case 4:
            if (taiiku_tama_gstep == 4) {
                if (taiiku_tama_fall == 1) {
                    s32 xa;
                    s32 ya;
                    xa = taiiku_tama_sx >> 3;
                    ya = taiiku_tama_sy >> 3;
                    cal_cw_x = xa + 96;
                    cal_cw_y = ya - 16;
                    if (taiiku_tama_xf <= 0) {
                        taiiku_tama_pfrm = 0;
                        taiiku_tama_vel = 0;
                        taiiku_tama_pstep = taiiku_tama_pstep + 1;
                    }
                } else {
                    s32 xb;
                    s32 yb;
                    xb = taiiku_tama_sx >> 3;
                    yb = taiiku_tama_sy >> 3;
                    cal_cw_x = xb - 96;
                    cal_cw_y = yb - 16;
                    if (taiiku_tama_xf >= 0) {
                        taiiku_tama_pfrm = 0;
                        cal_cw_disp = 164;
                        taiiku_tama_vel = 0;
                        cal_cw_x = -80;
                        taiiku_tama_pstep = taiiku_tama_pstep + 1;
                    }
                }
            }
            break;
        case 5: {
            s32 n5;
            n5 = cal_cw_y;
            if (n5 < 48) {
                cal_cw_y = n5 + 2;
            } else {
                cal_cw_y = 48;
                if (taiiku_tama_fall == 2) {
                    cal_cw_disp |= 1;
                }
                taiiku_tama_pfrm = 0;
                taiiku_tama_pstep = taiiku_tama_pstep + 1;
            }
            break;
        }
        case 6:
            if (taiiku_tama_fall == 1) {
                s32 ha;
                ha = cal_cw_x;
                if (ha >= 33) {
                    cal_cw_x = ha - 2;
                } else {
                    cal_cw_x = 32;
                    taiiku_tama_pstep = taiiku_tama_pstep + 1;
                }
            } else {
                s32 hb;
                hb = cal_cw_x;
                if (hb < 32) {
                    cal_cw_x = hb + 2;
                } else {
                    cal_cw_x = 32;
                    taiiku_tama_pstep = taiiku_tama_pstep + 1;
                }
            }
            break;
        case 7:
            if (taiiku_tama_gstep >= 7) {
                taiiku_state_reset();
            }
            break;
        default:
            game_reset();
            break;
        }
        break;
    case 4:
    default:
        game_reset();
        break;
    }

    taiiku_tama_zone = get_taiiku_zone();
    if (pad_trig & 0x20) {
        taiiku_tama_btn = 1;
    }
    if (taiiku_tama_btn != 0 || (pad_data & 0xF000) || taiiku_tama_fall != 0) {
        taiiku_cal_cw_frame_set();
    }
}
