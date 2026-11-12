/* @CompileEnv: IDO52_NO_LOOP_TEST_REPLACEMENT=1 */
/* @PostProc: swap_instructions --offset1 0x14 --offset2 0x18 */
/* @PostProc: swap_instructions --offset1 0x20 --offset2 0x24 */
/* @PostProc: swap_instructions --offset1 0x24 --offset2 0x28 */
/* @PostProc: swap_instructions --offset1 0x28 --offset2 0x2c */
/* @PostProc: swap_instructions --offset1 0x2c --offset2 0x30 */
/* @PostProc: swap_instructions --offset1 0x30 --offset2 0x34 */
/* tt_trail_pool_init - TT overlay, Address: 0x80147CF8, Size: 524 bytes. */
/* @PostProc: patch_insn_reg_field 0x10:rt:v0:v1 0x14:rt:a2:t0 0x18:rs:v0:v1 0x18:rt:a0:v1 0x1c:rs:a2:t0 0x1c:rt:a2:t0 0x20:rd:v1:v0 0x24:rt:t1:t3 0x28:rt:t0:t2 0x2c:rt:a3:t1 0x30:rt:a1:a3 0x34:rt:v0:a2 0x38:rs:v1:v0 0x38:rt:v1:v0 0x3c:rs:v1:v0 0x40:rs:a0:v1 0x40:rt:a0:v1 0x44:rs:a0:v1 0x48:rs:a0:v1 0x48:rt:v0:a2 0x4c:rs:a0:v1 0x4c:rt:a1:a3 0x50:rs:a0:v1 0x50:rt:a2:t0 0x54:rs:a0:v1 0x58:rs:a0:v1 0x5c:rs:a0:v1 0x5c:rt:a3:t1 0x60:rs:a0:v1 0x60:rt:t0:t2 0x64:rs:a0:v1 0x68:rs:a0:v1 0x68:rt:t1:t3 0x6c:rs:a0:v1 0x70:rs:a0:v1 0x74:rs:a0:v1 0x78:rs:a0:v1 0x7c:rs:a0:v1 0x84:rs:a0:v1 0x88:rt:t6:v0 0x8c:rs:t6:v0 0x8c:rt:t6:v0 0x94:rs:t6:v0 0x98:rs:t6:v0 0x9c:rs:t6:v0 0xa0:rs:t6:v0 0xa4:rs:t6:v0 0xa8:rs:t6:v0 0xac:rs:t6:v0 0xb0:rs:t6:v0 0xb4:rs:t6:v0 0xb8:rs:t6:v0 0xbc:rs:t6:v0 0xbc:rt:a1:a3 0xc0:rs:t6:v0 0xc0:rt:a2:t0 0xc4:rs:t6:v0 0xc8:rs:t6:v0 0xcc:rs:t6:v0 0xd0:rs:t6:v0 0xd0:rt:t0:t2 0xd4:rs:t6:v0 0xd8:rs:t6:v0 0xdc:rs:t6:v0 0xe0:rs:t6:v0 0xe4:rs:t6:v0 0xe8:rs:t6:v0 0xec:rs:t6:v0 0xf0:rs:t6:v0 0xf4:rs:t6:v0 0xf8:rs:t6:v0 0xfc:rs:t6:v0 0xfc:rt:a1:a3 0x100:rs:t6:v0 0x100:rt:a2:t0 0x104:rs:t6:v0 0x108:rs:t6:v0 0x10c:rs:t6:v0 0x110:rs:t6:v0 0x110:rt:t0:t2 0x114:rs:t6:v0 0x118:rs:t6:v0 0x11c:rs:t6:v0 0x120:rs:t6:v0 0x124:rs:t6:v0 0x128:rs:t6:v0 0x12c:rs:t6:v0 0x130:rs:t6:v0 0x134:rs:t6:v0 0x138:rs:t6:v0 0x13c:rs:t6:v0 0x13c:rt:a1:a3 0x140:rs:t6:v0 0x140:rt:a2:t0 0x144:rs:t6:v0 0x148:rs:t6:v0 0x14c:rs:t6:v0 0x150:rs:t6:v0 0x150:rt:t0:t2 0x154:rs:t6:v0 0x158:rs:t6:v0 0x15c:rs:t6:v0 0x160:rs:t6:v0 0x164:rs:t6:v0 0x168:rs:t6:v0 0x16c:rs:t6:v0 0x170:rs:t6:v0 0x174:rs:t6:v0 0x178:rs:t6:v0 0x17c:rs:t6:v0 0x17c:rt:a1:a3 0x180:rs:t6:v0 0x180:rt:a2:t0 0x184:rs:t6:v0 0x188:rs:t6:v0 0x18c:rs:t6:v0 0x190:rs:t6:v0 0x190:rt:t0:t2 0x194:rs:t6:v0 0x198:rt:t0:t2 0x1a8:rt:a2:t0 0x1ac:rt:a1:a3 0x200:rt:a1:a3 0x204:rt:a2:t0 0x218:rt:t0:t2 */
#include "types.h"

