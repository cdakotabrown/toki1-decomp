/* func_80137640 -- EVENT overlay, Address: 0x80137640 */
/* @SymbolStart: 0x801375CC */
/* @SymbolSize: 176 */
#include "types.h"
extern s32 D_800EECBC;
extern s16 D_8012438C;
extern s16 D_8012438E;
extern s16 D_80124390;
extern s16 D_801243A4;
extern s16 D_801243A6;
extern s16 D_801243A8;
extern s16 D_80124394;
extern s16 D_80124396;
extern s16 D_80124398;
extern s16 D_8012439C;
extern s16 D_8012439E;
extern s16 D_801243A0;
extern s16 D_80094714;
extern s16 D_80094718;
extern void func_80011DFC(void);
extern void func_800F6000(void);
void func_80137640(void) {
    D_800EECBC = 0;
    D_8012438C = 1;
    D_8012438E = 1;
    D_80124390 = 1;
    D_801243A4 = 0;
    D_801243A6 = 0;
    D_801243A8 = 0;
    D_80124394 = 1;
    D_80124396 = 1;
    D_80124398 = 1;
    D_8012439C = 0;
    D_8012439E = 0;
    D_801243A0 = 0;
    func_80011DFC();
    D_80094714 = 0;
    D_80094718 = 0;
    func_800F6000();
}
