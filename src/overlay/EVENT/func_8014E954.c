/* Address: 0x8014E930 (cross-prologue head; .sym draws the start at 0x8014E954) */
/* Size: 700 bytes (175 insns) including the absorbed 9-insn head */
/* @CompileEnv: IDO52_NO_CSE_SITE=cvt:2 IDO52_FORCE_WEB_COLOR=59:a3 */
/* @SymbolStart: 0x8014E930 */
/* @SymbolSize: 700 */
/* @RodataBase: 0x8011FF88 */
#include "types.h"

extern s32 D_8012422C;
extern s32 D_80124298;
extern s32 D_80124304;
extern s32 D_8009473C;
extern s32 D_80094740;
extern s32 D_80094744;
extern s32 ev_flag18;
extern s16 D_80094730;

extern u8 D_801252E8;
extern u8 D_80094784[];
extern u8 D_800947C4[];
extern u8 D_8011FCBC[];
extern u8 D_8011FCCC[];
extern u8 D_8011FCD4[];

extern u8 D_800B1746;
extern volatile u8 D_800B1746_v;
extern u8 D_800B0A21[];
extern s16 D_800B0956;
extern u8 D_8012531C;

extern u8 *D_80125218;
extern u8 *D_8012521C;

extern s16 D_8012438C;
extern s16 D_8012438E;
extern s16 D_80124390;
extern s16 D_8012439C;
extern s16 D_8012439E;
extern s16 D_801243A0;

extern void func_80078970(u8 *, u8 *);
extern void func_800469F4(s32);
extern void func_80011DFC(void);

void func_8014E954(void)
{
    u32 off;

    D_8009473C = D_8012422C;
    D_80094740 = D_80124298;
    D_80094744 = D_80124304;

    func_80078970(D_80094784, D_8011FCBC);

    D_801252E8 = 7;
    if (!(D_800B0A21[D_800B1746 * 52] & 1)) {
        D_8012531C = 0;
        func_80078970(D_800947C4, D_8011FCCC);
    } else {
        D_8012531C = 1;
        func_80078970(D_800947C4, D_8011FCD4);
        if (D_800B1746_v == 10) {
            D_80094730 = 1;
        }
    }

    off = D_8012531C * 3U;
    ev_flag18 = 9;

    D_8012439C = (D_80125218 + (u32)D_800B1746 * 6U)[off + 0];
    D_8012439E = (D_80125218 + (D_800B1746 + 0U) * 6U)[off + 1];
    D_801243A0 = (D_80125218 + (D_800B1746 | 0U) * 6U)[off + 2];
    D_8012438C = (D_8012521C + (D_800B1746 ^ 0U) * 6U)[off + 0];
    D_8012438E = (D_8012521C + (D_800B1746 * 1U) * 6U)[off + 1];
    D_80124390 = (D_8012521C + (D_800B1746 >> 0) * 6U)[off + 2];

    if (D_800B0956 < 120) {
        if (D_8012531C != 0) {
            switch ((u32)D_800B1746) {
            case 0:
                D_8012439C = 4;
                D_8012438C = 4;
                break;
            case 1:
                D_8012439C = 4;
                D_8012438C = 3;
                break;
            case 2:
                D_8012438C = 3;
                D_8012439C = 3;
                break;
            case 3:
                D_80124390 = 3;
                D_801243A0 = 3;
                break;
            case 4:
                D_80124390 = 1;
                D_801243A0 = 1;
                break;
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
            case 10:
                break;
            }
        }
    }

    func_800469F4(17746);
    func_80011DFC();
}
