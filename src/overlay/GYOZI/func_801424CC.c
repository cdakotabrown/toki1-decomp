/* @CompileEnv: IDO52_CONST_INREG_VALUES=8 */
/* @PostProc: swap_instructions --offset1 0x29c --offset2 0x2a4 */
/* @PostProc: swap_instructions --offset1 0x2a0 --offset2 0x2a8 */
/* @PostProc: swap_instructions --offset1 0x2a4 --offset2 0x2ac */
/* @PostProc: swap_instructions --offset1 0x2a8 --offset2 0x2ac */
/* @PostProc: patch_insn_reg_field 0x278:rt:v0:v1 0x280:rt:v0:v1 0x2c4:rt:v0:v1 */
/* func_801424CC -- GYOZI overlay, Address: 0x801424CC, Size: 752 bytes. */
#include "types.h"

typedef struct {
      u32 unk00;
      s16 unk04;
      s16 unk06;
      s16 unk08;
      s16 unk0A;
      s16 unk0C;
      u8 unk0E;
      u8 unk0F;
      s16 unk10;
      s16 unk12;
      u8 unk14;
      u8 unk15;
      u8 unk16;
      u8 unk17;
      s16 unk18;
      s16 unk1A;
      s16 unk1C;
      s16 unk1E;
      u32 unk20;
} Sprite;

typedef union {
    struct {
          s16 unk0;
          s16 unk2;
    } h;
      u32 w;
} Pair;

extern void func_800BCE90(s32 arg0);
extern void func_80089200(void);
extern void func_8008F618(s32 arg0);
extern void func_80143AD0(void);
extern void func_8013ED84(void);
extern void func_8004EE18(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4);
extern s32 func_800BCE40(void);
extern void func_8008FD1C(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
extern void func_8008FF60(s32 a0, s32 a1, s32 a2, s32 a3);
extern void func_8008FB00(void);
extern void func_8004DE1C(void);

extern s16 D_800D9250;
extern s16 D_800D9254;
extern s16 D_800F554E;
extern s16 D_800F5552;
extern u8 D_800F62CF;
extern u8 D_800F6415[];
extern s32 D_800F6468;
extern u8 D_8012B8C4;
extern u8 D_8012B908;
extern Sprite D_8012CA40[];
extern s32 D_8014829C;
extern s32 D_801483D4;
extern s32 D_8014850C;
extern s32 D_80148628;
extern s32 D_8014862C;
extern s32 D_80148630;
extern s32 D_80148B60;
extern s32 D_80148B64;
extern s32 D_80148B68;
extern s16 D_80148B6C;
extern s32 D_80148B70;
extern s32 D_80148B74;
extern s32 D_80148B78;
extern s32 D_80148B7C;
extern s32 D_80148B80;
extern s32 D_80148B84;
extern s32 D_80148B88;
extern s32 D_80148B8C;
extern u32 D_80148BE0[];
extern Pair D_80148CB8[];
extern Pair D_80148D70[];
extern s16 D_80148E40[];
extern s16 D_80148EAC;
extern s32 D_80148EB0;

void func_801424CC(void)
{
    Sprite *sp;
    s16 i;
    s32 r;

    func_800BCE90(D_800F6468);
    D_800D9250 = 0;
    D_800D9254 = 0;
    func_80089200();
    func_8008F618(D_800F62CF);
    func_80143AD0();
    func_8013ED84();

    D_80148628 = D_8014829C;
    D_8014862C = D_801483D4;
    D_80148630 = D_8014850C;
    func_8004EE18(D_80148B74, 18, 1, 3, 0);

    for (i = 4; i < 54; i++) {
        D_8012CA40[i].unk00 = 0xC1000000;
        r = func_800BCE40();
        D_8012CA40[i].unk04 = (r % 320) - 159;
        D_80148CB8[i - 4].h.unk0 = 0;
        D_80148CB8[i - 4].h.unk2 = D_8012CA40[i].unk04;
        D_80148D70[i].w = D_80148CB8[i - 4].w;
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
        D_800F6415[i] = 9;
        D_80148E40[i] = 2;
        D_80148BE0[i] = 0x8000;
    }

    func_8004EE18(D_80148B70, 17, 1, 2, 0);
    func_8008FD1C(D_80148B78, D_80148B7C, D_80148B80, D_80148B84, D_80148B88,
                  D_80148B8C);
    func_8008FF60(D_80148B64, D_80148B68, D_80148B60, D_80148B6C);

    D_8012B8C4 = 8;
    D_80148EAC = 0;
    D_80148EB0 = 0;
    D_800F554E = D_800F554E + 1;
    D_800F5552 = D_800F5552 + 2;
    D_8012B908 = 8;
    func_8008FB00();
    func_8004DE1C();
}
