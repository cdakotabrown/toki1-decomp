/* Address: 0x8013C7D0 (cross-prologue head 0x8013C770 absorbed) */
/* Size: 280 bytes (70 instructions) */
/* @SymbolStart: 0x8013C770 */
/* @SymbolSize: 280 */
#include "types.h"

extern volatile s16 D_800B0B72;
extern volatile s16 D_800B0B76;
extern volatile s16 D_800B0B7A;
extern s32 D_801204DC;
extern s32 D_801204F8;
extern s32 D_80120514;
extern s32 D_80120678;
extern s32 D_8012067C;
extern s32 D_80120680;
extern s32 D_80121D78;
extern s32 D_80121D7C;
extern s32 D_80121D80;
extern s16 D_80121D84;
extern s32 D_80121D88;
extern s32 D_80121D8C;
extern s32 D_80121D90;
extern s32 D_80121D94;
extern s32 D_80121D98;
extern s32 D_80121D9C;
extern s32 D_80121DA0;

extern void func_8004C250(void);
extern void func_800FFD40(void);
extern void func_80012D64(s32, s32, s32, s32, s32);
extern void func_8004C46C(s32, s32, s32, s32, s32, s32);
extern void func_8004C6B0(s32, s32, s32, s16);
extern void func_80011DFC(void);

void func_8013C7D0(void) {
    D_80120678 = D_801204DC;
    D_8012067C = D_801204F8;
    D_80120680 = D_80120514;
    D_800B0B72 = D_800B0B72 + 1;
    D_800B0B76 = D_800B0B76 + 1;
    D_800B0B7A = D_800B0B7A - 20;

    func_8004C250();
    func_800FFD40();
    func_80012D64(D_80121D88, 17, 1, 2, 0);
    func_8004C46C(D_80121D8C, D_80121D90, D_80121D94, D_80121D98, D_80121D9C, D_80121DA0);
    func_8004C6B0(D_80121D7C, D_80121D80, D_80121D78, D_80121D84);
    func_80011DFC();
}
