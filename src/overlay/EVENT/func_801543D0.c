/* Address: 0x801543D0 (true start 0x8015438C: the head pointer/value copies */
/* Size: 0x80154418 - 0x8015438C = 140 bytes (35 instructions) */
/* @SymbolStart: 0x8015438C */
/* @SymbolSize: 140 */
#include "types.h"

extern u8 D_80124364;
extern u8 D_80124368;
extern s32 D_80124264;
extern s32 D_801242D0;
extern s32 D_8012433C;
extern s32 D_8009471C;
extern s32 D_80094720;
extern s32 D_80094724;
extern s32 D_80094728;
extern s32 D_8009472C;
extern s32 D_800EECBC;

extern s32 func_80049B20(s32, s32, s32);
extern void func_80011DFC(void);

void func_801543D0(void) {
    D_8009471C = (s32)&D_80124364;
    D_80094720 = (s32)&D_80124368;
    D_80094724 = D_80124264;
    D_80094728 = D_801242D0;
    D_8009472C = D_8012433C;
    if (func_80049B20(D_800EECBC, 0, 0) != 0) {
        func_80011DFC();
    }
}