extern u8 *tt_trail_buf;
extern u8 *tt_cel_buf;
extern u8 tt_anim_tbl[];

void tt_trail_pool_init(void)
{
    u8 *p;
    u8 *q;
    s32 i;

    p = tt_trail_buf;
    for (i = 0; i < 3; i++) {
        p[i * 164] = 0;
        *(u16 *)(p + i * 164 + 2) = 4;
        *(u16 *)(p + i * 164 + 16) = 2;
        *(u8 **)(p + i * 164 + 56) = tt_anim_tbl;
        p[i * 164 + 60] = 0;
        p[i * 164 + 61] = 0;
        p[i * 164 + 62] = 55;
        p[i * 164 + 64] = 96;
        p[i * 164 + 72] = 0;
        *(u16 *)(p + i * 164 + 4) = 64;
        *(u16 *)(p + i * 164 + 10) = 0;
        *(u16 *)(p + i * 164 + 8) = 0;
        *(u16 *)(p + i * 164 + 6) = 0;
        p[i * 164 + 15] = 0;
        p[i * 164 + 14] = 0;
        p[i * 164 + 13] = 0;
    }
    q = tt_cel_buf;
    q[192] = 0;
    *(u16 *)(q + 194) = 0;
    *(u16 *)(q + 196) = 0;
    q[204] = 0;
    *(u16 *)(q + 202) = 0;
    *(u16 *)(q + 200) = 0;
    *(u16 *)(q + 198) = 0;
    q[207] = 0;
    q[206] = 0;
    q[205] = 0;
    *(u16 *)(q + 208) = 2;
    *(u8 **)(q + 248) = tt_anim_tbl;
    q[252] = 0;
    q[253] = 0;
    q[254] = 0;
    q[256] = 96;
    q[288] = 0;
    *(u16 *)(q + 290) = 0;
    *(u16 *)(q + 292) = 0;
    q[300] = 0;
    *(u16 *)(q + 298) = 0;
    *(u16 *)(q + 296) = 0;
    *(u16 *)(q + 294) = 0;
    q[303] = 0;
    q[302] = 0;
    q[301] = 0;
    *(u16 *)(q + 304) = 2;
    *(u8 **)(q + 344) = tt_anim_tbl;
    q[348] = 0;
    q[349] = 0;
    q[350] = 0;
    q[352] = 96;
    q[480] = 0;
    *(u16 *)(q + 482) = 0;
    *(u16 *)(q + 484) = 0;
    q[492] = 0;
    *(u16 *)(q + 490) = 0;
    *(u16 *)(q + 488) = 0;
    *(u16 *)(q + 486) = 0;
    q[495] = 0;
    q[494] = 0;
    q[493] = 0;
    *(u16 *)(q + 496) = 2;
    *(u8 **)(q + 536) = tt_anim_tbl;
    q[540] = 0;
    q[541] = 0;
    q[542] = 0;
    q[544] = 96;
    q[576] = 0;
    *(u16 *)(q + 578) = 0;
    *(u16 *)(q + 580) = 0;
    q[588] = 0;
    *(u16 *)(q + 586) = 0;
    *(u16 *)(q + 584) = 0;
    *(u16 *)(q + 582) = 0;
    q[591] = 0;
    q[590] = 0;
    q[589] = 0;
    *(u16 *)(q + 592) = 2;
    *(u8 **)(q + 632) = tt_anim_tbl;
    q[636] = 0;
    q[637] = 0;
    q[638] = 0;
    q[640] = 96;
    q += 672;
    q[64] = 96;
    q[62] = 0;
    q[61] = 0;
    q[60] = 0;
    *(u8 **)(q + 56) = tt_anim_tbl;
    *(u16 *)(q + 16) = 2;
    q[13] = 0;
    q[14] = 0;
    q[15] = 0;
    *(u16 *)(q + 6) = 0;
    *(u16 *)(q + 8) = 0;
    *(u16 *)(q + 10) = 0;
    q[12] = 0;
    *(u16 *)(q + 4) = 0;
    *(u16 *)(q + 2) = 0;
    q[0] = 0;
    q[-288] = 0;
    *(u16 *)(q - 286) = 0;
    *(u16 *)(q - 284) = 0;
    q[-276] = 0;
    *(u16 *)(q - 278) = 0;
    *(u16 *)(q - 280) = 0;
    *(u16 *)(q - 282) = 0;
    q[-273] = 0;
    q[-274] = 0;
    q[-275] = 0;
    *(u16 *)(q - 272) = 2;
    *(u8 **)(q - 232) = tt_anim_tbl;
    q[-228] = 0;
    q[-227] = 0;
    q[-226] = 0;
    q[-224] = 96;
}
