/* @RodataBase: 0x8013D114 */
/* @PostProc: patch_insn_reg_field 0x234:rt:t1:t3 0x238:rt:t3:t2 0x23c:rs:t1:t3 0x23c:rt:t2:t4 0x240:rd:t4:t5 0x240:rt:t3:t4 0x248:rt:t4:t5 0x254:rd:t5:t6 0x258:rs:t5:t6 0x264:rt:t7:t0 0x268:rs:t7:t0 0x268:rt:t7:t0 0x26c:rd:t6:t9 0x270:rs:t7:t0 0x270:rt:t8:t1 0x274:rd:t9:t3 0x274:rs:t6:t9 0x274:rt:t8:t1 0x280:rt:t9:t3 0x28c:rt:t0:t2 0x290:rs:t0:t2 0x29c:rt:t2:t6 0x2a0:rs:t2:t6 0x2a0:rt:t2:t6 0x2a4:rt:t1:t5 0x2a8:rs:t2:t6 0x2a8:rt:t3:t7 0x2ac:rd:t4:t8 0x2ac:rs:t1:t5 0x2ac:rt:t3:t7 0x2b8:rt:t4:t8 0x2c4:rd:t5:t0 0x2c8:rt:t5:t0 0x2d4:rt:t6:t4 0x2d8:rd:t9:t3 0x2dc:rs:t9:t3 0x2dc:rt:t0:t2 0x2e0:rs:t6:t4 0x2e0:rt:t8:t6 0x2e4:rd:t2:t5 0x2e4:rs:t8:t2 0x2e4:rt:t0:t6 0x2ec:rt:t2:t5 0x2f8:rd:t1:t7 0x2fc:rt:t1:t7 0x308:rt:t4:t3 0x30c:rd:t7:t9 0x310:rs:t7:t9 0x310:rt:t6:t1 0x314:rs:t4:t3 0x314:rt:t5:t4 0x318:rd:t9:t2 0x318:rs:t5:t1 0x318:rt:t6:t4 0x320:rt:t9:t2 0x32c:rd:t8:t6 0x330:rt:t8:t6 0x33c:rt:t2:t9 0x340:rd:t3:t8 0x344:rs:t3:t8 0x344:rt:t4:t0 0x348:rs:t2:t9 0x348:rt:t1:t3 0x34c:rd:t7:t1 0x34c:rs:t1:t0 0x34c:rt:t4:t3 0x354:rt:t7:t1 0x360:rd:t5:t4 0x364:rt:t5:t4 0x370:rt:t9:t8 0x374:rd:t0:t5 0x378:rs:t0:t5 0x378:rt:t2:t7 0x37c:rs:t9:t8 0x37c:rt:t8:t9 0x380:rd:t3:t0 0x380:rs:t8:t7 0x380:rt:t2:t9 0x388:rt:t3:t0 */
/* Address: 0x8013AF30, Size: 1196 bytes (299 instructions) */
#include "types.h"

extern void menu_check();
extern void menu_cursor_move();
extern void menu_bar_show();
extern void debug_string_show();
extern void debug_number_show();
extern s32 option_value_step();

extern s16 cursor_cw_x;
extern s16 cursor_cw_y;
extern u8 *option_menu1_lbl_tbl[];
typedef struct {
    u8 pad[244];
    union {
        u32 w;
        u16 h[2];
        u8 b[4];
    } f;
} OptCfg;

extern OptCfg screen_w;
extern u8 byouki_kind;
extern u8 byouki_state;
extern s16 para_tai;
extern s16 para_bun;
extern s16 para_ri;
extern s16 para_gei;
extern s16 para_un;
extern s16 para_zat;
extern s16 para_you;
extern s16 para_kon;
extern s16 para_str;
extern u32 pad_data;
extern u8 now_icon;

void option_menu1_main1(void)
{
    u8 **p;
    OptCfg *q;
    s32 y;
    s32 v;

    menu_check(0, cursor_cw_x, cursor_cw_y);
    menu_cursor_move(0);
    menu_bar_show(0);

    q = &screen_w;
    p = option_menu1_lbl_tbl;
    y = -96;
    do {
        debug_string_show(*p, -32, y);
        y += 12;
        p++;
    } while (y != 96);

    debug_number_show(q->f.b[1] & 0xF, 32, -96);
    debug_number_show((u32)q->f.h[0] >> 12, 32, -84);
    debug_number_show(q->f.h[1] & 1, 32, -72);
    debug_number_show((q->f.w << 14) >> 31, 32, -60);
    debug_number_show((q->f.w << 13) >> 31, 32, -48);
    debug_number_show((q->f.w << 12) >> 31, 32, -36);
    debug_number_show((q->f.w << 11) >> 31, 32, -24);
    debug_number_show(para_tai, 32, -12);
    debug_number_show(para_bun, 32, 0);
    debug_number_show(para_ri, 32, 12);
    debug_number_show(para_gei, 32, 24);
    debug_number_show(para_un, 32, 36);
    debug_number_show(para_zat, 32, 48);
    debug_number_show(para_you, 32, 60);
    debug_number_show(para_kon, 32, 72);
    debug_number_show(para_str, 32, 84);

    if ((pad_data & 0x90) != 0) {
        switch ((u32)now_icon) {
        case 0:
            v = option_value_step(q->f.b[1] & 0xF, 3, 0);
            q->f.b[1] = (u8)((v & 0xF) | (q->f.b[1] & 0xFFF0));
            break;
        case 1:
            v = option_value_step((u32)q->f.h[0] >> 12, 11, 0);
            *(volatile u8 *)&q->f.b[1] =
                (u8)((v << 4) | (byouki_kind & 0xFF0F));
            break;
        case 2:
            v = option_value_step(q->f.h[1] & 1, 1, 0);
            *(volatile u8 *)&q->f.b[2] =
                (u8)((v & 1) | (byouki_state & 0xFFFE));
            break;
        case 3:
            v = option_value_step((q->f.w << 14) >> 31, 1, 0);
            q->f.b[2] = (u8)(((v << 1) & 2) |
                                   (q->f.b[2] & 0xFFFD));
            break;
        case 4:
            v = option_value_step((q->f.w << 13) >> 31, 1, 0);
            q->f.b[2] = (u8)(((v << 2) & 4) |
                                   (q->f.b[2] & 0xFFFB));
            break;
        case 5:
            v = option_value_step((q->f.w << 12) >> 31, 1, 0);
            q->f.b[2] = (u8)(((v << 3) & 8) |
                                   (q->f.b[2] & 0xFFF7));
            break;
        case 6:
            v = option_value_step((q->f.w << 11) >> 31, 1, 0);
            q->f.b[2] = (u8)(((v << 4) & 0x10) |
                                   (q->f.b[2] & 0xFFEF));
            break;
        case 7:
            para_tai = (s16)option_value_step(para_tai, 999, 0);
            break;
        case 8:
            para_bun = (s16)option_value_step(para_bun, 999, 0);
            break;
        case 9:
            para_ri = (s16)option_value_step(para_ri, 999, 0);
            break;
        case 10:
            para_gei = (s16)option_value_step(para_gei, 999, 0);
            break;
        case 11:
            para_un = (s16)option_value_step(para_un, 999, 0);
            break;
        case 12:
            para_zat = (s16)option_value_step(para_zat, 999, 0);
            break;
        case 13:
            para_you = (s16)option_value_step(para_you, 999, 0);
            break;
        case 14:
            para_kon = (s16)option_value_step(para_kon, 999, 0);
            break;
        case 15:
            para_str = (s16)option_value_step(para_str, 999, 0);
            break;
        }
    }
}
