/* tt_board_step - TT overlay, Address: 0x801437E0, Size: 384 bytes (96 insns) */
/* - `@CompileEnv: IDO52_NO_LOOP_TEST_REPLACEMENT=1` (Hook 20).  The panel */
/* @SymbolSize: 384 */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* @CompileEnv: IDO52_NO_LOOP_TEST_REPLACEMENT=1 */
/* @PostProc: swap_instructions --offset1 0x14 --offset2 0x28 */
/* @PostProc: swap_instructions --offset1 0x6c --offset2 0x70 */
/* @PostProc: swap_instructions --offset1 0x68 --offset2 0x6c */
/* @PostProc: swap_instructions --offset1 0x64 --offset2 0x68 */
/* @PostProc: swap_instructions --offset1 0x60 --offset2 0x64 */
/* @PostProc: patch_insn_reg_field 0x1c:rt:s0:s1 0x2c:rs:s0:s1 0x40:rt:t3:t5 0x54:rs:s0:s1 0x54:rt:t0:t2 0x5c:rt:a1:a3 0x60:rs:a1:a3 0x60:rt:a1:a3 0x64:rt:a3:t0 0x74:rt:t2:t4 0x78:rs:t0:t2 0x78:rt:t1:t3 0x7c:rs:s0:s1 0x80:rs:s0:s1 0x84:rs:s0:s1 0x88:rs:s0:s1 0x8c:rs:s0:s1 0x90:rs:s0:s1 0x94:rs:s0:s1 0x98:rs:s0:s1 0x98:rt:a1:a3 0x9c:rs:s0:s1 0xa0:rs:s0:s1 0xa4:rs:s0:s1 0xa8:rs:s0:s1 0xa8:rt:a3:t0 0xac:rs:s0:s1 0xb0:rs:s0:s1 0xb0:rt:t1:t3 0xb4:rs:s0:s1 0xb8:rs:s0:s1 0xbc:rs:s0:s1 0xbc:rt:t2:t4 0xc8:rt:v1:a1 0xcc:rt:a0:t1 0xd0:rt:v1:a1 0xd4:rs:v1:a1 0xd4:rt:v1:a1 0xd8:rs:v1:a1 0x108:rt:a0:t1 0x10c:rt:a1:a3 0x11c:rt:a3:t0 0x120:rt:s0:s1 0x124:rs:s0:s1 0x124:rt:s0:s1 0x128:rd:s1:s0 0x134:rs:s0:s1 0x138:rs:s1:s0 0x138:rt:s1:s0 0x13c:rs:s1:s0 0x140:rs:s0:s1 0x140:rt:s0:s1 0x14c:rs:t3:t5 0x14c:rt:t3:t5 0x154:rs:t3:t5 0x154:rt:t4:t6 0x15c:rs:t4:t6 0x168:rs:s0:s1 0x16c:rs:s0:s1 0x170:rs:s0:s1 0x174:rs:s0:s1 */
#include "types.h"

extern u8 *tt_player_wk;
extern u8 *tt_trail_buf;
extern u8 *tt_cel_buf;
extern u8 tt_anim_tbl[];

extern void tt_cmd_once(s32 id);
extern void trail_init(u8 *rec);

void tt_board_step(void)
{
    u8 *p;
    u8 *q;
    u8 *r;
    s32 i;
    s32 n;

    p = tt_player_wk;
    switch ((u32)*(u16 *)(p + 6)) {
    case 0:
        tt_cmd_once(1550);
        p[0] = 1;
        *(s32 *)(p + 32) = 0;
        *(s32 *)(p + 36) = 0x00F80000;
        *(s32 *)(p + 40) = 0;
        *(s32 *)(p + 44) = 0xFFFE0000;
        *(s16 *)(p + 20) = 0;
        *(s16 *)(p + 22) = 248;
        *(s32 *)(p + 56) = (s32)tt_anim_tbl;
        p[60] = 0;
        p[61] = 0;
        p[62] = 0;
        p[64] = 96;
        p[72] = 0;
        *(u16 *)(p + 80) = *(u16 *)(p + 80) | 2;
        p[82] = 1;
        p[86] = 1;
        *(s16 *)(p + 6) = 64;

        q = tt_cel_buf;
        i = 5;
        do {
            q[62] = (u8)i;
            i += 5;
            q += 96;
            q[-96] = 1;
            *(s16 *)(q - 94) = 0;
            *(s16 *)(q - 92) = 0;
            q[-84] = 0;
            *(s16 *)(q - 86) = 0;
            *(s16 *)(q - 88) = 0;
            *(s16 *)(q - 90) = 0;
            q[-81] = 0;
            q[-82] = 0;
            q[-83] = 0;
            *(s16 *)(q - 80) = 2;
            *(s32 *)(q - 40) = (s32)tt_anim_tbl;
            q[-36] = 0;
            q[-35] = 0;
            q[-32] = 96;
        } while (i < 15);

        r = tt_trail_buf;
        n = 0;
        do {
            trail_init(r);
            n += 1;
            r += 164;
        } while (n != 3);
        break;
    case 64:
        if (*(s32 *)(tt_player_wk + 36) < 0x00C00000) {
            *(s32 *)(p + 36) = 0x00C00000;
            p[86] = 0;
            *(s16 *)(p + 6) = 0;
            p[13] = 1;
        }
        break;
    }
}
