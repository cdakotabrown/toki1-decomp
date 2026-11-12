/* func_8013E8C4 -- GYOZI overlay, Address: 0x8013E8C4, Size: 364 bytes (91 insns) */
#include "types.h"

extern void func_80090960(s32, s32);
extern void func_800BCE10(s32, s32);
extern void func_8004DE1C(void);
extern void func_80140740(void);
extern void func_8004EE18(s32, s32, s32, s32, s32);
extern void func_800549E8(s32);

extern u8 D_800F594E;

extern s32 D_800D9280;
extern s32 D_800D9288;
extern s32 D_800D92E0;
extern s32 D_80145D20;
extern s32 D_80145D28;
extern s32 D_80145D30;

extern s16 D_80148620;
extern s32 D_801486D0;
extern s32 D_801486D4;
extern s32 D_801486D8;
extern s16 D_801486DC;
extern s32 D_801486E0;

extern s8  D_8012B8C1;
extern s8  D_8012B8C2;
extern s8  D_8012B8C3;
extern s8  D_8012B8C4;
extern s8  D_8012B8C5;
extern s8  D_8012B8C7;
extern s32 D_8012B8CC;
extern s32 D_8012B8D0;
extern s16 D_8012B8D4;
extern s16 D_8012B8D6;
extern s16 D_8012B8D8;
extern s16 D_8012B8E6;
extern s16 D_8012B8EA;
extern s32 D_8012B8F4;
extern s32 D_8012B8F8;
extern s8  D_8012B903;

void func_8013E8C4(void)
{
    func_80090960(4, 0);
    D_800F594E = D_800F594E + 1;
    func_800BCE10((s32)&D_800D9280, (s32)&D_80145D20);
    func_800BCE10((s32)&D_800D9288, (s32)&D_80145D28);
    func_800BCE10((s32)&D_800D92E0, (s32)&D_80145D30);
    D_80148620 = 9;
    func_8004DE1C();
    func_80140740();
    func_8004EE18(D_801486E0, 17, 1, 3, 0);
    func_800549E8(96);
    D_8012B8C1 = 12;
    D_8012B8C2 = 1;
    D_8012B8F8 = 0x41000000;
    D_8012B8C3 = 4;
    D_8012B8CC = D_801486D4;
    D_8012B8D0 = D_801486D8;
    D_8012B8F4 = D_801486D0;
    D_8012B8D4 = (s16)D_801486DC;
    D_8012B8D6 = 0;
    D_8012B8D8 = 0;
    D_8012B903 = 17;
    D_8012B8E6 = -160;
    D_8012B8EA = -120;
    D_8012B8C7 = 0;
    D_8012B8C5 = 0;
    D_8012B8C4 = 8;
}
