/* @CompileFlags: -Wc,-pic0 */
/* @RodataBase: 0x8015CE58 */
/* rpg_bat_menu - RPG_BAT overlay, Address: 0x8014BB80, Size: 1124 bytes */
/* - `@RodataBase: 0x8015CE58` pins this function's own in-EXN jr table */
/* @PostProc: patch_insn_reg_field 0x24:rt:v0:v1 0x28:rs:v0:v1 0x28:rt:v0:v1 0x30:rt:t1:t0 0x3c:rt:t1:t0 0x40:rt:t1:t0 0x4c:rt:t1:t0 0x50:rt:t1:t0 0x5c:rs:v0:v1 0x60:rt:t1:t0 0x64:rt:t2:t1 0x68:rs:t2:t1 0x68:rt:t2:t1 0x70:rs:t2:t1 0x70:rt:t3:t2 0x74:rs:t3:t2 0x84:rt:v0:v1 0x88:rs:v0:v1 0x88:rt:v0:v1 0x90:rt:t0:v0 0x94:rs:t0:v0 0x94:rt:v1:v0 0x9c:rd:t4:t3 0x9c:rt:v1:v0 0xa0:rs:t4:t3 0xa8:rs:v0:v1 0xb0:rs:v0:v1 0xb8:rt:v0:v1 0xc0:rt:v0:v1 0xc4:rs:v0:v1 0xcc:rd:t5:t4 0xcc:rt:v0:v1 0xd4:rt:t5:t4 0xd8:rt:t5:t4 0xe0:rs:t5:t4 0xfc:rt:t9:t5 0x100:rs:t9:t5 0x100:rt:t9:t5 0x108:rs:t9:t5 0x110:rt:t8:t9 0x114:rt:t6:t8 0x118:rt:t6:t8 0x11c:rt:t8:t9 0x13c:rt:t7:t6 0x140:rt:t1:t7 0x144:rt:t1:t7 0x148:rt:t7:t6 0x1a0:rt:t2:t0 0x1a4:rs:t2:t0 0x1a4:rt:t2:t0 0x1ac:rs:t2:t0 0x224:rt:v0:v1 0x228:rs:v0:v1 0x228:rt:v0:v1 0x230:rs:v0:v1 0x238:rs:v0:v1 0x244:rs:v0:v1 0x250:rs:v0:v1 0x2b8:rt:t3:t1 0x2bc:rs:t3:t1 0x2bc:rt:t3:t1 0x2c4:rs:t3:t1 0x2c8:rt:t4:t2 0x2cc:rs:t4:t2 0x2cc:rt:t4:t2 0x2d4:rs:t4:t2 0x2d4:rt:t5:t3 0x2d8:rs:t5:t3 0x300:rt:v0:v1 0x304:rs:v0:v1 0x304:rt:v0:v1 0x308:rt:t0:v0 0x30c:rs:v0:v1 0x314:rs:v0:v1 0x318:rt:t0:v0 0x320:rs:v0:v1 0x324:rt:t0:v0 0x330:rs:t0:v0 0x330:rt:t0:v0 0x338:rd:t9:t4 0x338:rs:t0:v0 0x344:rt:t9:t4 0x348:rs:t0:v0 0x348:rt:t0:v0 0x350:rd:t8:t5 0x350:rs:t0:v0 0x35c:rt:t8:t5 0x360:rs:t0:v0 0x360:rt:t0:v0 0x368:rd:t6:t9 0x368:rs:t0:v0 0x370:rt:t6:t9 0x398:rt:t7:t8 0x3a0:rt:t7:t8 0x414:rd:t0:t6 0x414:rs:v1:v0 0x41c:rt:t0:t6 */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"
#include "tokimeki/class_w.h"

typedef struct {
    s16 a[3];
    s16 b[3];
    s16 c[3];
    s16 d[3];
} Rect4;

extern void menu_set(s32, s32, s16 *, s16 *, s16 *, s16 *);
extern void menu_check(s32, s32, s32);
extern void menu_cursor_move(s32);
extern void step_directional_scroll_once(void);
extern void rpg_bat_anim_set(s32, s32, s32, s32);
extern s32  rpg_bat_rec_flag_check(s32);
extern void rpg_bat_count_up(s32);

