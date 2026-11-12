/* func_80133CF4 -- EVENT overlay, Address: 0x80133CF4 */
/* @SymbolStart: 0x80133C94 */
/* @SymbolSize: 688 */
#include "types.h"

typedef struct { s32 w[17]; } Desc68;

extern s32 D_80120530;
extern s32 D_80120548;
extern s32 D_80120560;
extern s32 D_80120678;
extern s32 D_8012067C;
extern s32 D_80120680;
extern s32 D_801206E8;
extern s32 D_801206EC;
extern s32 D_801206F0;
extern s16 D_801206F4;
extern s32 D_801206F8;
extern s32 D_801206FC;
extern s32 D_80120700;
extern s32 D_80120704;
extern s32 D_80120708;
extern s32 D_8012070C;
extern s32 D_80120710;
extern s16 D_801207AC;
extern s16 D_801207B0;
extern s16 D_801207B4;
extern s32 D_800B1C70;
extern s32 D_800B1CF0;
extern volatile s16 D_800B0BA6;
extern volatile s16 D_800B0BAA;
extern volatile s16 D_800B0BAE;
extern Desc68 D_800EAFA0;
extern Desc68 D_800EB028;
extern Desc68 D_800EB06C;
extern u8 D_800EB029;
extern u8 D_800EB02A;
extern u8 D_800EB06B;
extern u8 D_800EB06D;
extern u8 D_800EB06E;
extern u8 D_800EB0AF;
extern s16 D_800EB040;
extern s16 D_800EB084;

extern void func_8004C250(void);
extern void func_800F7380(void);
extern void func_80012D64(s32, s32, s32, s32, s32);
extern void func_8004C46C(s32, s32, s32, s32, s32, s32);
extern void func_8004C6B0(s32, s32, s32, s16);
extern void func_80011DFC(void);

void func_80133CF4(void) {
    s32 pad_hi;
    Desc68 buf;

    D_80120678 = D_80120530;
    D_8012067C = D_80120548;
    D_80120680 = D_80120560;
    D_800B0BA6 = (s16)(D_800B0BA6 + 3);
    D_800B0BAA = (s16)(D_800B0BAA + 2);
    D_800B0BAE = (s16)(D_800B0BAE - 20);
    func_8004C250();
    D_800B1C70 = -1;
    D_800B1CF0 = -1;
    func_800F7380();
    func_80012D64(D_801206F8, 17, 1, 2, 0);
    func_80012D64(D_801206F8, 18, 1, 2, 4);
    func_80012D64(D_801206F8, 19, 1, 2, 0);
    func_8004C46C(D_801206FC, D_80120700, D_80120704, D_80120708,
                  D_8012070C, D_80120710);
    func_8004C6B0(D_801206EC, D_801206F0, D_801206E8, D_801206F4);
    buf = D_800EAFA0;
    D_800EB06C = buf;
    D_800EB028 = buf;
    D_800EB06E = 64;
    D_800EB02A = 64;
    D_800EB040 = 8;
    D_800EB029 = 12;
    D_800EB06B = 19;
    D_800EB084 = 9;
    D_800EB06D = 12;
    D_800EB0AF = 18;
    D_801207B0 = 0;
    D_801207AC = 0;
    D_801207B4 = 1;
    func_80011DFC();
}
