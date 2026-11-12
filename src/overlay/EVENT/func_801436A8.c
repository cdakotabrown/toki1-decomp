/* func_801436A8 -- EVENT overlay, Address: 0x801436A8 */
/* @SymbolStart: 0x80143684 */
/* @SymbolSize: 256 */
#include "types.h"

extern s32 D_80120408;
extern s32 D_80120420;
extern s32 D_80120438;
extern s32 D_80120678;
extern s32 D_8012067C;
extern s32 D_80120680;
extern s32 D_801234B4;
extern s32 D_801234B8;
extern s32 D_801234BC;
extern s16 D_801234C0;
extern s32 D_801234C4;
extern s32 D_801234C8;
extern s32 D_801234CC;
extern s32 D_801234D0;
extern s32 D_801234D4;
extern s32 D_801234D8;
extern s32 D_801234DC;
extern volatile s16 D_800B0ADA;
extern volatile s16 D_800B0ADE;

extern void func_80106E30(void);
extern void func_80012D64(s32, s32, s32, s32, s32);
extern void func_8004C46C(s32, s32, s32, s32, s32, s32);
extern void func_8004C6B0(s32, s32, s32, s16);
extern void func_8004C250(void);
extern void func_80011DFC(void);

void func_801436A8(void) {
    D_80120678 = D_80120408;
    D_8012067C = D_80120420;
    D_80120680 = D_80120438;
    func_80106E30();
    func_80012D64(D_801234C4, 17, 1, 2, 0);
    func_8004C46C(D_801234C8, D_801234CC, D_801234D0, D_801234D4,
                  D_801234D8, D_801234DC);
    func_8004C6B0(D_801234B8, D_801234BC, D_801234B4, D_801234C0);
    D_800B0ADA = (s16)(D_800B0ADA + 2);
    D_800B0ADE = (s16)(D_800B0ADE - 20);
    func_8004C250();
    func_80011DFC();
}
