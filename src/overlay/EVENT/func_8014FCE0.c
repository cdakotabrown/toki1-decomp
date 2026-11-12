/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_AS1_TIE_TAKE=41 */
/* @SymbolStart: 0x8014FCA0 */
/* @SymbolSize: 820 */
/* Address: 0x8014FCE0 (window from the absorbed head at 0x8014FCA0) */
/* Size: 820 bytes (205 instructions) over the true window */
#include "types.h"
extern s32 D_8009473C;
extern s32 D_80094740;
extern s32 D_80094744;
extern s32 D_80124220;
extern s32 D_8012428C;
extern s32 D_801242F8;
extern u8 D_801252E8;
extern u8 D_800B0895;

extern u8 D_800B0896;
extern u8 D_800B1746;
extern u8 D_80125314;
extern s32 ev_flag18;
extern u8 D_80094784[];
extern u8 D_800947C4[];
extern u8 D_8011FDFC[];
extern u8 D_8011FE04[];
extern u8 *D_80125208;
extern u8 *D_8012520C;
extern volatile s16 D_8012438C;
extern volatile s16 D_8012438E;
extern volatile s16 D_80124390;
extern s16 D_8012439C;
extern s16 D_8012439E;
extern volatile s16 D_801243A0;

extern void func_80078970(u8 *dst, u8 *src);
extern void func_800469F4(s32 arg0);
extern void func_80011DFC(void);

void func_8014FCE0(void)
{
    u32 slot;
    u8 gv;
    u8 *pa;
    u8 *pb;
    s32 pose;
    s32 eventId;

    D_8009473C = D_80124220;
    D_80094740 = D_8012428C;
    D_80094744 = D_801242F8;
    D_801252E8 = 0;
    eventId = D_800B0895;

if (eventId == 95) {
        slot = D_800B0896;
        if (slot >= 6 && slot < 9) {
            D_80125314 = 0;
        } else if (slot >= 9 && slot < 12) {
            D_80125314 = 1;
        } else {
            D_80125314 = 2;
        }
    } else if (eventId == 96) {
        slot = D_800B0896;
        if (slot != 0 && slot < 3) {
            D_80125314 = 2;
        } else if (slot >= 3 && slot < 6) {
            D_80125314 = 3;
        } else if (slot >= 6 && slot < 9) {
            D_80125314 = 4;
        } else if (slot >= 9 && slot < 12) {
            D_80125314 = 5;
        } else {
            D_80125314 = 6;
        }
    } else if (eventId == 97) {
        slot = D_800B0896;
        if (slot != 0 && slot < 3) {
            D_80125314 = 6;
        } else if (slot >= 3 && slot < 6) {
            D_80125314 = 7;
        } else if (slot >= 6 && slot < 9) {
            D_80125314 = 8;
        } else if (slot >= 9 && slot < 12) {
            D_80125314 = 9;
        } else {
            D_80125314 = 10;
        }
    } else {
        D_80125314 = 10;
    }

    ev_flag18 = -1;
    func_80078970(D_80094784, D_8011FDFC);
    func_80078970(D_800947C4, D_8011FE04);

    gv = D_800B1746;
    pa = D_80125208;
    pose = D_80125314 * 3;
    D_8012439C = (pa + gv * 33U)[pose + 0];
    D_8012439E = (pa + (s32)gv * 33U)[pose + 1];
    pb = D_8012520C;
    D_801243A0 = (pa + (gv | 0U) * 33U)[pose + 2];
    D_8012438C = (pb + (gv ^ 0U) * 33U)[pose + 0];
    D_8012438E = (pb + (gv * 1U) * 33U)[pose + 1];
    D_80124390 = (pb + (gv >> 0) * 33U)[pose + 2];
    func_800469F4(17248);
    func_80011DFC();
}
