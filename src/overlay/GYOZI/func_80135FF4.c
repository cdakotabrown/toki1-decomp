/* @SymbolStart: 0x80135FEC */
/* @SymbolSize: 440 */
/* @PostProc: patch_insn_reg_field 0x38:rt:a0:t6 0x44:rs:a0:t6 0x44:rt:a0:t6 0x48:rt:t0:a0 0x4c:rs:a0:t6 0x4c:rt:t0:a0 0x50:rt:a3:v1 0x54:rs:a3:v1 0x54:rt:a3:v1 0x58:rt:a1:t0 0x5c:rt:t1:t4 0x60:rd:t6:t7 0x64:rs:a3:v1 0x64:rt:t6:t7 0x68:rt:t7:t8 0x70:rs:t7:t8 0x70:rt:t8:t9 0x74:rt:t8:t9 0x78:rs:a1:t0 0x78:rt:a1:t0 0x80:rs:a1:t0 0x80:rt:t0:a0 0x84:rd:t9:t1 0x88:rd:v1:v0 0x88:rs:a3:v1 0x88:rt:t9:t1 0x8c:rs:v1:v0 0x98:rs:v1:v0 0x9c:rs:t1:t4 0x9c:rt:t1:t4 0xa4:rs:t1:t4 0xa4:rt:t0:a0 0xa8:rd:t4:t5 0xac:rd:a2:v0 0xac:rs:a3:v1 0xac:rt:t4:t5 0xb0:rs:a2:v0 0xb0:rt:t5:t6 0xb8:rs:t5:t6 0xb8:rt:t6:t7 0xc0:rs:a2:v0 0xc0:rt:t6:t7 0xcc:rt:a0:t8 0xd0:rs:a0:t8 0xd0:rt:a0:t8 0xd4:rt:t0:a0 0xd8:rs:a0:t8 0xd8:rt:t0:a0 0xdc:rt:a3:v1 0xe0:rs:a3:v1 0xe0:rt:a3:v1 0xe4:rt:a1:t2 0xe8:rt:t1:t6 0xec:rd:t7:t9 0xf0:rs:a3:v1 0xf0:rt:t7:t9 0xf4:rt:t8:t0 0xfc:rs:t8:t0 0xfc:rt:t9:t1 0x100:rt:t9:t1 0x104:rs:a1:t2 0x104:rt:a1:t2 0x10c:rs:a1:t2 0x10c:rt:t0:a0 0x110:rd:t2:t3 0x114:rd:v1:v0 0x114:rs:a3:v1 0x114:rt:t2:t3 0x118:rs:v1:v0 0x118:rt:t3:t4 0x120:rs:t3:t4 0x120:rt:t4:t5 0x124:rs:v1:v0 0x124:rt:t4:t5 0x128:rs:t1:t6 0x128:rt:t1:t6 0x130:rs:t1:t6 0x130:rt:t0:a0 0x134:rd:t5:t7 0x138:rd:a2:v0 0x138:rs:a3:v1 0x138:rt:t5:t7 0x13c:rs:a2:v0 0x13c:rt:t6:t8 0x144:rs:t6:t8 0x144:rt:t7:t9 0x14c:rs:a2:v0 0x14c:rt:t7:t9 0x150:rt:a0:t0 0x154:rs:a0:t0 0x154:rt:a0:t0 0x158:rt:t0:a0 0x15c:rs:a0:t0 0x15c:rt:t0:a0 0x160:rt:a3:v1 0x164:rs:a3:v1 0x164:rt:a3:v1 0x168:rt:a1:t4 0x16c:rd:t8:t1 0x170:rs:a3:v1 0x170:rt:t8:t1 0x174:rt:t9:t2 0x17c:rs:t9:t2 0x17c:rt:t2:t3 0x180:rt:t2:t3 0x184:rs:a1:t4 0x184:rt:a1:t4 0x18c:rs:a1:t4 0x18c:rt:t0:a0 0x190:rd:t3:t5 0x194:rd:v1:v0 0x194:rs:a3:v1 0x194:rt:t3:t5 0x198:rs:v1:v0 0x198:rt:t4:t6 0x1a0:rs:t4:t6 0x1a0:rt:t5:t7 0x1a4:rs:v1:v0 0x1a4:rt:t5:t7 */
/* func_80135FF4 -- GYOZI overlay, Address: 0x80135FF4, Size: 432 bytes */
#include "types.h"

extern s32 D_8012E66C;
extern u8 D_800F53A0[];

#define CUR_CHARA 3887

extern void func_8008E408(s32);
extern void func_80072FE8(void);
extern void func_8008FB00(void);
extern void func_8004DE1C(void);

void func_80135FF4(void)
{
    s32 sel;
    u32 i0;
    u32 i1;
    u32 i2;

    func_8008E408(D_8012E66C + 1);
    func_80072FE8();

    sel = D_8012E66C;
    if (sel == 0) {
        i0 = D_800F53A0[CUR_CHARA];
        *(s16 *)(D_800F53A0 + i0 * 56 + 430) += 3;
        i1 = D_800F53A0[CUR_CHARA];
        *(s16 *)(D_800F53A0 + i1 * 56 + 434) += 2;
        i2 = D_800F53A0[CUR_CHARA];
        *(s16 *)(D_800F53A0 + i2 * 56 + 438) += -15;
    } else if (sel == 1) {
        i0 = D_800F53A0[CUR_CHARA];
        *(s16 *)(D_800F53A0 + i0 * 56 + 430) += 2;
        i1 = D_800F53A0[CUR_CHARA];
        *(s16 *)(D_800F53A0 + i1 * 56 + 434) += 1;
        i2 = D_800F53A0[CUR_CHARA];
        *(s16 *)(D_800F53A0 + i2 * 56 + 438) += -10;
    } else {
        i0 = D_800F53A0[CUR_CHARA];
        *(s16 *)(D_800F53A0 + i0 * 56 + 434) += 1;
        i1 = D_800F53A0[CUR_CHARA];
        *(s16 *)(D_800F53A0 + i1 * 56 + 438) += 10;
    }

    func_8008FB00();
    func_8004DE1C();
}
