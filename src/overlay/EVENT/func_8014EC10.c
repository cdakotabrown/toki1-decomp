/* @SymbolStart: 0x8014EBEC */
/* @SymbolSize: 1284 */
/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_FORCE_WEB_ADJSAVE=179:0.9 */
#include "types.h"

typedef struct { u8 b[52]; } CharRec;
typedef struct {
    CharRec rec[64];
    u8 pad[46];
    u8 cur;
} CharBlock;
extern s32 D_80124200;
extern s32 D_8012426C;
extern s32 D_801242D8;
extern s32 D_8009473C;
extern s32 D_80094740;
extern s32 D_80094744;
extern s32 ev_flag18;
extern u8 D_80094784[];
extern u8 D_800947C4[];
extern u8 D_8011FCDC[];
extern u8 D_8011FCE4[];
extern u8 D_8011FCF0[];
extern u8 D_8011FCF8[];
extern u8 D_8011FD04[];
extern u8 D_8011FD0C[];
extern u8 D_800B0895;
extern u8 D_800B0896;
extern u8 D_800B1746;
extern CharBlock D_800B0A18;
extern u8 D_801252E8;
extern u8 D_801252F4;
extern u8 D_801252F8;
extern u8 D_801252FC;
extern u8 D_8012531C;
extern s16 D_80094730;
extern u8 *D_801251D0;
extern u8 *D_801251D4;
extern s16 D_8012438C[];
extern s16 D_8012439C[];
extern void func_80078970();
extern void func_800469F4();
extern void func_80011DFC();

void func_8014EC10()
{
    D_8009473C = D_80124200;
    D_80094740 = D_8012426C;
    D_80094744 = D_801242D8;
    func_80078970(D_80094784, D_8011FCDC);
    D_801252E8 = 3;
    if ((D_800B0895 | 0) == 97 && D_800B0896 >= 9 && D_800B0896 < 12) {
        D_8012531C = 2;
        D_801252F4 = 1;
        func_80078970(D_800947C4, D_8011FCE4);
        if (D_800B0A18.rec[D_800B0A18.cur].b[11]) {
            D_8012531C = 5;
        }
        ev_flag18 = 11;
        if (D_800B1746 == 7) {
            D_80094730 = 2;
        }
    } else if ((D_800B0895 | 0) == 97 && D_800B0896 >= 3 && D_800B0896 < 6) {
        D_8012531C = 3;
        D_801252F8 = 1;
        func_80078970(D_800947C4, D_8011FCF0);
        if (D_800B0A18.rec[D_800B0A18.cur].b[12]) {
            D_8012531C = 5;
        }
        ev_flag18 = 12;
        if (D_800B1746 == 7) {
            D_80094730 = 4;
        }
    } else if (D_800B0895 == 95 && D_800B0896 >= 9 && D_800B0896 < 12) {
        D_8012531C = 4;
        D_801252FC = 1;
        func_80078970(D_800947C4, D_8011FCF8);
        if (D_800B0A18.rec[D_800B0A18.cur].b[13]) {
            D_8012531C = 5;
        }
        ev_flag18 = 13;
        if (D_800B1746 == 7) {
            D_80094730 = 4;
        }
    } else if ((D_800B0A18.rec[D_800B0A18.cur].b[10] & 1) == 0) {
        D_8012531C = 0;
        func_80078970(D_800947C4, D_8011FD04);
        ev_flag18 = 10;
    } else {
        D_8012531C = 1;
        func_80078970(D_800947C4, D_8011FD0C);
        ev_flag18 = 10;
        if (D_800B1746 == 7) {
            D_80094730 = 2;
        }
    }
    D_8012439C[0] = (D_801251D0 + (D_800B1746 | 0) * 15)[D_8012531C * 3 + 0];
    D_8012439C[1] = (D_801251D0 + (D_800B1746 ^ 0) * 15)[D_8012531C * 3 + 1];
    D_8012439C[2] = (D_801251D0 + (D_800B1746 >> 0) * 15)[D_8012531C * 3 + 2];
    D_8012438C[0] = (D_801251D4 + (D_800B1746 << 0) * 15)[D_8012531C * 3 + 0];
    D_8012438C[1] = (D_801251D4 + (D_800B1746 / 1) * 15)[D_8012531C * 3 + 1];
    D_8012438C[2] = (D_801251D4 + (D_800B1746 * 1) * 15)[D_8012531C * 3 + 2];
    if (D_8012531C == 5) {
        if (D_800B1746 != 1 && D_800B1746 != 3 && D_800B1746 != 7
            && D_800B1746 != 8) {
            D_8012438C[0] = D_8012438C[1] = D_8012438C[2] = 3;
            D_8012439C[0] = D_8012439C[1] = D_8012439C[2] = 3;
        } else {
            D_8012438C[0] = D_8012438C[1] = D_8012438C[2] = 4;
            D_8012439C[0] = D_8012439C[1] = D_8012439C[2] = 4;
        }
        if ((D_800B1746 | 0) == 3 && D_801252F4) {
            D_8012439C[0] = D_8012439C[1] = D_8012439C[2] = 3;
        }
        if (D_800B1746 == 10) {
            D_80094730++;
        }
    }
    func_800469F4(17501);
    func_80011DFC();
}
