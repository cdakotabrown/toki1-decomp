/* func_80134488 -- GYOZI overlay, Address: 0x80134488, Size: 604 bytes (151 insns) */
#include "types.h"

typedef struct Ent {
      s8 unk00;
      s8 unk01;
      s8 unk02;
      s8 unk03;
      s8 unk04;
      u8 unk05;
      s8 unk06;
      u8 unk07;
      s8 unk08[4];
      s32 unk0C;
      s32 unk10;
      s16 unk14;
      s16 unk16;
      s16 unk18;
      s16 unk1A[6];
      s16 unk26;
      s16 unk28;
      s16 unk2A;
      s8 unk2C[8];
      s32 unk34;
      s32 unk38;
      s8 unk3C[7];
      s8 unk43;
} Ent;

extern void func_80136220(void);
extern void func_8004EE18(s32, s32, s32, s32, s32);
extern void func_800549E8(s32);
extern void func_8004DE1C(void);

extern Ent D_80129F40[];

extern s32 D_80146150;
extern s32 D_80146154;
extern s32 D_80146158;
extern s32 D_8014615C;
extern s32 D_80146160;
extern s32 D_80146164;
extern s16 D_80146168;
extern s16 D_8014616C;
extern s32 D_80146170;
extern s32 D_80146174;
extern s8 D_801461A0;

void func_80134488(void)
{
    s16 i;

    D_801461A0 = 0;
    func_80136220();
    func_8004EE18(D_80146170, 17, 1, 2, 0);

    for (i = 0; i < 3; i++) {
        func_800549E8(i + 96);
        D_80129F40[i + 96].unk01 = 10;
        D_80129F40[i + 96].unk02 = 0;
        D_80129F40[i + 96].unk38 = 0x01000000;
        D_80129F40[i + 96].unk03 = 4;
        D_80129F40[i + 96].unk0C = D_80146158;
        D_80129F40[i + 96].unk10 = D_80146160;
        D_80129F40[i + 96].unk34 = D_80146150;
        D_80129F40[i + 96].unk14 = D_80146168;
        D_80129F40[i + 96].unk16 = 0;
        D_80129F40[i + 96].unk18 = 0;
        D_80129F40[i + 96].unk06 = 1;
        D_80129F40[i + 96].unk43 = 17;
        D_80129F40[i + 96].unk26 = -160;
        D_80129F40[i + 96].unk2A = -120;
        D_80129F40[i + 96].unk07 = 128;
        D_80129F40[i + 96].unk05 = 0;
        D_80129F40[i + 96].unk04 = 0;
    }

    D_80129F40[96].unk26 = 0;
    D_80129F40[97].unk16 = 3;
    D_80129F40[98].unk16 = 4;

    func_8004EE18(D_80146174, 18, 0, 0, 2);
    func_800549E8(99);

    D_80129F40[99].unk01 = 13;
    D_80129F40[99].unk02 = 0;
    D_80129F40[99].unk38 = 0;
    D_80129F40[99].unk03 = 4;
    D_80129F40[99].unk0C = D_8014615C;
    D_80129F40[99].unk10 = D_80146164;
    D_80129F40[99].unk34 = D_80146154;
    D_80129F40[99].unk14 = D_8014616C;
    D_80129F40[99].unk16 = 0;
    D_80129F40[99].unk18 = 0;
    D_80129F40[99].unk06 = 1;
    D_80129F40[99].unk43 = 0;
    D_80129F40[99].unk26 = -160;
    D_80129F40[99].unk2A = -120;
    D_80129F40[99].unk07 = 128;
    D_80129F40[99].unk05 = 129;
    D_80129F40[99].unk04 = 0;

    func_8004DE1C();
}