extern Rect4 rpg_bat_menu_tbl;
extern volatile s32 rpg_bat_menu_ctl;
extern s32 rpg_bat_bukatu_ready;
extern u8  menu_mode;
extern volatile s8 now_icon;
extern s32 pad_trig;
extern s16 cursor_cw_x;
extern s16 cursor_cw_y;
extern CLASS_W class_w[];
extern RPG_BAT_UNIT rpg_bat_unit0;

void rpg_bat_menu(void)
{
    s32 pad0;
    Rect4 buf;
    s32 state;
    s32 flags;
    s32 sel;

    state = rpg_bat_menu_ctl;
    buf = rpg_bat_menu_tbl;
    if (state != 3 && (menu_mode & 1) == 0) {
        step_directional_scroll_once();
        state = rpg_bat_menu_ctl;
    }
    flags = rpg_bat_unit0.flag;
    if ((flags << 15) >= 0 && state != 4 && state != 5) {
        state = 6;
    }
    rpg_bat_menu_ctl = state;
    switch ((u32)state) {
    case 0:
        rpg_bat_anim_set(37, 49, 5, 1);
        if (rpg_bat_bukatu_ready == 1) {
            menu_set(0, 3, buf.a, buf.b, buf.c, buf.d);
        } else {
            menu_set(0, 2, buf.a, buf.b, buf.c, buf.d);
        }
        rpg_bat_count_up(1);
        return;
    case 1:
        if (rpg_bat_rec_flag_check(37) == 0) {
            return;
        }
        rpg_bat_count_up(1);
        return;
    case 2:
        rpg_bat_anim_set(37, 2, 1, 1);
        if (rpg_bat_bukatu_ready == 1) {
            rpg_bat_anim_set(35, 255, 1, 1);
        } else {
            rpg_bat_anim_set(35, 255, 1, 0);
        }
        rpg_bat_count_up(1);
        return;
    case 3:
        menu_check(0, cursor_cw_x, cursor_cw_y);
        menu_cursor_move(0);
        rpg_bat_anim_set(34, 255, 1, 1);
        sel = now_icon;
        switch (sel) {
        case 0:
            rpg_bat_anim_set(34, 5, 1, 1);
            break;
        case 1:
            rpg_bat_anim_set(34, 6, 1, 1);
            break;
        case 2:
            rpg_bat_anim_set(34, 7, 1, 1);
            break;
        case -1:
            rpg_bat_anim_set(34, 255, 1, 0);
            break;
        }
        if (now_icon == -1) {
            break;
        }
        if ((pad_trig & 0x20) == 0) {
            break;
        }
        rpg_bat_anim_set(34, 255, 1, 0);
        rpg_bat_anim_set(35, 255, 1, 0);
        sel = now_icon;
        switch (sel) {
        case 0:
            rpg_bat_unit0.flag = rpg_bat_unit0.flag ^ 0x01000000;
            break;
        case 1:
            rpg_bat_unit0.flag = rpg_bat_unit0.flag ^ 0x02000000;
            break;
        case 2:
            rpg_bat_unit0.flag = rpg_bat_unit0.flag ^ 0x04000000;
            break;
        }
        rpg_bat_count_up(1);
        return;
    case 4:
        rpg_bat_anim_set(37, 49, 33, 1);
        class_w[133].frame = 6;
        rpg_bat_anim_set(34, 255, 1, 0);
        rpg_bat_anim_set(35, 255, 1, 0);
        rpg_bat_count_up(1);
        return;
    case 5:
        if (rpg_bat_rec_flag_check(37) == 0) {
            break;
        }
        rpg_bat_anim_set(37, 255, 1, 0);
        rpg_bat_count_up(1);
        return;
    case 6:
        rpg_bat_unit0.flag = flags & ~0x10000;
        rpg_bat_anim_set(34, 255, 1, 0);
        rpg_bat_anim_set(35, 255, 1, 0);
        rpg_bat_anim_set(37, 255, 1, 0);
        rpg_bat_menu_ctl = 0;
        break;
    }
}
