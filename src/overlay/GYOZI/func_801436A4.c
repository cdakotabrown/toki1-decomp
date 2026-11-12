/* @SymbolStart: 0x8014369C */
/* @SymbolSize: 316 */
/* func_801436A4 - GYOZI overlay, Address: 0x801436A4, Size: 308 bytes (77 */
#include "types.h"

extern void func_80143C70(void);
extern void func_8004EE18(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4);
extern void func_8008FD1C(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
extern void func_8008FF60(s32 a0, s32 a1, s32 a2, s32 a3);
extern void func_8004DE1C(void);

extern s32 D_800F6600;
extern s32 D_800F6680;
extern s32 D_80148BD0;
extern s32 D_80148BD4;
extern s32 D_80148BD8;
extern s32 D_80148BDC;
extern s32 D_80148BE0;
extern s32 D_80148BE4;
extern s32 D_80148BE8;
extern s32 D_80148BC4;
extern s32 D_80148BC8;
extern s32 D_80148BC0;
extern s16 D_80148BCC;
extern s8  D_8012B908;
extern s8  D_8012B8C4;
extern s16 D_8012B95E;
extern s16 D_8012B960;
extern s16 D_8012B950;
extern s8  D_8012B94A;

typedef struct { s32 b[17]; } Blk68;
extern Blk68 D_8012B8C0[];

void func_801436A4(void)
{
    D_800F6600 = -1;
    D_800F6680 = -1;
    func_80143C70();
    func_8004EE18(D_80148BD0, 17, 1, 2, 0);
    func_8008FD1C(D_80148BD4, D_80148BD8, D_80148BDC, D_80148BE0,
                  D_80148BE4, D_80148BE8);
    func_8008FF60(D_80148BC4, D_80148BC8, D_80148BC0, D_80148BCC);
    D_8012B908 = 8;
    D_8012B8C4 = 8;
    D_8012B8C0[2] = D_8012B8C0[0];
    D_8012B95E = 4;
    D_8012B960 = 0;
    D_8012B950 = 0;
    D_8012B94A = 0;
    func_8004DE1C();
}
