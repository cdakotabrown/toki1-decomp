/* @SymbolStart: 0x80143A34 */
/* @SymbolSize: 1444 */
/* @CompileEnv: IDO52_EXCLUDE_REGS_WEB=171:v0,64:a1 */
/* @CompileFlags: -Wc,-pic1 */
/* Address: 0x80143A34 (cross-prologue head absorbed), 1444 bytes / 361 insns */
/* @PostProc: patch_stack_complete --old 64 --new 72 */
/* @PostProc: swap_instructions --offset1 0x1d0 --offset2 0x1d8 */
/* @PostProc: swap_instructions --offset1 0x1e0 --offset2 0x1e4 */
/* @PostProc: swap_instructions --offset1 0x23c --offset2 0x244 */
/* @PostProc: swap_instructions --offset1 0x24c --offset2 0x250 */
/* @PostProc: swap_instructions --offset1 0x398 --offset2 0x39c */
/* @PostProc: swap_instructions --offset1 0x49c --offset2 0x4a0 */
/* @PostProc: patch_insn_reg_field 0x24:rt:a3:t0 0x28:rs:a3:t0 0x28:rt:a3:t0 0x38:rd:t1:a2 0x3c:rs:a3:t0 0x48:rs:a3:t0 0x50:rs:a3:t0 0x54:rt:t4:t5 0x5c:rs:a3:t0 0x5c:rt:t4:t5 0x60:rt:t5:t6 0x64:rt:t7:t8 0x68:rs:t5:t6 0x68:rt:t6:t7 0x6c:rs:t6:t7 0x74:rs:a3:t0 0x78:rs:a3:t0 0x78:rt:t7:t8 0x84:rs:a3:t0 0x88:rt:t8:t9 0x90:rs:a3:t0 0x90:rt:t8:t9 0x94:rs:a3:t0 0x98:rs:a3:t0 0xa0:rt:t9:t5 0xa4:rs:a3:t0 0xa4:rt:t9:t5 0xa8:rs:a3:t0 0xb4:rt:t4:t6 0xb8:rs:a3:t0 0xb8:rt:t4:t6 0xbc:rs:t1:a2 0xbc:rt:v1:v0 0xc0:rt:t6:t8 0xc4:rs:v1:v0 0xc4:rt:t5:t7 0xc8:rs:t5:t7 0xcc:rs:v1:v0 0xcc:rt:t7:t9 0xd4:rs:a3:t0 0xd4:rt:t6:t8 0xd8:rs:t7:t9 0xdc:rt:t8:t5 0xe4:rs:a3:t0 0xe4:rt:t8:t5 0xe8:rs:a3:t0 0xec:rs:a3:t0 0xfc:rs:a3:t0 0xfc:rt:a2:a0 0x104:rs:a2:a0 0x10c:rs:a3:t0 0x114:rd:t9:t6 0x118:rd:t9:t6 0x118:rs:t9:t6 0x11c:rd:t4:t6 0x11c:rt:t9:t6 0x120:rt:t4:t6 0x124:rd:t5:t7 0x128:rs:a3:t0 0x128:rt:t5:t7 0x130:rs:a2:a0 0x130:rt:t4:t6 0x134:rd:t6:t8 0x138:rs:a3:t0 0x138:rt:t6:t8 0x140:rs:t1:a2 0x140:rt:t4:t8 0x144:rs:a3:t0 0x148:rs:a3:t0 0x148:rt:a0:t6 0x14c:rd:t7:t9 0x150:rt:t7:t9 0x154:rd:t8:t5 0x158:rs:a3:t0 0x158:rt:t8:t5 0x160:rs:a0:t6 0x160:rt:t7:t9 0x164:rd:t9:t7 0x168:rs:a3:t0 0x168:rt:t9:t7 0x170:rs:t1:a2 0x170:rt:t4:t8 0x178:rs:t4:t8 0x178:rt:t5:t9 0x17c:rs:t5:t9 0x188:rs:a3:t0 0x198:rt:t6:t5 0x1a0:rs:t6:t5 0x1a8:rt:t7:t6 0x1b0:rs:t7:t6 0x1b8:rt:t8:t7 0x1c0:rs:t8:t7 0x1d0:rt:t1:a2 0x1d8:rt:a3:t0 0x1e0:rt:t1:a2 0x1e4:rt:a3:t0 0x1e8:rt:t0:t1 0x1ec:rt:t2:t3 0x1f0:rt:t0:t1 0x1f4:rt:t2:t3 0x210:rt:t0:t1 0x214:rt:t2:t3 0x218:rt:t0:t1 0x220:rt:t2:t3 0x224:rt:t9:t8 0x22c:rs:t9:t8 0x23c:rt:t1:a2 0x244:rt:a3:t0 0x24c:rt:t1:a2 0x250:rt:a3:t0 0x254:rt:t0:t1 0x258:rt:t2:t3 0x25c:rt:t0:t1 0x260:rt:t2:t3 0x28c:rt:t2:t3 0x290:rt:t0:t1 0x294:rs:a3:t0 0x294:rt:t4:t9 0x29c:rs:t4:t9 0x2c0:rt:t0:t1 0x2c4:rt:t2:t3 0x30c:rt:t0:t1 0x310:rt:t2:t3 0x358:rt:t0:t1 0x35c:rt:t2:t3 0x384:rs:t1:a2 0x388:rt:t0:t1 0x394:rt:t2:t3 0x3a8:rd:t3:t4 0x3b0:rt:a1:a2 0x3b4:rt:t4:t5 0x3bc:rs:t4:t5 0x3c4:rt:t5:t6 0x3cc:rs:t5:t6 0x3d4:rs:a3:t0 0x3d4:rt:t6:t7 0x3dc:rs:t6:t7 0x3e4:rs:a3:t0 0x3e4:rt:t7:t8 0x3ec:rs:t7:t8 0x404:rt:a3:t0 0x410:rt:a3:t0 0x414:rt:t0:t1 0x418:rt:t2:t3 0x41c:rt:t0:t1 0x420:rt:t2:t3 0x43c:rt:t0:t1 0x440:rt:t0:t1 0x444:rt:t0:t1 0x448:rs:a3:t0 0x448:rt:a0:t9 0x44c:rt:t2:t3 0x468:rt:a0:t9 0x46c:rs:a3:t0 0x470:rt:a1:a2 0x478:rs:t0:t1 0x478:rt:t3:t4 0x480:rt:a1:a2 0x488:rs:a3:t0 0x494:rt:a2:a3 0x498:rt:t1:t2 0x49c:rt:t9:t5 0x4a0:rt:t8:t6 0x4a4:rs:t8:t6 0x4a4:rt:t8:t6 0x4a8:rs:t9:t5 0x4a8:rt:t9:t5 0x4b0:rs:a2:a3 0x4b0:rt:t8:t6 0x4b4:rs:a2:a3 0x4b4:rt:t9:t5 0x4b8:rt:t4:t7 0x4c0:rs:t4:t7 0x4c8:rt:t5:t8 0x4d0:rs:t5:t8 0x4d8:rs:a3:t0 0x4d8:rt:t6:t9 0x4e0:rs:t6:t9 0x4e8:rs:a3:t0 0x4e8:rt:t7:t5 0x4f0:rs:t7:t5 0x4f8:rt:t0:t1 0x4fc:rt:t2:t3 0x518:rt:t0:t1 0x51c:rt:t0:t1 0x520:rt:t0:t1 0x524:rt:t2:t3 0x540:rs:a3:t0 0x540:rt:t8:t6 0x544:rt:t3:t4 0x54c:rt:t8:t6 0x554:rt:a1:a2 0x55c:rs:a2:a3 0x55c:rt:a2:a3 0x560:rs:a2:a3 0x560:rt:t1:t2 0x568:rs:t3:t4 0x570:rs:a3:t0 0x570:rt:t9:t7 0x578:rs:t9:t7 0x584:rs:a3:t0 0x584:rt:t0:t1 0x588:rs:a3:t0 0x588:rt:t0:t1 0x58c:rs:a3:t0 0x58c:rt:t4:t8 0x594:rs:t4:t8 0x594:rt:t6:t5 0x598:rs:a3:t0 0x598:rt:t6:t5 0x59c:rs:t6:t5 0x59c:rt:t7:t6 0x5a0:rs:a3:t0 0x5a0:rt:t7:t6 */
#include "types.h"

