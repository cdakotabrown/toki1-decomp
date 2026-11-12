/* @CompileEnv: IDO52_COPYPROP_VAR=-4 IDO52_PRECOLOR_WEB=v0:0 IDO52_CONST_INREG_VALUES=128 IDO52_FORCE_WEB_COLOR=16:a1,18:a0 IDO52_COPYPROP_SITE=lod:9:deny,lod:10:deny IDO52_AS1_TIE_TAKE=389,393 IDO52_AS1_TIE_REJECT=377,834,838,839,917,951,952 */
/* @CompileFlags: -Wc,-pic1 */
/* @SymbolStart: 0x8013CCD4 */
/* @SymbolSize: 584 */
/* Address: 0x8013CCDC (true entry 0x8013CCD4, cross-prologue head absorbed) */
/* Size: 584 bytes (146 instructions incl. the 2-word head) */
/* @SymbolStart/@SymbolSize (func_8013CCD4.c stays a comment-only ASM-ONLY */
#include "types.h"
#include "tokimeki/libgs_types.h"

extern void goto_sub_sub_step(s32);
extern s32  get_CD_State2(void);
extern void cd_read_start2(s32, s32, s32);
extern void bg_set();
extern void memcpy(s32, s32, s32);
extern void etc_model_matrix_set(void);

extern u8  screen_bri;
extern u8  step_mode;
extern s32 pad_trig;
extern s32 Bg;
extern s32 etc_event_phase;
extern s32 etc_model_roty;

s32 etc_sub_sub_step7(void)
{
    u32 sel;
    s32 buf[6];
    u32 i;
    s32 n;

    sel = step_mode;
    switch (sel) {
    case 0:
        n = etc_event_phase;
        if (n > 0) {
            cd_read_start2(105, 0x801A0000, 0xB389);
            step_mode++;
        } else {
            step_mode++;
        }
        break;
    case 1:
        n = etc_event_phase;
        if (n == 0) {
            step_mode++;
        } else if (n > 0) {
            if (get_CD_State2() & 1) {
                memcpy(0x801E2000, 0x801A0600, 0x18000);
                n = etc_event_phase;
                if (n == 1) {
                    n = 2;
                    etc_event_phase = n;
                }
                step_mode++;
            }
        }
        break;
    case 2:
        if (pad_trig & 0x20) {
            for (i = 0; i < 6; i++) {
                buf[i] = 0x801E2000 + i * 0x4000;
            }
            bg_set(buf, 0, 6, 3, 2, 511, -160, -125, 13, 5);
            Bg = 0x01000000;
            n = 128;
            ((LIBGS_BG *)&Bg)[0].g = n & 0xFF;
            ((LIBGS_BG *)&Bg)[0].r = n & 0xFF;
            screen_bri = 128;
            step_mode++;
            if (0) {
            }
            ((LIBGS_BG *)&Bg)[0].b = n;
        }
        break;
    case 3:
        if (etc_model_roty >= 3072) {
            goto_sub_sub_step(0);
        } else {
            etc_model_roty += 8;
            etc_model_matrix_set();
        }
        break;
    default:
        return sel;
    }
}
