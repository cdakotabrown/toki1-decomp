/* @CompileEnv: IDO52_FORCE_WEB_COLOR=14:t2,9:a1 */
/* @SymbolStart: 0x80155218 */
/* @SymbolSize: 380 */
/* @RodataBase: 0x8015DCB8 */
/* @PostProc: patch_insn_reg_field 0x54:rd:t2:t0 0x58:rs:t2:t0 0xbc:rt:t4:t1 0xc0:rt:t3:t0 0xc4:rt:t3:t0 0xc8:rs:t4:t1 0xc8:rt:t4:t1 0xd0:rt:t4:t1 0xd4:rt:t5:t2 0xd8:rt:t7:t4 0xdc:rs:t5:t2 0xdc:rt:t6:t3 0xe0:rt:t6:t3 0xe4:rs:t7:t4 0xe4:rt:t7:t4 0xec:rt:t7:t4 0xf0:rt:t8:t5 0xf4:rt:t3:t7 0xf8:rs:t8:t5 0xf8:rt:t9:t6 0xfc:rt:t9:t6 0x100:rs:t3:t7 0x100:rt:t3:t7 0x108:rt:t3:t7 0x10c:rt:t4:t8 0x110:rt:t6:t0 0x114:rs:t4:t8 0x114:rt:t5:t9 0x118:rt:t5:t9 0x11c:rs:t6:t0 0x11c:rt:t6:t0 0x124:rt:t6:t0 0x128:rt:t7:t1 0x12c:rt:t9:t3 0x130:rs:t7:t1 0x130:rt:t8:t2 0x134:rt:t8:t2 0x138:rs:t9:t3 0x138:rt:t9:t3 0x140:rt:t9:t3 0x144:rt:t3:t4 0x148:rt:t5:t6 0x14c:rs:t3:t4 0x14c:rt:t4:t5 0x150:rt:t4:t5 0x154:rs:t5:t6 0x154:rt:t5:t6 0x15c:rt:t5:t6 0x160:rt:t6:t7 0x168:rs:t6:t7 */
#include "types.h"

typedef struct {
    u8  pad0[100];
    s16 v[3];
    u16 sel;
    s16 w[3];
    u8  pad1[6];
    s16 cnt;
    u8  pad2[2];
    s16 lvl;
    u8  pad3[10];
} Rec;

extern Rec *taco_actor_rec;
extern void taco_actor_exit(void);

#define REC(i) ((Rec *)((i) * 136 + (u8 *)taco_actor_rec))

void taco_actor_step1(s32 idx)
{
    s32 a1;

    REC(idx)->cnt += 256;
    a1 = REC(idx)->lvl;
    REC(idx)->w[2] = (a1 << 9) + 512;
    taco_actor_rec[idx].w[1] = (a1 << 9) + 512;
    taco_actor_rec[idx].w[0] = (a1 << 9) + 512;
    switch (REC(idx)->sel) {
    case 0:
        REC(idx)->v[0] += 50;
        break;
    case 1:
        REC(idx)->v[1] += 50;
        break;
    case 2:
        REC(idx)->v[2] += 50;
        break;
    case 3:
        REC(idx)->v[0] += 99;
        break;
    case 4:
        REC(idx)->v[1] += 99;
        break;
    case 5:
        REC(idx)->v[2] += 99;
        break;
    }
    if (REC(idx)->cnt >= 4097) {
        taco_actor_exit();
    }
}
