/* @SymbolStart: 0x80139B9C */
/* @SymbolSize: 1004 */
/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_FORCE_WEB_ADJSAVE=22:0.4 */
#include "types.h"

extern s32 pad_data;
extern s32 pad_trig;
extern u8 taco_name_end_str[];
extern u8 taco_name_fin_str[];
extern u8 taco_name_entry_str0[];
extern u8 taco_rank_name[];
extern u8 taco_wheel_chr_tbl[];
extern s32 taco_name_row;
extern s32 taco_name_col;
extern s32 taco_wheel_no;
extern s32 taco_wheel_ofs;
extern s32 taco_wheel_spd;

extern void sub_sub_step_up();
extern void strncpy();
extern void taco_glyph_disp();
extern void taco_wheel_disp();
extern void taco_name_disp();
extern void taco_drop();

void taco_name_entry()
{
    s32 pad;
    s32 span;

    if (taco_wheel_ofs < 0) {
        span = taco_wheel_ofs + 80;
    } else {
        span = 80 - taco_wheel_ofs;
    }
    taco_wheel_disp(taco_wheel_no, taco_wheel_ofs, 160, 0);
    taco_glyph_disp(taco_wheel_chr_tbl[taco_wheel_no % 40], 6, 4, 128, 0, 60, span * 7 * 4096 / 80, span * 7 * 4096 / 80, 1);
    taco_name_disp(480);
    taco_drop(0, 0);

    pad = pad_data;
    if (pad & 0x2000) {
        if (taco_wheel_spd < 0) {
            taco_wheel_spd = 1;
        }
    }
    if (pad & 0x8000) {
        if (taco_wheel_spd > 0) {
            taco_wheel_spd = -1;
        }
    }
    switch (taco_wheel_spd) {
    default:
        break;
    case -2:
        taco_wheel_ofs += 10;
        break;
    case -1:
        taco_wheel_ofs += 5;
        break;
    case 1:
        taco_wheel_ofs -= 5;
        break;
    case 2:
        taco_wheel_ofs -= 10;
        break;
    }
    if (taco_wheel_ofs == 0) {
        if (!(pad & 0xA000)) {
            taco_wheel_spd = 0;
        }
        if (pad & 0x2000) {
            taco_wheel_spd++;
        }
        if (pad & 0x8000) {
            taco_wheel_spd--;
        }
        if (taco_wheel_spd >= 3) {
            taco_wheel_spd = 2;
        }
        if (taco_wheel_spd < -2) {
            taco_wheel_spd = -2;
        }
    }
    if (taco_wheel_ofs > 50) {
        taco_wheel_no++;
        taco_wheel_ofs = -50;
    }
    if (taco_wheel_ofs < -50) {
        taco_wheel_ofs = 50;
        taco_wheel_no--;
    }
    if (taco_wheel_no >= 40) {
        taco_wheel_no = 0;
    }
    if (taco_wheel_no < 0) {
        taco_wheel_no = 39;
    }

    if ((pad_trig & 0x20) && !(pad_trig & 0x40)) {
        if (taco_wheel_no == 39) {
            strncpy(taco_rank_name[taco_name_row * 4 + taco_name_col], taco_name_end_str, 1);
            sub_sub_step_up();
        } else if (taco_name_col != 3) {
            strncpy(&taco_rank_name[taco_name_row * 4 + taco_name_col], &taco_wheel_chr_tbl[taco_wheel_no], 1);
            taco_name_col++;
        }
    }
    if ((pad_trig & 0x800) && !(pad_trig & 0x40)) {
        strncpy(taco_rank_name[taco_name_row * 4 + taco_name_col], taco_name_fin_str, 1);
        sub_sub_step_up();
    }
    if (pad_trig & 0x40) {
        if (taco_name_col != 0) {
            strncpy(&taco_rank_name[taco_name_row * 4 + taco_name_col - 1], taco_name_entry_str0, 1);
            taco_name_col--;
        }
    }
}
