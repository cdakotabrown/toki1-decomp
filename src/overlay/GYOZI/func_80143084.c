/* @CompileEnv: IDO52_COPYPROP_SITE=lod:1,lod:2      (Hook 32) */
/* @CompileEnv: IDO52_COPYPROP_SITE=lod:1,lod:2 */
/* func_80143084 -- GYOZI overlay, Address: 0x80143084, Size: 716 bytes */
#include "types.h"

typedef struct {
      s32 unk00;
      s16 unk04;
      s16 unk06;
      s16 unk08;
      s16 unk0A;
      s16 unk0C;
      u8  unk0E;
      u8  unk0F;
      s16 unk10;
      s16 unk12;
      u8  unk14;
      u8  unk15;
      u8  unk16;
      u8  unk17;
      s16 unk18;
      s16 unk1A;
      s16 unk1C;
      s16 unk1E;
      s32 unk20;
} GyoziEnt;

typedef union {
    s32 w;
    struct {
        u16 lo;
        s16 hi;
    } h;
} GyoziFx;

extern void func_80089200(void);
extern void func_8013ED84(void);
extern void func_8008F618(s32);
extern void func_80143BA0(void);
extern void func_8004EE18(s32, s32, s32, s32, s32);
extern void func_8008FD1C(s32, s32, s32, s32, s32, s32);
extern void func_8008FF60(s32, s32, s32, s32);
extern void func_800BCE10(s8 *, s8 *);
extern s32  func_800BCE40(void);
extern void func_8004DE1C(void);

extern s16 D_800D9250;
extern s16 D_800D9254;
extern s8  D_800D92A0[];
extern u8  D_800F53A0[];
extern u8  D_800F62CF;
extern s8  D_8012B8C4;
extern s8  D_8012B908;
extern GyoziEnt D_8012CA40[];
extern s8  D_80145DE0[];
extern s32 D_801482F8;
extern s32 D_80148430;
extern s32 D_80148568;
extern s32 D_80148628;
extern s32 D_8014862C;
extern s32 D_80148630;
extern s32 D_80148B74;
extern s32 D_80148B94;
extern s32 D_80148B98;
extern s32 D_80148B9C;
extern s16 D_80148BA0;
extern s32 D_80148BA4;
extern s32 D_80148BA8;
extern s32 D_80148BAC;
extern s32 D_80148BB0;
extern s32 D_80148BB4;
extern s32 D_80148BB8;
extern s32 D_80148BBC;
extern s32 D_80148BF0[];
extern GyoziFx D_80148CB8[];
extern s32 D_80148D80[];
extern s16 D_80148E48[];
extern s16 D_80148EAC;
extern s32 D_80148EB0;

void func_80143084(void)
{
    s16 i;
    s32 r;
    u8 v8;

    D_800D9250 = 0;
    D_800D9254 = 0;
    func_80089200();
    func_8013ED84();
    func_8008F618(D_800F62CF);
    func_80143BA0();
    func_8004EE18(D_80148BA4, 17, 1, 2, 0);
    func_8008FD1C(D_80148BA8, D_80148BAC, D_80148BB0, D_80148BB4, D_80148BB8,
                  D_80148BBC);
    func_8008FF60(D_80148B98, D_80148B9C, D_80148B94, D_80148BA0);
    v8 = 8;
    D_8012B8C4 = v8;
    D_8012B908 = v8;
    func_8013ED84();
    D_80148628 = D_801482F8;
    D_8014862C = D_80148430;
    D_80148630 = D_80148568;
    func_800BCE10(D_800D92A0, D_80145DE0);
    func_8004EE18(D_80148B74, 18, 1, 3, 0);

    for (i = 4; i < 54; i++) {
        D_8012CA40[i].unk00 = 0xC1000000;
        r = func_800BCE40();
        D_8012CA40[i].unk04 = (s16)(r % 320 - 159);
        D_80148CB8[i - 4].h.lo = 0;
        D_80148CB8[i - 4].h.hi = D_8012CA40[i].unk04;
        D_80148D80[i - 4] = D_80148CB8[i - 4].w;
        D_8012CA40[i].unk06 = -120;
        D_8012CA40[i].unk08 = 8;
        D_8012CA40[i].unk0A = 8;
        D_8012CA40[i].unk0C = 10;
        D_8012CA40[i].unk0E = 0;
        D_8012CA40[i].unk0F = 0;
        D_8012CA40[i].unk10 = 0;
        D_8012CA40[i].unk12 = 498;
        D_8012CA40[i].unk14 = 128;
        D_8012CA40[i].unk15 = 128;
        D_8012CA40[i].unk16 = 128;
        D_8012CA40[i].unk18 = 0;
        D_8012CA40[i].unk1A = 0;
        D_8012CA40[i].unk1C = 4096;
        D_8012CA40[i].unk1E = 4096;
        D_8012CA40[i].unk20 = 0;
        D_800F53A0[i + 4213] = 9;
        D_80148E48[i - 4] = 2;
        D_80148BF0[i - 4] = 32768;
    }

    D_80148EAC = 0;
    D_80148EB0 = 0;
    func_8004DE1C();
}
