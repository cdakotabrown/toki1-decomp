/* func_801385F4 -- GYOZI overlay, Address: 0x801385F4, Size: 376 bytes */
/* @PostProc: patch_insn_reg_field 0x6c:rt:v1:t6 0x70:rs:v1:t6 0x70:rt:v1:t6 0x74:rt:t0:v1 0x78:rs:t0:v1 0x78:rt:t0:v1 0x7c:rd:v0:t7 0x7c:rt:v1:t6 0x80:rd:t6:t8 0x80:rs:t0:v1 0x80:rt:v0:t7 0x84:rs:t6:t8 0x84:rt:t7:t9 0x8c:rt:t8:t1 0x90:rs:t8:t1 0x90:rt:t8:t1 0x94:rt:t7:t9 0x9c:rd:t9:t2 0x9c:rs:t0:v1 0x9c:rt:v0:t7 0xa0:rt:t8:t1 0xa4:rs:t9:t2 0xa4:rt:t1:t0 0xac:rs:t1:t0 0xb8:rs:t1:t0 0xd0:rt:t1:t0 0xd4:rt:t2:t3 0xd8:rt:t3:t4 0xdc:rd:t3:t4 0xdc:rs:t3:t4 0xe0:rd:t2:t3 0xe0:rs:t2:t3 0xe8:rs:t2:t3 0xe8:rt:t2:t3 0xec:rs:t3:t4 0xec:rt:t3:t4 0xf8:rt:t2:t3 0x100:rt:t3:t4 0x104:rt:t5:t6 0x108:rs:t5:t6 0x108:rt:t5:t6 0x10c:rt:t0:v1 0x110:rs:t0:v1 0x110:rt:t0:v1 0x114:rd:t6:t8 0x114:rt:t5:t6 0x118:rd:t7:t9 0x118:rs:t0:v1 0x118:rt:t6:t8 0x11c:rs:t7:t9 0x11c:rt:t8:t1 0x124:rs:t8:t1 0x128:rt:t4:t5 0x12c:rs:t4:t5 0x12c:rt:t4:t5 0x130:rt:t7:t9 0x134:rt:t6:t8 0x138:rd:t9:t7 0x13c:rs:t0:v1 0x13c:rt:t9:t7 0x148:rs:t4:t5 0x14c:rs:t7:t9 0x14c:rt:t7:t9 0x154:rd:t8:t1 0x154:rt:t7:t9 0x158:rd:t9:t7 0x158:rs:t0:v1 0x158:rt:t8:t1 0x15c:rs:t9:t7 0x15c:rt:t3:t4 0x164:rs:t3:t4 0x168:rd:t4:t3 0x16c:rd:t2:t5 0x16c:rs:t0:v1 0x16c:rt:t4:t3 0x174:rs:t2:t5 0x174:rt:t6:t8 */
#include "types.h"

typedef struct W4 { s32 w; } W4;

extern void func_8004EDE0(s32, s32);
extern void func_80054864(s32);
extern void func_8004C670(void);
extern void func_8005493C(s32);
extern void func_8004EDF4(s32);
extern void func_80053EFC(void);
extern void func_8005ABC0(void);
extern void func_800744A0(void);
extern void func_80074950(void);
extern void func_8009068C(void);
extern void func_800504CC(s32, s32, s32, s32, s32, s32);
extern void func_8004DDD8(void);

extern s32 D_800C3280[];
extern s32 D_800C32C0[];
extern s32 D_800C3300[];
extern  u8 D_800F53A0[][56];
extern volatile u8 D_800F5AAC;
extern u8 D_800F62CF;
extern s32 D_8012E660;
extern W4 D_801474B4;
extern u8 D_801474B8;

void func_801385F4(void)
{
    u32 idx;
    u32 sel;
    s32 arg1;
    s32 arg2;

    func_8004EDE0(1, 0);
    func_80054864(1);
    func_8004C670();
    func_8005493C(0);
    func_8004EDF4(1);
    func_80053EFC();
    func_8005ABC0();
    func_800744A0();
    func_80074950();
    func_8009068C();

    idx = D_800F5AAC;
    D_800F62CF = D_800F53A0[0][idx * 4 + 1677];
    D_801474B8 = D_8012E660;
    sel = *((volatile u8 *)D_800F53A0 + idx * 4 + 1677);
    if (sel == 2 || sel == 6) {
        arg1 = 0xB33B;
        arg2 = 0xB322;
    } else {
        arg1 = 0xB320;
        arg2 = 0xB2FD;
    }
    func_800504CC(1, arg1, arg2, D_800C3280[sel], D_800C32C0[sel],
                  D_800C3300[sel]);

    D_801474B4 =
        *(W4 *)&D_800F53A0[D_800F53A0[0][D_800F5AAC * 4 + 1677]][432];
    *(s16 *)&D_800F53A0[D_800F53A0[0][D_800F5AAC * 4 + 1677]][434] = 50;
    func_8004DDD8();
}
