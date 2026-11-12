/* Function: func_800647B8 */
/* @CompileEnv: IDO52_UGEN_TEMP_REG=2:t1 */
/* Address: 0x800647B8 */
/* Size: 188 bytes (47 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* Status: CONFIRMED UNMATCHABLE FROM C */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern u8 D_801280D3;
extern u16 D_80128102;
extern u16 D_801280BE;
extern u8 D_8012815B;
extern u16 D_80128146;
extern u8 D_80128117;
extern s16 D_80128174;
extern s16 D_80128182;
extern s16 D_80128186;

extern void memcpy(void *, void *, s32);

extern u8 old_player_rec;
extern u8 player_rec;

void func_800647B8(u8 val, s16 t3_val, s32 s0_arg, s32 v1_arg) {

    D_801280D3 = val;
    D_80128102 = 1;
    D_801280BE = 1;
    D_8012815B = val;
    D_80128146 = 1;

    if (s0_arg < v1_arg) {
        D_80128117 = 68;
        D_801280D3 = 68;
        D_80128102 = 1;
        D_801280BE = 1;
        D_8012815B = 68;
        D_80128146 = 1;
    }

    D_80128174 = t3_val;
    D_80128182 = 0;
    D_80128186 = -72;
    memcpy(&old_player_rec, &player_rec, 124);
}
