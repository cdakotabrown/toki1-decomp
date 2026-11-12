/* Address: 0x80157190 */
/* Size: 1016 bytes (254 instructions) */
#include "types.h"

typedef struct {
    u8  b00;
    u8  b01;
    s8  b02;
    u8  b03;
    u8  b04;
    u8  b05;
    s8  b06;
    u8  b07;
    u8  pad08[4];
    s32 w12;
    s32 w16;
    s16 h20;
    s16 h22;
    s16 h24;
    u8  pad26[12];
    s16 h38;
    u8  pad40[2];
    s16 h42;
    s32 w44;
    u8  pad48[4];
    s32 w52;
    s32 w56;
    u8  pad60[7];
    u8  b67;
} EntA;

typedef struct {
    s32 w00;
    u8  pad04[14];
    s16 h18;
    u8  b20;
    u8  b21;
    u8  b22;
    u8  pad23[13];
} EntB;

extern u8   D_800F19A9;
extern s32  D_800B0860[];
extern s16  D_800F1996;
extern s16  D_800F1994;
extern u8   D_80125864;
extern s32  ev_flag24;
extern s32  D_8012585C;
extern s16  D_80125860;
extern s32  D_80094710;
extern u8   D_800E9E9F;
extern s16  D_800EC1A2;
extern s16  D_800EC1C6;
extern s32  D_80125280;
extern s32  D_80125284;
extern s32  D_80125288;
extern EntA D_800E9620[];
extern s32  D_80125270;
extern s32  D_80125274;
extern s32  D_80125278;
extern s16  D_8012527C;
extern s32  D_801257DC[];
extern u8   D_800EAFA3;
extern s32  D_8012528C;
extern s32  D_80125290;
extern s32  D_80125294;
extern s32  D_80125298;
extern s32  D_8012529C;
extern s32  D_801252A0;
extern EntB D_800EC190[];

extern void func_80110094(void);
extern void func_80012D64(s32 a, s32 b, s32 c, s32 d, s32 e);
extern void func_800189C4(s32 a);
extern s32  func_800789A0(void);
extern void func_8004C46C(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
extern void func_80011DFC(void);

void func_80157190(void) {
    s16 i;
    s32 r;

    if (D_800F19A9 == 1) {
        for (i = 11; i < 16; i++) {
            D_800B0860[i + 1258] = -1;
            D_800B0860[i + 1290] = -1;
        }
        D_800F1996 = 0;
    } else {
        for (i = 21; i < 26; i++) {
            D_800B0860[i + 1258] = -1;
            D_800B0860[i + 1274] = -1;
        }
        D_800F1994 = 0;
    }

    D_80125864 = 128;
    ev_flag24 = 1;
    D_8012585C = 0;
    D_80125860 = 1;
    func_80110094();

    func_80012D64(D_80094710, 15, 1, 2, 0);
    D_800E9E9F = 15;
    D_800EC1A2 = 495;
    D_800EC1C6 = 495;

    for (i = 16; i < 32; i++) {
        func_80012D64(D_80125280, i & 0xFF, 1, 0, i - 16);
    }
    func_80012D64(D_80125284, 14, 1, 0, 0);
    func_80012D64(D_80125288, 13, 1, 0, 0);

    for (i = 0; i < 10; i++) {
        func_800189C4(i + 96);
        D_800E9620[i + 96].b01 = 13;
        D_800E9620[i + 96].b02 = 1;
        D_800E9620[i + 96].w56 = 0x01000000;
        D_800E9620[i + 96].b03 = 4;
        D_800E9620[i + 96].w12 = D_80125274;
        D_800E9620[i + 96].w16 = D_80125278;
        D_800E9620[i + 96].w52 = D_80125270;
        D_800E9620[i + 96].h20 = D_8012527C;
        D_800E9620[i + 96].h22 = 3;
        D_800E9620[i + 96].h24 = 0;
        D_800E9620[i + 96].b06 = 1;
        D_800E9620[i + 96].b67 = 0;
        r = func_800789A0();
        D_800E9620[i + 96].h38 = (s16)(r % 256 - 128);
        D_800E9620[i + 96].h42 = 160;
        D_800E9620[i + 96].b07 = 0;
        D_800E9620[i + 96].b05 = 128;
        D_800E9620[i + 96].b04 = 8;
        r = func_800789A0();
        D_800E9620[i + 96].w44 = r % 32768 - 16384;
        r = func_800789A0();
        D_801257DC[i] = ((0 - r % 9) << 16) - 0x280000;
    }

    D_800EAFA3 |= 0x80;
    func_8004C46C(D_8012528C, D_80125290, D_80125294, D_80125298,
                  D_8012529C, D_801252A0);

    for (i = 0; i < 6; i++) {
        D_800EC190[i + 4].w00 = D_800EC190[i + 4].w00 & 0x7FFFFFFF;
        D_800EC190[i + 4].h18 = 494;
        *((u8 *)D_800B0860 + i + 4145) = 12;
    }
    for (i = 0; i < 6; i++) {
        D_800EC190[i + 4].b20 = D_800EC190[i + 4].b21 = D_800EC190[i + 4].b22 = 128;
    }
    func_80011DFC();
}
