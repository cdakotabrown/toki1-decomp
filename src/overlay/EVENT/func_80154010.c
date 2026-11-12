/* Address: 0x80154010, Size: 680 bytes (170 instructions) */
/* 3. `@CompileEnv: IDO52_SKIP_PASSES=eliminduct` removes uopt's rewrite of */
/* @CompileEnv: IDO52_SKIP_PASSES=eliminduct */
/* @PostProc: patch_stack_complete --old 64 --new 56 */
/* @PostProc: remap_stack_slots --map 62:50 */
/* @PostProc: patch_insn_reg_field 0x44:rt:v1:t8 0x48:rs:v1:t8 0x48:rt:t8:t9 0x4c:rs:v1:t8 0x4c:rt:t9:t0 0x50:rs:t9:t0 0x50:rt:t9:t0 0x54:rs:t8:t9 0x54:rt:t8:t9 0x58:rs:v1:t8 0x58:rt:t1:t2 0x5c:rs:t1:t2 0x5c:rt:t1:t2 0x60:rd:t0:t1 0x60:rs:t8:t9 0x60:rt:t9:t0 0x64:rd:t2:t3 0x64:rs:t0:t1 0x64:rt:t1:t2 0x68:rd:t3:t4 0x68:rt:t2:t3 0x6c:rd:t3:t4 0x6c:rs:t3:t4 0x6c:rt:t2:t3 0x74:rt:t3:t4 0x78:rt:t4:t5 0x7c:rs:t4:t5 0x7c:rt:t4:t5 0x80:rt:t6:t7 0x84:rd:t5:t6 0x84:rt:t4:t5 0x88:rd:t6:t7 0x88:rs:t6:t7 0x88:rt:t5:t6 0x8c:rt:t7:t8 0x90:rs:t7:t8 0x90:rt:t7:t8 0x94:rs:t6:t7 0x94:rt:t6:t7 0x9c:rt:v1:a0 0xa0:rd:t8:t9 0xa0:rs:t6:t7 0xa0:rt:t7:t8 0xa4:rs:v1:a0 0xa8:rt:t8:t9 0xac:rt:t9:t0 0xb0:rt:t9:t0 0xb4:rs:v1:a0 0xb8:rt:t0:t2 0xbc:rs:t0:t2 0xc4:rt:t1:t3 0xc8:rs:t1:t3 0xc8:rt:t1:t3 0xd0:rs:t1:t3 0xd4:rt:t2:t4 0xdc:rt:t2:t4 0xf0:rt:a0:t5 0xf4:rs:a0:t5 0xf4:rt:a0:t5 0xfc:rd:t3:t6 0xfc:rt:a0:t5 0x100:rs:t3:t6 0x100:rt:t4:t7 0x104:rt:a2:a0 0x108:rt:t4:t7 0x10c:rt:t5:t8 0x110:rs:t5:t8 0x110:rt:t5:t8 0x114:rs:a2:a0 0x114:rt:a2:a0 0x11c:rs:t5:t8 0x120:rt:t6:t9 0x124:rs:t6:t9 0x124:rt:t6:t9 0x128:rt:t7:t0 0x12c:rs:t6:t9 0x130:rt:t0:t4 0x134:rs:a2:a0 0x138:rt:t7:t0 0x13c:rt:t8:t2 0x140:rs:t8:t2 0x14c:rs:a0:t5 0x150:rt:t9:t3 0x154:rt:t9:t3 0x158:rs:t0:t4 0x158:rt:t0:t4 0x160:rs:t0:t4 0x164:rt:t3:t8 0x170:rt:t1:t6 0x174:rs:t1:t6 0x174:rt:t1:t6 0x178:rt:v1:a0 0x17c:rs:t1:t6 0x17c:rt:v1:a0 0x188:rt:t5:t0 0x18c:rt:t9:t3 0x190:rd:t2:t7 0x194:rd:v0:v1 0x194:rt:t2:t7 0x198:rs:v0:v1 0x198:rt:t3:t8 0x1a0:rs:t3:t8 0x1a0:rt:t4:t9 0x1a4:rs:v0:v1 0x1a4:rt:t4:t9 0x1a8:rs:t5:t0 0x1a8:rt:t5:t0 0x1b0:rs:t5:t0 0x1b0:rt:v1:a0 0x1b4:rd:t6:t1 0x1b8:rd:v0:v1 0x1b8:rt:t6:t1 0x1bc:rs:v0:v1 0x1bc:rt:t7:t2 0x1c4:rs:t7:t2 0x1c4:rt:t8:t5 0x1c8:rs:v0:v1 0x1c8:rt:t8:t5 0x1cc:rs:t9:t3 0x1cc:rt:t9:t3 0x1d4:rs:t9:t3 0x1d4:rt:v1:a0 0x1d8:rd:t0:t4 0x1dc:rd:v0:v1 0x1dc:rt:t0:t4 0x1e0:rs:v0:v1 0x1e0:rt:t1:t6 0x1e8:rs:t1:t6 0x1e8:rt:t2:t7 0x1f0:rs:v0:v1 0x1f0:rt:t2:t7 0x1fc:rt:t3:t8 0x204:rt:t3:t8 0x208:rt:t4:t9 0x210:rs:t4:t9 0x210:rt:t4:t9 0x220:rt:t4:t9 0x224:rt:t5:t0 0x228:rs:t5:t0 0x228:rt:t5:t0 0x230:rs:t5:t0 0x234:rt:t6:t2 0x238:rs:t6:t2 0x240:rt:t7:t5 0x244:rs:t7:t5 0x244:rt:t7:t5 0x24c:rs:t7:t5 0x264:rt:t8:t3 0x268:rs:t8:t3 0x268:rt:t8:t3 0x270:rt:t9:t4 0x274:rs:t9:t4 0x274:rt:t9:t4 0x278:rt:t8:t3 0x280:rt:t0:t6 0x284:rs:t0:t6 0x284:rt:t0:t6 0x28c:rt:t9:t4 0x2a4:rt:t0:t6 */
#include "types.h"

