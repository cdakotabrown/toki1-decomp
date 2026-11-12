/* @CompileEnv: IDO52_COPYPROP_VAR=-28 IDO52_PRECOLOR_WEB=v0:71,v0:75 IDO52_AS1_XBB_SKIP_CAND=3,4 */
/* @SymbolStart: 0x80137660 */
/* @SymbolSize: 444 */
#include "types.h"
#include "tokimeki/class_w.h"

extern void menu_check();
extern void menu_cursor_move();
extern void option_menu_box_show();
extern void device_cmd_send();
extern void class_clear_all();
extern void text_sys_init();
extern s32  goto_step();
extern s32  sub_sub_step_up();

typedef union FlagWord {
    u32 w;
    u8  b;
} FlagWord;

typedef struct OptState {
    s16 x;
    s16 pad0;
    s16 y;
    u8  pad1[0x2A19 - 0x0006];
    u8  flag;
    u8  pad2[0x2A30 - 0x2A1A];
    s16 w;
    s16 h;
} OptState;

extern OptState cursor_cw_x;
extern volatile u8 bunkaken_flags;
extern volatile FlagWord taco_rec;
extern s8  menu_csr;
extern u8  cw96_active[];
extern u32 pad_trig;
extern CLASS_W class_w[];

s32 option_subgame_main(void)
{
    s32 f1;
    s32 f2;
    s32 t;
    s32 i;
    s32 n;
    s32 s;
    s32 ret;
    s32 one;

    one = 1;
    cursor_cw_x.flag = 128;
    cursor_cw_x.w = 4096;
    cursor_cw_x.h = 4096;

    menu_check(1, cursor_cw_x.x, cursor_cw_x.y);
    menu_cursor_move(1);
    option_menu_box_show();

    f1 = bunkaken_flags;
    f1 = f1 & 1;
    if (one == f1) {
        class_w[111].disp_flag = 128;
    }
    f2 = taco_rec.w;
    f2 = f2 & 1;
    if (one == f2) {
        class_w[112].disp_flag = 128;
    }

    s = menu_csr;
    for (i = 0, n = 0; i < 2; i++, n += 2) {
        if (i == s) {
            *(s16 *)&cw96_active[i * 68 + 1044] = n + 59;
        } else {
            *(s16 *)&cw96_active[i * 68 + 1044] = n + 60;
        }
    }

    t = pad_trig;
    if (t & 0x0020) {
        ret = s;
        switch ((u32)s) {
        case 0:
            if (one == f1) {
                bunkaken_flags = bunkaken_flags & 0xFFFD;
                device_cmd_send(1281);
                class_clear_all(0);
                text_sys_init();
                return goto_step(208);
            }
            break;
        case 1:
            if (one == f2) {
                taco_rec.b = taco_rec.b & 0xFFFD;
                device_cmd_send(1281);
                class_clear_all(0);
                text_sys_init();
                return goto_step(144);
            }
            break;
        }
        return ret;
    } else if (t & 0x0040) {
        return sub_sub_step_up();
    }
    return t;
}
