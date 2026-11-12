/* @CompileEnv: IDO52_AS1_NO_LA_PULL=3 IDO52_AS1_NO_FALLTHRU_FILL=occ:1 IDO52_UGEN_TEMP_REG=10:t9,7:t3,8:t4,9:t5,11:t8,6:t2,12:t0,13:t7,14:t6 IDO52_FORCE_WEB_COLOR=74:a3 IDO52_PRECOLOR_WEB=v0:6 */
/* @CompileFlags: -cckr */
/* Address: 0x8013EF2C, Size: 484 bytes (121 instructions) */
#include "types.h"

struct Names {
    char n[22][33];
};

extern struct Names date_machiawase_name_set_tbl;
extern unsigned char now_girl[];
extern int event_girl_no;
extern unsigned char now_sub_sub_step;
extern short date_now_serifu0;
extern unsigned char machiawase_mode;
extern unsigned char machiawase_basyo;
extern unsigned char machiawase_basyo2;
extern unsigned char machiawase_basyo3;
extern unsigned char date_machi_mode;
extern char aname[];
extern char mname[];

void date_machiawase_name_set()
{
    int ff;
    struct Names buf;

    buf = date_machiawase_name_set_tbl;
    mes_window_switch(1);
    ff = 255;
    event_girl_no = now_girl[0];
    if ((get_g_zyotai_s(now_girl[0]) & 0x7F) == 4) {
        date_now_serifu0 = 8;
        now_sub_sub_step += 36;
        return;
    }
    if ((machiawase_mode == machiawase_basyo && date_machi_mode == 1) || ff == machiawase_basyo2 ||
        (machiawase_basyo == machiawase_basyo2 && ff == machiawase_basyo3) ||
        (machiawase_basyo == machiawase_basyo2 && machiawase_basyo == machiawase_basyo3)) {
        now_sub_sub_step += 17;
        return;
    }
    if (machiawase_mode == machiawase_basyo2) {
        strcpy(aname, buf.n[machiawase_basyo]);
    } else {
        strcpy(aname, buf.n[machiawase_basyo2]);
    }
    if (machiawase_basyo3 != 255) {
        date_now_serifu0 = 3;
        if (machiawase_mode == machiawase_basyo3) {
            strcpy(mname, buf.n[machiawase_basyo]);
        } else {
            strcpy(mname, buf.n[machiawase_basyo3]);
        }
    }
    sub_sub_step_up();
}