extern void tt_cmd_once(s32 arg0);

extern u8 *tt_game_wk;
extern u8 *tt_player_wk;
extern u8 *tt_player_shot_buf;
extern u8 *tt_rec_pool;
extern u8 *tt_recs_buf;

void tt_player_pad_step(void)
{
    u8 *pad;
    u8 *st;
    u8 *q;
    u8 *p;
    u8 *pc;
    u8 *pb;
    u8 *base;
    s32 n;
    s32 bank;
    s32 vx;
    s32 vy;
    u32 k;
    u8 flag;
    u8 tog;
    u16 keys;

    base = tt_game_wk;
    st = tt_player_wk;
    pad = base + 32;

    if (*(u16 *)(base + 32) & 0x8000) {
        *(s32 *)(st + 40) = -1;
        st[89] = 0;
        if ((u32)st[88] < 8U) {
            st[88] = st[88] + 1;
        }
    } else if (*(u16 *)pad & 0x2000) {
        *(s32 *)(st + 40) = 1;
        st[88] = 0;
        if ((u32)st[89] < 8U) {
            st[89] = st[89] + 1;
        }
    } else {
        *(s32 *)(st + 40) = 0;
        if (st[88] != 0) {
            st[88] = st[88] - 1;
        }
        if (st[89] != 0) {
            st[89] = st[89] - 1;
        }
    }

    keys = *(u16 *)pad;
    if (keys & 0x1000) {
        *(s32 *)(st + 44) = -1;
    } else if (keys & 0x4000) {
        *(s32 *)(st + 44) = 1;
    } else {
        *(s32 *)(st + 44) = 0;
    }

    vx = *(s32 *)(st + 40);
    if (vx != 0 && *(s32 *)(st + 44) != 0) {
        vy = *(s32 *)(st + 44);
        k = (u32)((st[84] * 3) << 13);
        *(s32 *)(st + 40) = vx * k;
        *(s32 *)(st + 44) = vy * k;
    } else {
        k = (u32)(st[84] << 15);
        vy = *(s32 *)(st + 44);
        *(s32 *)(st + 40) = vx * k;
        *(s32 *)(st + 44) = vy * k;
    }

    if (*(u16 *)(pad + 4) & 1) {
        p = tt_player_shot_buf;
        n = 0;
        do {
            n++;
            if (st[85] == 10) {
                if (p[0] == 0 && p[168] == 0 && p[336] == 0) {
                    tt_cmd_once(1544);

                    p[0] = 1;
                    *(s16 *)(p + 4) = 32;
                    p[13] = 0;
                    p[14] = 0;
                    p[15] = 0;
                    *(s16 *)(p + 6) = 0;
                    *(s16 *)(p + 8) = 0;
                    *(s16 *)(p + 10) = 0;
                    p[168] = 1;
                    *(s16 *)(p + 172) = 32;
                    p[336] = 1;
                    *(s16 *)(p + 340) = 32;
                    break;
                }
            } else if (p[0] == 0) {
                tt_cmd_once(1544);
                p[0] = 1;
                *(s16 *)(p + 4) = 32;
                p[13] = 0;
                p[14] = 0;
                p[15] = 0;
                *(s16 *)(p + 6) = 0;
                *(s16 *)(p + 8) = 0;
                *(s16 *)(p + 10) = 0;
                break;
            }
            p += 84;
        } while (n < 2);

        if (st[83] != 0) {

            q = tt_player_shot_buf + 504;
            n = 6;
            do {
                n++;
                if (q[0] == 0) {
                    q[0] = 1;
                    *(s16 *)(q + 4) = 32;
                    *(s16 *)(q + 10) = 0;
                    *(s16 *)(q + 8) = 0;
                    *(s16 *)(q + 6) = 0;
                    q[15] = 0;
                    q[14] = 0;
                    q[13] = 0;
                    break;
                }
                q += 84;
            } while (n < 8);

            q = tt_player_shot_buf + 672;
            n = 8;
            do {
                n++;
                if (q[0] == 0) {
                    q[0] = 1;
                    *(s16 *)(q + 4) = 32;
                    *(s16 *)(q + 10) = 0;
                    *(s16 *)(q + 8) = 0;
                    *(s16 *)(q + 6) = 0;
                    q[15] = 0;
                    q[14] = 0;
                    q[13] = 0;
                    break;
                }
                q += 84;
            } while (n < 10);

            q = tt_player_shot_buf + 840;
            n = 10;
            do {
                n++;
                if (q[0] == 0) {
                    q[0] = 1;
                    *(s16 *)(q + 4) = 32;
                    *(s16 *)(q + 10) = 0;
                    *(s16 *)(q + 8) = 0;
                    *(s16 *)(q + 6) = 0;
                    q[15] = 0;
                    q[14] = 0;
                    q[13] = 0;
                    break;
                }
                q += 84;
            } while (n != 12);
        }
    }

    if (*(u16 *)(pad + 4) & 2) {
        pb = tt_rec_pool;
        pc = tt_recs_buf;
        flag = 0;
        n = 0;
        do {
            n++;
            if (pc[0] == 0 && pb[0] == 0 &&
                (st[92] == 0 || st[93] == 0)) {
                tt_cmd_once(1545);
                pc[0] = 1;
                *(s16 *)(pc + 4) = 32;
                pc[13] = 0;
                pc[14] = 0;
                pc[15] = 0;
                *(s16 *)(pc + 6) = 0;
                *(s16 *)(pc + 8) = 0;
                *(s16 *)(pc + 10) = 0;
                pb[0] = 1;
                pb[1] = 1;
                *(u16 *)(pb + 62) = 1;
                tog = st[96];
                *(s16 *)(pb + 4) = 32;
                pb[13] = 0;
                pb[14] = 0;
                pb[15] = 0;
                *(s16 *)(pb + 6) = 0;
                *(s16 *)(pb + 8) = 0;
                *(s16 *)(pb + 10) = 0;
                pb[61] = tog;
                flag = 1;
                break;
            }
            pc += 88;
            pb += 88;
        } while (n < 2);

        if (st[83] != 0) {
            bank = 176;
            do {
                pb = bank + tt_rec_pool;
                pc = bank + tt_recs_buf;
                n = 0;
                do {
                    n++;
                    if (pc[0] == 0 && pb[0] == 0 &&
                        (st[92] == 0 || st[93] == 0)) {
                        pc[0] = 1;
                        *(s16 *)(pc + 4) = 32;
                        *(s16 *)(pc + 10) = 0;
                        *(s16 *)(pc + 8) = 0;
                        *(s16 *)(pc + 6) = 0;
                        pc[15] = 0;
                        pc[14] = 0;
                        pc[13] = 0;
                        pb[0] = 1;
                        pb[1] = 1;
                        *(u16 *)(pb + 62) = 1;
                        *(s16 *)(pb + 4) = 32;
                        pb[13] = 0;
                        pb[14] = 0;
                        pb[15] = 0;
                        *(s16 *)(pb + 6) = 0;
                        *(s16 *)(pb + 8) = 0;
                        *(s16 *)(pb + 10) = 0;
                        pb[61] = st[96];
                        flag = 1;
                        break;
                    }
                    pc += 88;
                    pb += 88;
                } while (n < 2);
                bank += 176;
            } while (bank != 704);
        }

        if (flag != 0) {
            if (st[96] != 0) {
                st[93] = 1;
            } else {
                st[92] = 1;
            }
            st[96] = st[96] + 1;
            st[96] = st[96] & 1;
        }
    }
}
