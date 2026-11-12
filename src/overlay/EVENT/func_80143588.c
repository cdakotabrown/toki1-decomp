/* func_80143588 -- EVENT overlay, Address: 0x80143588 */
/* @SymbolStart: 0x80143564 */
/* @SymbolSize: 288 */
#include "types.h"

extern s32 D_80120404;
extern s32 D_8012041C;
extern s32 D_80120434;
extern s32 D_80120678;
extern s32 D_8012067C;
extern s32 D_80120680;
extern s32 ev_flag1c;
extern s32 D_80123488;
extern s32 D_8012348C;
extern s32 D_80123490;
extern s16 D_80123494;
extern s32 D_80123498;
extern s32 D_8012349C;
extern s32 D_801234A0;
extern s32 D_801234A4;
extern s32 D_801234A8;
extern s32 D_801234AC;
extern s32 D_801234B0;
extern volatile s16 D_800B0AD6;
extern volatile s16 D_800B0ADA;
extern volatile s16 D_800B0ADE;

extern void func_80106D80(void);
extern void func_80012D64(s32, s32, s32, s32, s32);
extern void func_8004C46C(s32, s32, s32, s32, s32, s32);
extern void func_8004C6B0(s32, s32, s32, s16);
extern void func_8004C250(void);
extern void func_80011DFC(void);

void func_80143588(void) {
    D_80120678 = D_80120404;
    D_8012067C = D_8012041C;
    D_80120680 = D_80120434;
    func_80106D80();
    func_80012D64(D_80123498, 17, 1, 2, 0);
    func_8004C46C(D_8012349C, D_801234A0, D_801234A4, D_801234A8,
                  D_801234AC, D_801234B0);
    func_8004C6B0(D_8012348C, D_80123490, D_80123488, D_80123494);
    ev_flag1c = 1;
    D_800B0AD6 = (s16)(D_800B0AD6 + 3);
    D_800B0ADA = (s16)(D_800B0ADA + 2);
    D_800B0ADE = (s16)(D_800B0ADE - 20);
    func_8004C250();
    func_80011DFC();
}
