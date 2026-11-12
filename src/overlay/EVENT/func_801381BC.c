/* @PostProc: swap_instructions --offset1 0x118 --offset2 0x120 */
/* @PostProc: swap_instructions --offset1 0x11C --offset2 0x124 */
/* @PostProc: swap_instructions --offset1 0x120 --offset2 0x128 */
/* @PostProc: swap_instructions --offset1 0x124 --offset2 0x128 */
/* Address: 0x801381BC */
/* Size: 324 bytes (81 instructions) */
#include "types.h"

extern u8 D_800B1746;
extern u8 D_800E96EF;
extern u8 D_800E9733;
extern s16 D_80094714;
extern s16 D_80094718;
extern u8 D_80094784;
extern u8 D_8011F69C;
extern s32 D_8012036C;
extern s32 D_80120384;
extern s32 D_8012039C;
extern s32 D_80120678;
extern s32 D_8012067C;
extern s32 D_80120680;
extern u8 D_801212B4;
extern s16 D_800B0A6E;
extern s16 D_800B0A72;

extern void func_80012D2C(s32, s32);
extern void func_8001886C(s32);
extern void func_80010678(void);
extern void func_80018944(s32);
extern void func_80012D40(s32);
extern void func_80017E50(void);
extern void func_8001FD50(void);
extern void func_80045B14(void);
extern void func_800F6000(void);
extern void func_8003535C(void);
extern void func_8003580C(void);
extern void func_8004C360(void);
extern void func_8004BC20(s32);
extern void func_8004CDDC(void);
extern void func_80078970(s32, s32);
extern void func_8004C250(void);
extern void func_80011DFC(void);

void func_801381BC(void) {
    D_800B1746 = 2;
    func_80012D2C(1, 0);
    func_8001886C(1);
    func_80010678();
    func_80018944(0);
    func_80012D40(1);
    func_80017E50();
    func_8001FD50();
    D_800E96EF = 0;
    D_800E9733 = 0;
    D_80094714 = 0;
    D_80094718 = 0;
    func_80045B14();
    func_800F6000();
    func_8003535C();
    func_8003580C();
    func_8004C360();
    func_8004BC20((s32)D_800B1746);
    func_8004CDDC();
    func_800F6000();
    D_80120678 = D_8012036C;
    D_8012067C = D_80120384;
    D_80120680 = D_8012039C;
    func_80078970((s32)&D_80094784, (s32)&D_8011F69C);
    D_801212B4 = 0;
    D_800B0A6E = D_800B0A6E + 1;
    D_800B0A72 = D_800B0A72 + 2;
    func_8004C250();
    func_80011DFC();
}
