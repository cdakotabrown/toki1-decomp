/* Address: 0x8013C6BC (true start 0x8013C614: the head global copies and the */
/* Size: 0x8013C770 - 0x8013C614 = 348 bytes (87 instructions) */
/* @SymbolStart: 0x8013C614 */
/* @SymbolSize: 348 */
#include "types.h"

extern s32 D_801204D8;
extern s32 D_801204F4;
extern s32 D_80120510;
extern s32 D_80120678;
extern s32 D_8012067C;
extern s32 D_80120680;
extern s16 D_8012438C;
extern s16 D_8012438E;
extern s16 D_80124390;
extern s16 D_80124394;
extern s16 D_80124396;
extern s16 D_80124398;
extern s16 D_8012439C;
extern s16 D_8012439E;
extern s16 D_801243A0;
extern s16 D_801243A4;
extern s16 D_801243A6;
extern s16 D_801243A8;
extern s32 D_80121D4C;
extern s32 D_80121D50;
extern s32 D_80121D54;
extern s16 D_80121D58;
extern s32 D_80121D5C;
extern s32 D_80121D60;
extern s32 D_80121D64;
extern s32 D_80121D68;
extern s32 D_80121D6C;
extern s32 D_80121D70;
extern s32 D_80121D74;

extern void func_800FFC90(void);
extern void func_80012D64(s32 a, s32 b, s32 c, s32 d, s32 e);
extern void func_8004C46C(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
extern void func_8004C6B0(s32 a, s32 b, s32 c, s32 d);
extern void func_80011DFC(void);

void func_8013C6BC(void) {
    D_80120678 = D_801204D8;
    D_8012067C = D_801204F4;
    D_80120680 = D_80120510;
    D_8012438C = 1;
    D_8012438E = 2;
    D_80124390 = 4;
    D_80124394 = 1;
    D_80124396 = 2;
    D_80124398 = 3;
    D_8012439C = 0;
    D_8012439E = 2;
    D_801243A0 = 4;
    D_801243A4 = 0;
    D_801243A6 = 2;
    D_801243A8 = 4;
    func_800FFC90();
    func_80012D64(D_80121D5C, 17, 1, 2, 0);
    func_8004C46C(D_80121D60, D_80121D64, D_80121D68, D_80121D6C,
                  D_80121D70, D_80121D74);
    func_8004C6B0(D_80121D50, D_80121D54, D_80121D4C, D_80121D58);
    func_80011DFC();
}
