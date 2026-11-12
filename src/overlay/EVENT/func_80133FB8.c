/* Address: 0x80133F44 (cross-prologue head absorbed) .. 0x80133FB8 */
/* Size: 300 bytes (75 instructions) */
/* @SymbolStart: 0x80133F44 */
/* @SymbolSize: 300 */
#include "types.h"

extern s16 D_801207AC;
extern s16 D_801207B0;
extern s32 D_80120534;
extern s32 D_8012054C;
extern s32 D_80120564;
extern s32 D_80120678;
extern s32 D_8012067C;
extern s32 D_80120680;
extern volatile s16 D_800B0BA6;
extern volatile s16 D_800B0BAA;
extern volatile s16 D_800B0BAE;

extern s32 D_80120714;
extern s32 D_80120718;
extern s32 D_8012071C;
extern s16 D_80120720;
extern s32 D_80120724;
extern s32 D_80120728;
extern s32 D_8012072C;
extern s32 D_80120730;
extern s32 D_80120734;
extern s32 D_80120738;
extern s32 D_8012073C;

extern void func_8004C250(void);
extern void func_800F7430(void);
extern void func_80012D64(s32, s32, s32, s32, s32);
extern void func_8004C46C(s32, s32, s32, s32, s32, s32);
extern void func_8004C6B0(s32, s32, s32, s16);
extern void func_80011DFC(void);

void func_80133FB8(void) {
    D_801207AC = 0;
    D_801207B0 = 3;
    D_80120678 = D_80120534;
    D_8012067C = D_8012054C;
    D_80120680 = D_80120564;
    D_800B0BA6 = D_800B0BA6 + 1;
    D_800B0BAA = D_800B0BAA + 1;
    D_800B0BAE = D_800B0BAE - 20;

    func_8004C250();
    func_800F7430();
    func_80012D64(D_80120724, 17, 1, 2, 0);
    func_8004C46C(D_80120728, D_8012072C, D_80120730, D_80120734, D_80120738, D_8012073C);
    func_8004C6B0(D_80120718, D_8012071C, D_80120714, D_80120720);
    func_80011DFC();
}
