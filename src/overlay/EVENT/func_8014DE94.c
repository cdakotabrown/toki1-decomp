/* @SymbolStart: 0x8014DE60 */
/* @SymbolSize: 164 */
/* Address: 0x8014DE94 */
/* Size: 112 bytes (28 instructions) */
#include "types.h"

extern s32 D_8009473C;
extern s32 D_80094740;
extern s32 D_80094744;
extern s32 D_8012421C;
extern s32 D_80124288;
extern s32 D_801242F4;
extern s32 ev_flag04;
extern s32 ev_flag18;
extern s32 D_80094784;
extern s32 D_8011FC30;
extern s16 D_801243A4;
extern s16 D_801243A6;
extern u8 D_801252E8;
extern u8 D_8012531C;

extern void func_80078970();
extern void func_800469F4();
extern void func_80011DFC();

void func_8014DE94(void) {

    D_8009473C = D_8012421C;
    D_80094740 = D_80124288;
    D_80094744 = D_801242F4;
    ev_flag04 = 0;
    D_8012531C = 0;
    func_80078970(&D_80094784, &D_8011FC30);
    D_801252E8 = 2;
    D_801243A4 = 0;
    D_801243A6 = 0;
    ev_flag18 = -1;
    func_800469F4(16996);
    func_80011DFC();
}
