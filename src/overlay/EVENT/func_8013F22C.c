/* func_8013F22C -- EVENT overlay, Address: 0x8013F22C */
/* @SymbolStart: 0x8013F208 */
/* @SymbolSize: 256 */
#include "types.h"

extern s32 D_80120458;
extern s32 D_80120474;
extern s32 D_80120490;
extern s32 D_80120678;
extern s32 D_8012067C;
extern s32 D_80120680;
extern volatile s16 D_800B0B0A;
extern volatile s16 D_800B0B0E;
extern volatile s16 D_800B0B12;
extern s32 D_80122730;
extern s32 D_80122734;
extern s32 D_80122738;
extern s32 D_8012273C;
extern s32 D_80122740;
extern s32 D_80122744;
extern s32 D_80122748;

extern void func_80102770(void);
extern void func_80012D64(s32, s32, s32, s32, s32);
extern void func_8004C46C(s32, s32, s32, s32, s32, s32);
extern void func_8004C6B0(s32, s32, s32, s32);
extern void func_8004C250(void);
extern void func_80011DFC(void);

void func_8013F22C(void) {
    D_80120678 = D_80120458;
    D_8012067C = D_80120474;
    D_80120680 = D_80120490;
    func_80102770();
    func_80012D64(D_80122730, 17, 1, 2, 0);
    func_8004C46C(D_80122734, D_80122738, D_8012273C, D_80122740,
                  D_80122744, D_80122748);
    func_8004C6B0(0, 0, 0, 0);
    D_800B0B0A = (s16)(D_800B0B0A + 1);
    D_800B0B0E = (s16)(D_800B0B0E + 1);
    D_800B0B12 = (s16)(D_800B0B12 - 20);
    func_8004C250();
    func_80011DFC();
}
