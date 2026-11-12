/* @SymbolStart: 0x801504AC */
/* @SymbolSize: 520 */
/* Address: 0x801504AC (cross-prologue head; .sym draws the start at 0x801504D8) */
/* Size: 520 bytes (130 instructions) including the absorbed head */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=18:t0 IDO52_AS1_XBB_VOLATILE_OK=all IDO52_EXCLUDE_REGS_WEB=24:v1,23:v1 */
#include "types.h"

extern volatile u8 D_800B1746;

extern s32 D_80124230;
extern s32 D_8012429C;
extern s32 D_80124308;
extern s32 D_8009473C;
extern s32 D_80094740;
extern s32 D_80094744;

extern u8 D_801252E8;
extern u8 D_80094784[];
extern u8 D_8011FE38[];
extern u8 D_800947C4[];
extern u8 D_8011FE40[];
extern u8 D_8011FE48[];

extern u8 D_800B0A35[];
extern u8 D_8012531C;
extern s16 D_80094730;
extern s32 ev_flag18;
extern u8 *D_80125220;
extern u8 *D_80125224;

extern s16 D_8012438C;
extern s16 D_8012438E;
extern s16 D_80124390;
extern s16 D_8012439C;
extern s16 D_8012439E;
extern s16 D_801243A0;

extern void func_80078970();
extern void func_800469F4();
extern void func_80011DFC();

void func_801504D8(void) {
    u32 off;
    u8 gv;
    u8 *pa;
    u8 *pb;

    D_8009473C = D_80124230;
    D_80094740 = D_8012429C;
    D_80094744 = D_80124308;

    D_801252E8 = 3;
    func_80078970(D_80094784, D_8011FE38);
    func_80078970(D_800947C4, D_8011FE40);

    gv = D_800B1746;
    if (gv == 6U) {
        func_80078970(D_800947C4, D_8011FE48);
    }

    if (!(D_800B0A35[D_800B1746 * 52] & 1)) {
        D_8012531C = 0;
    } else {
        D_8012531C = 1;
        if (D_800B1746 == 5) {
            D_80094730 = 2;
        }
    }

    gv = D_800B1746;
    pa = D_80125220;
    off = D_8012531C * 3U;
    ev_flag18 = 29;

    D_8012439C = (pa + gv * 6U)[off + 0];
    D_8012439E = (pa + (u32)gv * 6U)[off + 1];
    D_801243A0 = (pa + (gv | 0U) * 6U)[off + 2];
    pb = D_80125224;
    D_8012438C = (pb + (gv ^ 0U) * 6U)[off + 0];
    D_8012438E = (pb + (gv * 1U) * 6U)[off + 1];
    D_80124390 = (pb + (gv >> 0) * 6U)[off + 2];

    func_800469F4(17679);
    func_80011DFC();
}