extern u8 D_800B0860[];
extern u8 D_800B0F45;
extern volatile u8 D_800B1746;
extern u8 D_800B1AF4;
extern u8 D_800B1AF9;
extern s32 D_800EECBC;
extern s32 ev_flag18;
extern s32 D_80124258;
extern s32 D_801242C4;
extern s32 D_80124330;
extern s16 D_80124354;
extern s16 D_80124358;
extern s16 D_801243A4[];
extern u8 D_80125310;
extern s32 D_8009471C;
extern s32 D_80094720;
extern s32 D_80094724;
extern s32 D_80094728;
extern s32 D_8009472C;

extern u32 func_8002328C(s32);
extern void func_8004A8EC(s32);
extern void func_8004C250(void);
extern void func_80033E88(void);
extern void func_80049B20(s32, s32, s32);

void func_80154010(void)
{
    s16 val;
    u32 ret;
    u32 once;
    s32 first;
    u32 n;
    s32 q;
    s32 c;
    s16 *p;
    u32 f3;

    ret = func_8002328C(D_800B1746);
    once = D_800B1AF9;
    first = (once < 1);
    D_800B1AF9 = once + 1;
    if (first) {
        n = ret & 0x7F;
        D_80124354 = (s16)(((n >= 2) + (n >= 3) + (n >= 4)) * 3);
    }
    val = (s16)(D_801243A4[D_800EECBC] + D_80124354);
    if (D_800B0F45 != 0) {
        val = 10;
    }
    if (D_800B0F45 != 0 && (ret & 0x7F) < 2 && D_800B1746 == 6) {
        val = 18;
    } else {
        q = ev_flag18;
        if (q >= 0) {
            c = D_800B1746;
            if (q == D_800B0860[1749 + c] && (f3 = D_800B0860[1765]) >= 0 &&
                    D_800B1AF4 == 80 && D_80125310 == 0) {
                val = 11;
                if (f3 != 0 && (ret & 0x7F) < 2 && c == 6) {
                    val = 19;
                }
                if (D_800B1AF9 == 1) {
                    func_8004A8EC(3);
                    p = (s16 *)&D_800B0860[D_800B1746 * 52];
                    p[211] -= 1;
                    p = (s16 *)&D_800B0860[D_800B1746 * 52];
                    p[213] -= 2;
                    p = (s16 *)&D_800B0860[D_800B1746 * 52];
                    p[215] += 5;
                    func_8004C250();
                    func_80033E88();
                }
            }
        }
    }
    D_8009471C = (s32)&val;
    D_80094720 = (s32)&D_80124358;
    ret = func_8002328C(D_800B1746);
    if (D_800B1746 == 1 && (ret & 0x7F) < 2 && D_80124358 == 3) {
        func_8004A8EC(1);
        func_80033E88();
    }
    D_80094724 = D_80124258;
    D_80094728 = D_801242C4;
    D_8009472C = D_80124330;
    func_80049B20(D_800EECBC, 1, 0);
}
