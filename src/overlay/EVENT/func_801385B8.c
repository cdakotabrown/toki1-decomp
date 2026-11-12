/* func_801385B8 -- EVENT overlay, Address: 0x801385B8 */
/* @SymbolStart: 0x8013856C */
/* @SymbolSize: 332 */
/* @CompileEnv: IDO52_UGEN_NO_MOVE_NOALIAS=occ:0 */
#include "types.h"

typedef struct { s32 w[17]; } Desc68;

extern s32 D_80120374;
extern s32 D_8012038C;
extern s32 D_801203A4;
extern s32 D_80120678;
extern s32 D_8012067C;
extern s32 D_80120680;
extern s32 D_80121228;
extern s32 D_8012122C;
extern s32 D_80121230;
extern s16 D_80121234;
extern s32 D_80121238;
extern s32 D_8012123C;
extern s32 D_80121240;
extern s32 D_80121244;
extern s32 D_80121248;
extern s32 D_8012124C;
extern s32 D_80121250;
extern volatile s16 D_800B0A6E;
extern volatile s16 D_800B0A76;
extern Desc68 D_800EAFA0;
extern Desc68 D_800EB028;

extern void func_8004C250(void);
extern void func_800FBD10(void);
extern void func_80012D64(s32, s32, s32, s32, s32);
extern void func_8004C46C(s32, s32, s32, s32, s32, s32);
extern void func_8004C6B0(s32, s32, s32, s16);
extern void func_80011DFC(void);

void func_801385B8(void) {
    D_80120678 = D_80120374;
    D_8012067C = D_8012038C;
    D_80120680 = D_801203A4;
    D_800B0A6E = (s16)(D_800B0A6E + 2);
    D_800B0A76 = (s16)(D_800B0A76 - 10);
    func_8004C250();
    func_800FBD10();
    func_80012D64(D_80121238, 17, 1, 2, 0);
    func_8004C46C(D_8012123C, D_80121240, D_80121244, D_80121248,
                  D_8012124C, D_80121250);
    func_8004C6B0(D_8012122C, D_80121230, D_80121228, D_80121234);
    D_800EB028 = D_800EAFA0;
    func_80011DFC();
}
