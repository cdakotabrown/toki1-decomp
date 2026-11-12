/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:0 */
/* @SymbolStart: 0x80137890 */
/* @RodataBase: 0x8013ada0 */
/* bunka_sd_main - BUNKA_SD overlay, Address: 0x80137898, Size: 668 bytes */
/* @SymbolSize: 676 */
#include "types.h"

extern void update_anim_state(s32);
extern void hizuke_disp_switch(s32);
extern void message_window_show(void);
extern s32 sub_step_up(void);
extern s32 goto_sub_step(s32);
extern void goto_step(s32);
extern s32 goto_sub_sub_step(s32);
extern s32 bunka_sd_pad6(void);
extern s32 bunka_sd_pad4(void);
extern s32 bunka_sd_pad5(void);
extern s32 bunka_sd_pad3(void);
extern s32 bunka_sd_pad9(void);
extern s32 bunka_sd_pad1(void);
extern s32 bunka_sd_pad2(void);
extern s32 bunka_sd_pad8(void);
extern s32 bunka_sd_pad7(void);

extern s32 bunka_bukatu;
extern u8 now_sub_step;
extern u8 now_year;
extern u16 basyo_pack;
extern u8 bunka_ret_sub_step;
extern u8 bunka_ret_sub_sub_step;

s32 bunka_sd_main(void)
{
    s32 ret;
    s32 mode;
    u8 charId;
    u32 v;

    ret = now_sub_step;
    switch ((u32)now_sub_step) {
    case 0:
        update_anim_state(2);
        hizuke_disp_switch(0);
        message_window_show();
        ret = sub_step_up();
        break;
    case 1:
        mode = bunka_bukatu;
        switch ((u32)mode) {
        case 2:
            charId = now_year;
            switch (charId) {
            case 95: goto_sub_step(240); break;
            case 96: goto_sub_step(241); break;
            case 97: goto_sub_step(242); break;
            }
            break;
        case 3:
            goto_sub_step(243);
            break;
        case 5:
            goto_sub_step(244);
            break;
        case 0:
            goto_sub_step(245);
            break;
        case 1:
            charId = now_year;
            switch (charId) {
            case 95: goto_sub_step(246); break;
            case 96: goto_sub_step(247); break;
            case 97: goto_sub_step(248); break;
            }
            break;
        }
        return;
    case 16:
        v = basyo_pack;
        if ((v >> 12) < 6U) {
            goto_step(112);
        } else {
            goto_step(113);
        }
        goto_sub_step(bunka_ret_sub_step);
        ret = goto_sub_sub_step(bunka_ret_sub_sub_step);
        break;
    case 240: ret = bunka_sd_pad6(); break;
    case 241: ret = bunka_sd_pad4(); break;
    case 242: ret = bunka_sd_pad5(); break;
    case 243: ret = bunka_sd_pad3(); break;
    case 244: ret = bunka_sd_pad9(); break;
    case 245: ret = bunka_sd_pad1(); break;
    case 246: ret = bunka_sd_pad2(); break;
    case 247: ret = bunka_sd_pad8(); break;
    case 248: ret = bunka_sd_pad7(); break;
    }
    return ret;
}
