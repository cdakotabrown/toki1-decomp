/* @PostProc: patch_insn_reg_field 0x10c:rt:t0:t1 0x110:rs:t0:t1 0x118:rt:t3:t4 0x11c:rt:t1:t2 0x120:rs:t1:t2 0x120:rt:t1:t2 0x128:rs:t1:t2 0x128:rt:t2:t3 0x12c:rt:t2:t3 0x130:rs:t3:t4 0x130:rt:t3:t4 0x138:rs:t3:t4 0x138:rt:t4:t5 0x140:rt:t4:t5 */
/* Address: 0x8014517C */
/* Size: 332 bytes (83 instructions) */
#include "types.h"

extern u8 D_800B1746;
extern u8 D_800E96EF;
extern u8 D_800E9733;
extern u8 D_800F19AB;
extern s16 D_80094714;
extern s16 D_80094718;
extern s16 D_800B0C0E;
extern s16 D_800B0C12;
extern s32 D_801205C8;
extern s32 D_801205E4;
extern s32 D_80120600;
extern s32 D_80120678;
extern s32 D_8012067C;
extern s32 D_80120680;
extern u8 D_8011FA68[];

extern void func_80012D2C(s32 a, s32 b);
extern void func_8001886C(s32 a);
extern void func_80010678(void);
extern void func_80018944(s32 a);
extern void func_80012D40(s32 a);
extern void func_80017E50(void);
extern void func_8001FD50(void);
extern void func_80036884(u8 *p);
extern void func_80045B14(void);
extern void func_800F6000(void);
extern void func_8003535C(void);
extern void func_8003580C(void);
extern void func_8004C360(void);
extern void func_8004BC20(s32 a);
extern void func_8004CDDC(void);
extern s32 func_8002328C(s32 a);
extern void func_8004C250(void);
extern void func_80011DFC(void);

void func_8014517C(void) {
    D_800B1746 = 10;
    func_80012D2C(1, 0);
    func_8001886C(1);
    func_80010678();
    func_80018944(0);
    func_80012D40(1);
    func_80017E50();
    func_8001FD50();
    func_80036884(D_8011FA68);
    D_800E96EF = 0;
    D_800E9733 = 0;
    D_80094714 = 0;
    D_80094718 = 0;
    func_80045B14();
    func_800F6000();
    func_8003535C();
    func_8003580C();
    func_8004C360();
    D_800F19AB = 0;
    func_8004BC20(D_800B1746);
    func_8004CDDC();
    func_800F6000();
    D_80120678 = D_801205C8;
    D_8012067C = D_801205E4;
    D_80120680 = D_80120600;
    if ((u32)(func_8002328C(10) & 0x7f) < 2) {
        D_800B0C0E = (s16)(D_800B0C0E + 1);
    }
    D_800B0C12 = (s16)(D_800B0C12 + 2);
    func_8004C250();
    func_80011DFC();
}
