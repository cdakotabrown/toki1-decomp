/* @CompileEnv: IDO52_FORCE_WEB_SPILL=18 */
/* @CompileFlags: -cckr */
/* @SymbolStart: 0x8014E750 */
/* @SymbolSize: 480 */
/* Address: 0x8014E774 */
/* Size: 444 bytes (111 instructions) */
#include "types.h"

extern s32 D_8009473C;
extern s32 D_80094740;
extern s32 D_80094744;
extern s32 D_80124234;
extern s32 D_801242A0;
extern s32 D_8012430C;

extern u8 D_80094784[];
extern u8 D_800947C4[];
extern u8 D_8011FCAC[];
extern u8 D_8011FCB4[];

extern u8 D_800B0A20[];
extern u8 D_800B1746;
extern u8 D_801252E8;
extern u8 D_8012531C;
extern s32 ev_flag18;

extern u8 *D_80125228;
extern u8 *D_8012522C;

extern s16 D_8012438C;
extern s16 D_8012438E;
extern s16 D_80124390;
extern s16 D_8012439C;
extern s16 D_8012439E;
extern s16 D_801243A0;

extern void func_80078970();
extern void func_800469F4();
extern void func_80011DFC();

void func_8014E774(void) {
    u8 *q;
    u8 *r;
    u32 j;

    D_8009473C = D_80124234;
    D_80094740 = D_801242A0;
    D_80094744 = D_8012430C;
    func_80078970(D_80094784, D_8011FCAC);
    func_80078970(D_800947C4, D_8011FCB4);
    D_801252E8 = 3;
    if (D_800B1746 == 7) {
        D_801252E8 = 5;
    }
    if ((D_800B0A20[D_800B1746 * 52] & 1) != 0) goto lab_E81C;
    D_8012531C = 0;
    goto lab_E824;
lab_E81C:
    D_8012531C = 1;
lab_E824:
    q = D_80125228;
    j = D_8012531C * 3;
    ev_flag18 = 8;
    D_8012439C = (q + D_800B1746 * 6U)[j + 0];
    D_8012439E = (q + (D_800B1746 << 0) * 6U)[j + 1];
    r = D_8012522C;
    D_801243A0 = (q + (D_800B1746 | 0U) * 6U)[j + 2];
    D_8012438C = (r + (D_800B1746 ^ 0U) * 6U)[j + 0];
    D_8012438E = (r + (D_800B1746 * 1U) * 6U)[j + 1];
    D_80124390 = (r + (D_800B1746 >> 0) * 6U)[j + 2];
    func_800469F4(17727);
    func_80011DFC();
}
