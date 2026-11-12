/* Address: 0x80142614 (cross-prologue head 0x801425B4 absorbed) */
/* Size: 280 bytes (70 instructions) */
/* @SymbolStart: 0x801425B4 */
/* @SymbolSize: 280 */
#include "types.h"

extern volatile s16 D_800B0BDA;
extern volatile s16 D_800B0BDE;
extern volatile s16 D_800B0BE2;
extern s32 D_80120584;
extern s32 D_801205A0;
extern s32 D_801205BC;
extern s32 D_80120678;
extern s32 D_8012067C;
extern s32 D_80120680;
extern s32 D_80123020;
extern s32 D_80123024;
extern s32 D_80123028;
extern s16 D_8012302C;
extern s32 D_80123030;
extern s32 D_80123034;
extern s32 D_80123038;
extern s32 D_8012303C;
extern s32 D_80123040;
extern s32 D_80123044;
extern s32 D_80123048;

extern void func_8004C250(void);
extern void func_801053B0(void);
extern void func_80012D64(s32, s32, s32, s32, s32);
extern void func_8004C46C(s32, s32, s32, s32, s32, s32);
extern void func_8004C6B0(s32, s32, s32, s16);
extern void func_80011DFC(void);

void func_80142614(void) {
    D_80120678 = D_80120584;
    D_8012067C = D_801205A0;
    D_80120680 = D_801205BC;
    D_800B0BDA = D_800B0BDA + 3;
    D_800B0BDE = D_800B0BDE + 2;
    D_800B0BE2 = D_800B0BE2 - 20;

    func_8004C250();
    func_801053B0();
    func_80012D64(D_80123030, 17, 1, 2, 0);
    func_8004C46C(D_80123034, D_80123038, D_8012303C, D_80123040, D_80123044, D_80123048);
    func_8004C6B0(D_80123024, D_80123028, D_80123020, D_8012302C);
    func_80011DFC();
}
