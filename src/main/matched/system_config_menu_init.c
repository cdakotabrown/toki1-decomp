/* Address: 0x8005DE8C */
/* Size: 360 bytes (90 instructions) */
/* Region: Game (0x80041000-0x80085FFF), IDO 5.2 */
/* @CompileEnv: IDO52_AS1_TIE_SRC_ORDER=bb:3 IDO52_FORCE_WEB_COLOR=0:a1,3:a2,9:a3,12:t0,21:t1,74:v0,75:v1,76:a0 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern s8 now_icon;
extern u8 system_config_menu_init_str0[];
extern u8 system_config_menu_init_str1[];
extern void text_sys_init(void);
extern void basyo_init(u8 *);
extern void icon_disp_switch(s32);
extern void data_save_load_class_init(void);
extern void system_config_disp(s32);
extern s32  get_mascot_count(void);
extern void k_sub_reset_point_set(void);
extern void set_kanji_string(s32, s32, s32, u8 *, s32);
extern void menu_set(s32, s32, s16 *, s16 *, s16 *, s16 *);
extern void k_sub_disp_start(s32);
extern void sub_sub_step_up(void);

void system_config_menu_init(void) {
    s32 unused0;
    s32 unused1;
    s16 a[2][4];
    s16 b[2][4];
    s16 c[2][4];
    s16 d[2][4];
    s32 n0;
    s32 n1;
    s32 n2;
    s32 n3;
    s16 *pa;
    s16 *pb;
    s16 *pc;
    s16 *pd;

    text_sys_init();
    basyo_init(system_config_menu_init_str0);
    icon_disp_switch(0);
    data_save_load_class_init();
    pa = a[0];
    pb = b[0];
    n0 = -64;
    pc = c[0];
    pd = d[0];
    n1 = -52;
    n2 = -40;
    n3 = -28;
    do {
        pb[3] = n3;
        n3 += 48;
        pb[1] = n1;
        pb[2] = n2;
        pb[0] = n0;
        n0 += 48;
        n2 += 48;
        n1 += 48;
        pa[1] = -124;
        pc[1] = 96;
        pd[1] = 12;
        pa[2] = -124;
        pc[2] = 96;
        pd[2] = 12;
        pa[3] = -124;
        pc[3] = 96;
        pd[3] = 12;
        pa += 4;
        pb += 4;
        pc += 4;
        pd += 4;
        pa[-4] = -124;
        pc[-4] = 96;
        pd[-4] = 12;
    } while (n3 != 68);
    system_config_disp(-1);
    if (get_mascot_count() == 0) {
        c[1][2] = 0;
    }
    k_sub_reset_point_set();
    set_kanji_string(-128, 50, 0, system_config_menu_init_str1, 0);
    menu_set(0, 8, a[0], b[0], c[0], d[0]);
    now_icon = now_icon % 8;
    k_sub_disp_start(0);
    sub_sub_step_up();
}
