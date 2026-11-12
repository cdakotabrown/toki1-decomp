/* @PostProc: swap_instructions --offset1 0x60 --offset2 0x64 */
/* @PostProc: patch_insn_reg_field 0x60:rt:v1:a0 0x64:rt:a0:t1 0x68:rs:v1:a0 0x68:rt:v1:a0 0x6c:rs:a0:t1 0x6c:rt:t1:t2 0x70:rs:a0:t1 0x70:rt:t2:t3 0x74:rs:t2:t3 0x74:rt:t2:t3 0x78:rs:t1:t2 0x78:rt:t1:t2 0x7c:rs:v1:a0 0x7c:rt:t4:t5 0x80:rs:t4:t5 0x80:rt:t5:t6 0x84:rd:t3:t4 0x84:rs:t1:t2 0x84:rt:t2:t3 0x88:rs:t3:t4 0x88:rt:t5:t6 0x90:rs:v1:a0 0x90:rt:t8:t0 0x94:rs:t8:t0 0x94:rt:t8:t0 0x98:rd:t0:t1 0x98:rt:t8:t0 0x9c:rd:t0:t1 0x9c:rs:t0:t1 0x9c:rt:t8:t0 0xa4:rt:t2:t3 0xa8:rs:t2:t3 0xa8:rt:t2:t3 0xac:rt:t4:t5 0xb0:rd:t6:t7 0xbc:rs:v1:a0 0xc0:rd:t7:t9 0xc4:rd:t9:t8 0xc4:rs:t6:t7 0xc4:rt:t7:t9 0xc8:rd:t1:t2 0xc8:rs:t9:t8 0xc8:rt:t0:t1 0xcc:rt:t1:t2 0x124:rt:t2:t3 0x12c:rs:t4:t5 0x12c:rt:t4:t5 0x130:rt:t3:t4 0x134:rs:t4:t5 0x144:rt:t3:t4 */
/* Address: 0x8014C234 */
/* Size: 364 bytes (91 instructions) */
#include "types.h"

extern u8 D_801252E8;
extern u8 D_8012531C;
extern u8 D_801252E4;
extern u8 D_80125328;
extern s16 D_80094730;
extern s16 D_80094734;
extern s16 D_80094868;
extern u8 D_800B1746;
extern u8 D_800B0898;
extern u8 D_800B0897;
extern s16 D_80124344;
extern s16 D_80124348;
extern s16 D_8012434C;
extern s16 D_80124350;
extern s16 D_80124358;
extern s16 D_80124360;
extern s16 D_80124364;
extern s16 D_80124368;
extern s16 D_80124370;
extern s16 D_80124378;
extern s16 D_8012437C;
extern s16 D_80124380;

extern void func_80017E50(void);
extern s32 func_8002328C(s32);
extern void func_80045B14(void);
extern void func_80011DFC(void);

void func_8014C234(void) {
    s32 v0;
    s32 t1;
    s32 a0;

    func_80017E50();
    D_80094730 = D_80094730 + D_8012531C * D_801252E8;
    D_80094734 = 0;
    v0 = func_8002328C(D_800B1746);
    t1 = v0 & 0x7F;
    a0 = D_801252E4;
    D_80124344 = (((u32)t1 >= 2) + ((u32)t1 >= 3)) * ((a0 == 0) + 1)
        + a0 * 6 - (a0 == 2) * 3;
    D_80124348 = 0;
    D_80124358 = 0;
    D_80124360 = 0;
    D_8012434C = 0;
    D_80124350 = 0;
    D_8012437C = 0;
    D_80124380 = 0;
    D_80124370 = 0;
    D_80124378 = 0;
    D_80124364 = 0;
    D_80124368 = 0;
    if (D_800B0898 == 6 && (u32)D_800B0897 < 8) {
        D_80125328 = 1;
    } else {
        D_80125328 = 0;
    }
    func_80017E50();
    func_80045B14();
    D_80094868 = 0;
    func_80011DFC();
}
