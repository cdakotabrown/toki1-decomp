/* func_80143420 -- EVENT overlay, Address: 0x80143420 */
/* @SymbolStart: 0x801433FC */
/* @SymbolSize: 360 */
#include "types.h"

extern s32 D_80120400;
extern s32 D_80120418;
extern s32 D_80120430;
extern s32 D_80120678;
extern s32 D_8012067C;
extern s32 D_80120680;
extern volatile s16 D_800B0AD6;
extern volatile s16 D_800B0ADA;
extern volatile s16 D_800B0ADE;
extern volatile s16 D_800B094E;
extern s32 D_8012345C;
extern s32 D_80123460;
extern s32 D_80123464;
extern s16 D_80123468;
extern s32 D_8012346C;
extern s32 D_80123470;
extern s32 D_80123474;
extern s32 D_80123478;
extern s32 D_8012347C;
extern s32 D_80123480;
extern s32 D_80123484;

extern void func_80106CD0(void);
extern void func_80012D64(s32, s32, s32, s32, s32);
extern void func_8004C46C(s32, s32, s32, s32, s32, s32);
extern void func_8004C6B0(s32, s32, s32, s16);
extern void func_8004C250(void);
extern void func_80011DFC(void);

void func_80143420(void) {
    D_80120678 = D_80120400;
    D_8012067C = D_80120418;
    D_80120680 = D_80120430;
    func_80106CD0();
    func_80012D64(D_8012346C, 17, 1, 2, 0);
    func_8004C46C(D_80123470, D_80123474, D_80123478, D_8012347C,
                  D_80123480, D_80123484);
    func_8004C6B0(D_80123460, D_80123464, D_8012345C, D_80123468);
    D_800B0AD6 = (s16)(D_800B0AD6 + 2);
    D_800B0ADA = (s16)(D_800B0ADA + 1);
    D_800B0ADE = (s16)(D_800B0ADE - 20);
    func_8004C250();
    D_800B0AD6 = (s16)(D_800B0AD6 + 3);
    D_800B0ADA = (s16)(D_800B0ADA + 2);
    D_800B0ADE = (s16)(D_800B0ADE - 20);
    D_800B094E = (s16)(D_800B094E + 10);
    func_8004C250();
    func_80011DFC();
}
