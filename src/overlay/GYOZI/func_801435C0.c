/* @SymbolStart: 0x801435BC */
/* @SymbolSize: 224 */
/* @PostProc: patch_insn_reg_field 0x70:rt:v1:t9 0x74:rs:v1:t9 0x74:rt:v1:t9 0x78:rt:a1:a0 0x7c:rs:v1:t9 0x7c:rt:a1:a0 0x80:rt:a0:v1 0x84:rs:a0:v1 0x84:rt:a0:v1 0x88:rt:v1:t3 0x8c:rd:t9:t0 0x90:rs:a0:v1 0x90:rt:t9:t0 0x94:rt:t0:t1 0x9c:rs:t0:t1 0x9c:rt:t1:t2 0xa0:rt:t1:t2 0xa4:rs:v1:t3 0xa4:rt:v1:t3 0xac:rs:v1:t3 0xac:rt:a1:a0 0xb0:rd:t2:t4 0xb4:rs:a0:v1 0xb4:rt:t2:t4 0xb8:rt:t3:t5 0xc0:rs:t3:t5 0xc0:rt:t4:t6 0xc8:rt:t4:t6 */
/* func_801435C0 - GYOZI overlay, Address: 0x801435C0, Size: 224 bytes (56 */
#include "types.h"

extern void func_8008F618(s32 a0);
extern void func_8013ED84(void);
extern void func_8004DE1C(void);
extern void func_8008FB00(void);
extern void func_800BCE10(s32 a0, s32 a1);

extern s16 D_800D9250;
extern s16 D_800D9254;
extern volatile u8 D_800F62CF;
extern s32 D_80148358;
extern s32 D_80148490;
extern s32 D_801485C8;
extern s32 D_80148628;
extern s32 D_8014862C;
extern s32 D_80148630;
extern u8  D_800F53A0[];
extern u8  D_800D92A0;
extern u8  D_80145DEC;

void func_801435C0(void)
{
    u32 idx;
    u8 *v0;

    D_800D9250 = 0;
    D_800D9254 = 0;
    func_8008F618((s32)D_800F62CF);
    func_8013ED84();

    D_80148628 = D_80148358;
    D_8014862C = D_80148490;
    D_80148630 = D_801485C8;
    func_8004DE1C();

    idx = (u32)D_800F62CF;
    v0  = &D_800F53A0[idx * 56];
    *(s16 *)(v0 + 0x1AE) = (s16)(*(s16 *)(v0 + 0x1AE) + 1);

    idx = (u32)D_800F62CF;
    v0  = &D_800F53A0[idx * 56];
    *(s16 *)(v0 + 0x1B2) = (s16)(*(s16 *)(v0 + 0x1B2) + 2);
    func_8008FB00();

    func_800BCE10((s32)&D_800D92A0, (s32)&D_80145DEC);
}
