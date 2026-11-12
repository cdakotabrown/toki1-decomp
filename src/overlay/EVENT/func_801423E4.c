/* Address: 0x801423E4 (cross-prologue head 0x80142384 absorbed) */
/* Size: 280 bytes (70 instructions) */
/* @SymbolStart: 0x80142384 */
/* @SymbolSize: 280 */
#include "types.h"

extern volatile s16 D_800B0BDA;
extern volatile s16 D_800B0BDE;
extern volatile s16 D_800B0BE2;
extern s32 D_8012057C;
extern s32 D_80120598;
extern s32 D_801205B4;
extern s32 D_80120678;
extern s32 D_8012067C;
extern s32 D_80120680;
extern s32 D_80122FC8;
extern s32 D_80122FCC;
extern s32 D_80122FD0;
extern s16 D_80122FD4;
extern s32 D_80122FD8;
extern s32 D_80122FDC;
extern s32 D_80122FE0;
extern s32 D_80122FE4;
extern s32 D_80122FE8;
extern s32 D_80122FEC;
extern s32 D_80122FF0;

extern void func_8004C250(void);
extern void func_80105250(void);
extern void func_80012D64(s32, s32, s32, s32, s32);
extern void func_8004C46C(s32, s32, s32, s32, s32, s32);
extern void func_8004C6B0(s32, s32, s32, s16);
extern void func_80011DFC(void);

void func_801423E4(void) {
    D_80120678 = D_8012057C;
    D_8012067C = D_80120598;
    D_80120680 = D_801205B4;
    D_800B0BDA = D_800B0BDA + 2;
    D_800B0BDE = D_800B0BDE + 1;
    D_800B0BE2 = D_800B0BE2 - 20;

    func_8004C250();
    func_80105250();
    func_80012D64(D_80122FD8, 17, 1, 2, 0);
    func_8004C46C(D_80122FDC, D_80122FE0, D_80122FE4, D_80122FE8, D_80122FEC, D_80122FF0);
    func_8004C6B0(D_80122FCC, D_80122FD0, D_80122FC8, D_80122FD4);
    func_80011DFC();
}
