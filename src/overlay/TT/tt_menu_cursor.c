/* tt_menu_cursor - TT overlay, Address: 0x801494C8, Size: 556 bytes (139 insns) */
/* @PostProc: swap_instructions --offset1 0x17c --offset2 0x180 */
/* @PostProc: patch_insn_reg_field 0x88:rt:t4:t6 0xbc:rt:v1:t3 0xc4:rs:v1:t3 0xc4:rt:v1:t4 0xc8:rs:v1:t4 0xc8:rt:t3:t5 0xcc:rs:t3:t5 0xd0:rt:v1:t4 0xdc:rs:t4:t6 0x114:rt:v1:t7 0x11c:rs:v1:t7 0x11c:rt:v1:t8 0x120:rs:v1:t8 0x120:rt:t5:t9 0x124:rt:t5:t9 0x12c:rt:v1:t8 0x134:rt:t7:t2 0x138:rt:t6:t1 0x13c:rd:t8:t3 0x13c:rt:t7:t2 0x140:rd:t8:t3 0x140:rs:t8:t3 0x140:rt:t7:t2 0x144:rd:t8:t3 0x144:rt:t8:t3 0x148:rd:t9:t4 0x148:rt:t8:t3 0x14c:rs:t9:t4 0x14c:rt:t6:t1 0x154:rt:t1:t5 0x164:rt:t1:t5 0x168:rt:t2:t6 0x170:rd:t3:t7 0x170:rt:t2:t6 0x174:rs:t3:t7 0x174:rt:t4:t8 0x178:rt:t5:t9 0x17c:rt:t8:t3 0x180:rt:t7:t2 0x184:rt:t6:t1 0x188:rt:t4:t8 0x190:rt:t5:t9 0x194:rt:t7:t2 0x198:rt:t8:t3 0x19c:rt:t6:t1 0x1b0:rt:t9:t4 0x1b4:rs:t9:t4 0x1b8:rt:t1:t5 0x1bc:rs:t1:t5 0x1c0:rt:t4:t8 0x1d8:rt:t2:t6 0x1e0:rs:t2:t6 0x1e0:rt:t3:t7 0x1e4:rt:t3:t7 0x1fc:rt:t4:t8 0x204:rs:t4:t8 0x204:rt:t5:t9 0x20c:rt:t5:t9 0x214:rt:t7:t2 0x21c:rs:t7:t2 */
#include "types.h"

extern u8 *tt_game_wk;
extern u8 *tt_step_wk;
extern u8 *tt_scroll_wk;
extern u8 *tt_anim_buf;
extern u8 *tt_text_buf;

extern void device_cmd_send(s32 id);
extern void tt_cmd_once(s32 id);
extern void tt_shot_command_step(void);
extern void tt_view_reset(void);

void tt_menu_cursor(void)
{
    u8 *g;
    u32 unused0;
    u32 unused1;
    u8 *hud;
    u8 *pl;
    u8 *ctl;
    u8 *spr;
    u8 *tbl;
    u32 limit;
    u32 flags;
    u32 n;

    g = tt_game_wk;
    hud = tt_step_wk;
    pl = tt_scroll_wk;
    spr = tt_anim_buf;
    ctl = g + 16;
    tbl = tt_text_buf;

    limit = 3;
    if (g[83] == 0) {
        limit = 2;
    }

    tbl[pl[10] * 20 + 2] = 0;

    flags = *(u16 *)(ctl + 4);
    if (flags & 0x1000) {
        tt_cmd_once(1555);
        *(s16 *)(hud + 38) = 0;
        n = pl[10] - 1;
        pl[10] = (u8)n;
        if ((u8)n == 255) {
            pl[10] = (u8)limit;
        }
    } else if (flags & 0x4000) {
        tt_cmd_once(1555);
        *(s16 *)(hud + 38) = 0;
        n = pl[10] + 1;
        pl[10] = (u8)n;
        if (limit < (u32)(u8)n) {
            pl[10] = 0;
        }
    }

    tbl[pl[10] * 20 + 2] = 2;
    *(s8 *)(spr + 0) = 1;
    spr[1] = 0;
    *(s16 *)(spr + 68) = 0;
    *(s16 *)(spr + 20) = -44;
    *(s16 *)(spr + 22) = (s16)(pl[10] * 8 + 160);
    *(s16 *)(spr + 16) = 1;
    spr[59] = 99;
    *(s16 *)(spr + 60) = 88;
    *(s16 *)(spr + 62) = 8;
    spr[56] = 128;
    spr[57] = 64;
    spr[58] = 64;

    flags = *(u16 *)(ctl + 4);
    if ((flags & 0x0800) || (flags & 0x0020)) {
        device_cmd_send(177);
        tt_cmd_once(1554);
        *(s16 *)(hud + 2) = (s16)(*(u16 *)(hud + 2) + 1);
        hud[34] = 0;
        *(s16 *)(hud + 4) = 0;
        *(s16 *)(pl + 16) = 0;
        *(s16 *)(pl + 18) = 0;
        tt_view_reset();
    }

    if (g[83] == 1) {
        if (hud[36] == 0) {
            tt_shot_command_step();
        }
    }
}
