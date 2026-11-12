/* tt_menu_init - TT overlay, Address: 0x801496F4, Size: 452 bytes (113 insns) */
/* @SymbolSize: 452 */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* @PostProc: swap_instructions --offset1 0x110 --offset2 0x114 */
/* @PostProc: swap_instructions --offset1 0x114 --offset2 0x118 */
/* @PostProc: patch_insn_reg_field 0x18:rt:t2:a3 0x24:rs:t2:a3 0x24:rt:t2:a3 0x38:rt:t2:a3 0x44:rt:t0:a0 0x4c:rt:a0:a2 0x54:rt:t2:a3 0x58:rs:t0:a2 0x58:rt:t0:a2 0x68:rt:t1:t0 0x6c:rt:a2:t7 0x74:rt:a2:t7 0x78:rt:a3:t8 0x80:rt:t0:a2 0x94:rt:t1:t0 0xa4:rt:a3:t8 0xa8:rt:t7:t9 0xb0:rs:t7:t9 0xb0:rt:t8:t1 0xb8:rs:t8:t1 0xc4:rt:t0:v1 0xc8:rt:v1:a2 0xcc:rs:t0:a2 0xcc:rt:t0:a2 0xd4:rt:a2:t2 0xdc:rt:a2:t2 0xe0:rt:a3:t3 0xf8:rt:t1:t0 0x104:rt:t0:a2 0x108:rt:a3:t3 0x10c:rt:t9:t4 0x110:rt:t3:t5 0x114:rt:t4:t6 0x118:rt:t9:t4 0x11c:rt:t3:t5 0x124:rt:t4:t6 0x144:rt:a3:v0 0x148:rt:t5:t7 0x14c:rt:t6:t8 0x150:rt:t7:t1 0x154:rt:t8:t2 0x158:rt:t9:t3 0x15c:rs:t2:a3 0x160:rs:t2:a3 0x164:rs:t2:a3 0x168:rs:t2:a3 0x168:rt:t5:t7 0x16c:rs:t2:a3 0x170:rs:t2:a3 0x170:rt:t6:t8 0x174:rs:t2:a3 0x178:rs:t2:a3 0x17c:rs:t2:a3 0x180:rs:t2:a3 0x184:rs:t2:a3 0x188:rs:t2:a3 0x188:rt:a3:v0 0x18c:rs:t2:a3 0x190:rs:t2:a3 0x194:rs:t2:a3 0x198:rs:t2:a3 0x19c:rs:t2:a3 0x19c:rt:t7:t1 0x1a0:rs:t2:a3 0x1a4:rs:t2:a3 0x1a4:rt:t8:t2 0x1a8:rs:t2:a3 0x1ac:rs:t2:a3 0x1b0:rs:t2:a3 0x1b0:rt:t9:t3 0x1b4:rs:t2:a3 0x1b8:rs:t2:a3 0x1bc:rs:t2:a3 0x1bc:rt:a3:v0 0x1c0:rs:t2:a3 0x1c8:rs:t2:a3 0x1c8:rt:t2:a3 */
#include "types.h"

extern u8 *tt_game_wk;
extern u8 *tt_anim_buf;
extern u8 *tt_text_buf;

extern s16 tt_menu_x_tbl[];
extern u16 tt_menu_id_tbl[];
extern s16 tt_submenu_x_tbl[];
extern u16 tt_submenu_id_tbl[];
extern u16 tt_submenu_id_tbl_end[];

extern void SetDispMask(s32 mode);

void tt_menu_init(void)
{
    u8 *p;
    u8 *r;
    u8 *q;
    s16 *s;
    u16 *u;
    s32 y;
    s32 t;
    s32 a;
    u16 *e;

    p = tt_game_wk;
    r = tt_anim_buf;
    q = tt_text_buf;
    SetDispMask(1);

    e = &tt_menu_id_tbl[4];
    u = tt_menu_id_tbl;
    s = tt_menu_x_tbl;
    y = 160;
    do {
        a = *s;
        *(s16 *)(q + 12) = (s16)y;
        *(s16 *)(q + 10) = a;
        t = *u;
        u += 1;
        y += 8;
        s += 1;
        q += 20;
        q[-18] = 0;
        q[-20] = 1;
        *(s16 *)(q - 14) = 0;
        *(s16 *)(q - 4) = 0;
        *(s16 *)(q - 6) = (s16)t;
    } while (u < e);

    s = tt_submenu_x_tbl;
    y = 208;
    if (p[83] == 0) {
        q[-20] = 0;
    }
    e = tt_submenu_id_tbl_end;
    u = tt_submenu_id_tbl;
    do {
        a = *s;
        *(s16 *)(q + 12) = (s16)y;
        *(s16 *)(q + 10) = a;
        t = *u;
        u += 1;
        y += 8;
        s += 1;
        q += 20;
        q[-18] = 0;
        q[-20] = 1;
        *(s16 *)(q - 14) = 0;
        *(s16 *)(q - 4) = 0;
        *(s16 *)(q - 6) = (s16)t;
    } while (u != e);

    *(s16 *)(q + 10) = -112;
    *(s16 *)(q + 12) = 136;
    *(s16 *)(q + 14) = 0;
    q[2] = 3;
    q[0] = 0;
    *(s16 *)(q + 6) = 0;
    *(s16 *)(q + 16) = 0;

    r[100] = 0;
    r[101] = 0;
    *(s16 *)(r + 168) = 0;
    *(s16 *)(r + 120) = -96;
    *(s16 *)(r + 122) = 56;
    *(s16 *)(r + 160) = 128;
    *(s16 *)(r + 162) = 48;
    r[170] = 0;
    r[171] = 0;
    *(s16 *)(r + 164) = 11;
    *(s16 *)(r + 166) = 15392;
    r[159] = 101;
    *(s16 *)(r + 116) = 0;
    r += 200;
    r[0] = 0;
    r[1] = 0;
    *(s16 *)(r + 68) = 0;
    *(s16 *)(r + 20) = 32;
    *(s16 *)(r + 22) = 56;
    *(s16 *)(r + 60) = 64;
    *(s16 *)(r + 62) = 48;
    r[70] = 0;
    r[71] = 48;
    *(s16 *)(r + 64) = 11;
    *(s16 *)(r + 66) = 15392;
    r[59] = 101;
    *(s16 *)(r + 16) = 0;
}
