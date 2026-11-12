/* func_801506D8 -- EVENT overlay, Address: 0x801506D8 */
/* @SymbolStart: 0x801506B4 */
/* @SymbolSize: 208 */
/* @PostProc: patch_insn_reg_field 0x90:rt:t0:t1 0x98:rt:t0:t1 0xb8:rt:t1:t2 0xbc:rt:t1:t2 */
#include "types.h"

extern s32 D_80124210;
extern s32 D_8012427C;
extern s32 D_801242E8;
extern s32 D_8009473C;
extern s32 D_80094740;
extern s32 D_80094744;
extern u8 D_80094784[];
extern u8 D_800947C4[];
extern u8 D_8011FE50[];
extern u8 D_8011FE58[];
extern u8 D_801252E8;
extern u8 D_8012531C;
extern s16 D_8012438C;
extern s16 D_8012438E;
extern s16 D_801243A4;
extern s16 D_801243A6;
extern u8 D_800B1746;
extern s32 ev_flag18;

extern void func_80078970(u32, u32);
extern s32 func_8002328C(s32);
extern void func_800469F4(u32);
extern void func_80011DFC(void);

void func_801506D8(void) {
    s32 sel;

    D_8009473C = D_80124210;
    D_80094740 = D_8012427C;
    D_80094744 = D_801242E8;
    func_80078970((u32)D_80094784, (u32)D_8011FE50);
    func_80078970((u32)D_800947C4, (u32)D_8011FE58);
    D_801252E8 = 5;
    D_8012438C = 0;
    sel = func_8002328C(D_800B1746);
    D_8012438E = (s16)(sel & 0x7F);
    D_801243A4 = 0;
    D_801243A6 = 0;
    D_8012531C = 0;
    ev_flag18 = -1;
    func_800469F4(16987);
    func_80011DFC();
}
