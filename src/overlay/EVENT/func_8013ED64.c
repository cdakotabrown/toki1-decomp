/* func_8013ED64 -- EVENT overlay, Address: 0x8013ED64 */
/* @SymbolStart: 0x8013ED40 */
/* @SymbolSize: 276 */
#include "types.h"

extern s32 D_8012044C;
extern s32 D_80120468;
extern s32 D_80120484;
extern s32 D_80120678;
extern s32 D_8012067C;
extern s32 D_80120680;
extern volatile s16 D_800B0B0A;
extern volatile s16 D_800B0B0E;
extern volatile s16 D_800B0B12;
extern s32 D_801226AC;
extern s32 D_801226B0;
extern s32 D_801226B4;
extern s16 D_801226B8;
extern s32 D_801226BC;
extern s32 D_801226C0;
extern s32 D_801226C4;
extern s32 D_801226C8;
extern s32 D_801226CC;
extern s32 D_801226D0;
extern s32 D_801226D4;

extern void func_80102560(void);
extern void func_80012D64(s32, s32, s32, s32, s32);
extern void func_8004C46C(s32, s32, s32, s32, s32, s32);
extern void func_8004C6B0(s32, s32, s32, s16);
extern void func_8004C250(void);
extern void func_80011DFC(void);

void func_8013ED64(void) {
    D_80120678 = D_8012044C;
    D_8012067C = D_80120468;
    D_80120680 = D_80120484;
    func_80102560();
    func_80012D64(D_801226BC, 17, 1, 2, 0);
    func_8004C46C(D_801226C0, D_801226C4, D_801226C8, D_801226CC,
                  D_801226D0, D_801226D4);
    func_8004C6B0(D_801226B0, D_801226B4, D_801226AC, D_801226B8);
    D_800B0B0A = (s16)(D_800B0B0A + 2);
    D_800B0B0E = (s16)(D_800B0B0E + 1);
    D_800B0B12 = (s16)(D_800B0B12 - 10);
    func_8004C250();
    func_80011DFC();
}
