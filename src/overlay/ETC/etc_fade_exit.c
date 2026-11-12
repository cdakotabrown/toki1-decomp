/* @SymbolSize: 188 */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=7:v1,0:a1,30:a0 IDO52_AS1_TIE_REJECT=21 */
/* @PostProc: patch_insn_reg_field 0x20:rt:v0:a1 0x24:rs:v0:a1 */
/* Address: 0x80143E8C */
/* Size: 188 bytes (47 instructions) */
#include "types.h"

extern void goto_step(s32);
extern void all_object_disp_off(void);
extern void etc_sprite_fade(void);
extern u8 screen_bri;
extern u8 now_girl;
extern s16 etc_roll_tz;
extern u8 sprite2_attribute[];
extern u8 D_80129D64;

void etc_fade_exit(void) {
    s32 a1;
    u8 *q;

    etc_sprite_fade();
    a1 = screen_bri;
    if (a1 != 0) {
        q = sprite2_attribute;
        do {
            q += 36;
            q[-14] = (u8)a1;
            q[-15] = (u8)a1;
            q[-16] = (u8)a1;
        } while (q != &D_80129D64);
        screen_bri = (u8)(a1 - 1);
    }
    etc_roll_tz = etc_roll_tz - 200;
    if (etc_roll_tz >= -30000) {
        return;
    }
    all_object_disp_off();
    if ((u32)now_girl >= 13) {
        goto_step(18);
    } else {
        goto_step(195);
    }
}
