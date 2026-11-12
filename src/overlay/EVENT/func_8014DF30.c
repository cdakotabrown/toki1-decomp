/* @SymbolStart: 0x8014DF04 */
/* @SymbolSize: 776 */
/* @CompileFlags: -cckr */
#include "types.h"

typedef struct { u8 b[52]; } CharRec;
typedef struct {
    CharRec rec[64];
    u8 pad[46];
    u8 cur;
} CharBlock;

extern s32 D_80124254;
extern s32 D_801242C0;
extern s32 D_8012432C;
extern s32 D_8009473C;
extern s32 D_80094740;
extern s32 D_80094744;
extern s32 ev_flag04;
extern s32 ev_flag18;
extern u8 D_801252E8;
extern u8 D_80094784[];
extern u8 D_800947C4[];
extern u8 D_8011FC3C[];
extern u8 D_8011FC4C[];
extern u8 D_8011FC50[];
extern u8 D_8011FC58[];
extern u8 D_800B1746;
extern u8 D_800B0896;
typedef struct { u8 b[4]; } Date;
extern Date D_800B0894;
extern u8 D_800B0897;
extern u8 D_800B0E2A;
extern u8 D_800B0E43;
extern u8 D_800B0E6C;
extern CharBlock D_800B0A18;
extern u8 D_8012531C;
extern u8 *D_80125268;
extern u8 *D_8012526C;
extern s16 D_8012438C[];
extern s16 D_8012439C[];
extern void func_80078970();
extern s32 func_8002328C();
extern s32 func_8002336C();
extern void func_800469F4();
extern void func_80011DFC();

void func_8014DF30()
{
    s32 r;
    u8 saved;

    D_8009473C = D_80124254;
    D_80094740 = D_801242C0;
    D_80094744 = D_8012432C;
    ev_flag04 = 0;
    func_80078970(D_80094784, D_8011FC3C);
    D_801252E8 = 3;
    saved = func_8002328C(D_800B1746);
    r = func_8002336C(D_800B1746);
    if (((D_800B0896 == 3 && D_800B0897 >= 26)
         || (D_800B0896 == 4 && D_800B0897 < 11))
        && (D_800B0E2A || D_800B1746)
        && (D_800B0E43 || D_800B1746 != 4 || saved != 128 || r)) {
        D_8012531C = 2;
        D_801252E8 = 3;
        func_80078970(D_800947C4, D_8011FC4C);
        ev_flag18 = -1;
    } else if ((D_800B0A18.rec[D_800B0A18.cur].b[0] & 1) == 0
               && (D_800B0894.b[2] < 6 || D_800B0894.b[2] >= 9 || D_800B0E6C
                   || saved != 128 || r)) {
        D_8012531C = 0;
        func_80078970(D_800947C4, D_8011FC50);
        ev_flag18 = 0;
    } else {
        D_8012531C = 1;
        func_80078970(D_800947C4, D_8011FC58);
        ev_flag18 = 0;
    }
    D_8012439C[0] = (D_80125268 + (D_800B1746 | 0) * 9)[D_8012531C * 3 + 0];
    D_8012439C[1] = (D_80125268 + (D_800B1746 ^ 0) * 9)[D_8012531C * 3 + 1];
    D_8012439C[2] = (D_80125268 + (D_800B1746 >> 0) * 9)[D_8012531C * 3 + 2];
    D_8012438C[0] = (D_8012526C + (D_800B1746 << 0) * 9)[D_8012531C * 3 + 0];
    D_8012438C[1] = (D_8012526C + (D_800B1746 / 1) * 9)[D_8012531C * 3 + 1];
    D_8012438C[2] = (D_8012526C + (D_800B1746 * 1) * 9)[D_8012531C * 3 + 2];
    func_800469F4(17015);
    func_80011DFC();
}
