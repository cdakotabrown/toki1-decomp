/* @SymbolSize: 588 */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=112:t0 */
#include "types.h"
#include "tokimeki/class_w.h"

extern u8 album_work[];
extern u8 mascot_flag[];
extern u8 screen_w[];
extern u8 cw96_active[][68];
extern volatile s16 cw159_scale_x;
extern volatile s16 cw159_scale_y;
extern s16 cursor_cw_x;
extern volatile s16 cursor_cw_y;
extern s8 menu_csr;
extern s32 pad_trig;
extern u8 now_sub_sub_step;
extern u8 option_talk_main_str0[];
extern u8 option_talk_sect_fmt[];
extern s32 option_talk_xa_tbl[];
extern s32 work_fuku;
extern s32 option_talk_xa_flag;
extern CLASS_W class_w[];

extern void menu_check();
extern void menu_cursor_move(s32 a0);
extern s32 get_CD_State(void);
extern void device_cmd_send(s32 a0);
extern void printf();
extern void goto_sub_sub_step(s32 a0);
extern void set_XAsector(s32 a0, s32 a1, s32 a2);
extern void sub_sub_step_up(void);
extern void option_menu_box_show(void);

void option_talk_main(void)
{
    s32 i;
    s32 idx;
    s32 sel;
    u8 *p;
    s32 n;

    p = &album_work[0]; for (i = 0; i < 14; i++) { if (p[20] != 0 || screen_w[i + 1340] != 0) { cw96_active[0][i * 68 + 1023] = 128; } p++; }

    class_w[159].disp_flag = 128;
    cw159_scale_x = 4096;
    cw159_scale_y = 4096;
    menu_check(1, cursor_cw_x, cursor_cw_y);
    menu_cursor_move(1);

    idx = menu_csr;
    for (i = 0, n = 0; i < 14; i++, n += 2) {
        if (i == idx) {
            *(s16 *)&cw96_active[i][1044] = n + 63;
        } else {
            *(s16 *)&cw96_active[i][1044] = n + 64;
        }
    }

    if ((pad_trig & 0x40) != 0) {
        device_cmd_send(116);
        printf(option_talk_main_str0);
        menu_csr = -1;
        goto_sub_sub_step((u8)(now_sub_sub_step + 2));
    } else if ((pad_trig & 0x20) != 0) {
        sel = menu_csr;
        if (mascot_flag[menu_csr] != 0 || screen_w[menu_csr + 1340] != 0) {
            if ((get_CD_State() & 2) != 0) {
                if (sel >= 0 && sel < 14) {
                    if (((s32 *)option_talk_xa_tbl[sel])[0] != 0) {
                        work_fuku = 1;
                        option_talk_xa_flag = 0;
                        set_XAsector(((s32 *)option_talk_xa_tbl[sel])[1],
                                      ((s32 *)option_talk_xa_tbl[sel])[2], sel);
                        printf(option_talk_sect_fmt, ((s32 *)option_talk_xa_tbl[sel])[1],
                                      ((s32 *)option_talk_xa_tbl[sel])[2]);
                        device_cmd_send(768);
                        sub_sub_step_up();
                    }
                }
            }
        } else {
            device_cmd_send(116);
        }
    }

    option_menu_box_show();
}
