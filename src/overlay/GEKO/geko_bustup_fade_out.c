/* @CompileEnv: IDO52_FORCE_WEB_COLOR=49:v1,48:a0 */
/* geko_bustup_fade_out -- GEKO overlay, Address: 0x80141F48, Size: 204 bytes (51 insns) */
#include "types.h"

typedef struct {
    u8 flag;
    u8 pad1[3];
    u8 voice;
    u8 pad2[63];
} GkRec;

extern void date_fade_out(void);
extern void sprite_rgb_set(s32 a0, s32 a1);

extern volatile u8 cal_cw_bright;
extern s32 bustup_on;
extern GkRec bustup_cw_disp[];
extern s32 Sprites[];

s32 geko_bustup_fade_out(void)
{
    u32 voice;
    s32 r;

    date_fade_out();
    voice = cal_cw_bright;
    bustup_on = 1;
    bustup_cw_disp[1].voice = voice;
    bustup_cw_disp[0].voice = voice;
    sprite_rgb_set(0, voice);
    sprite_rgb_set(1, cal_cw_bright);
    if ((u32)cal_cw_bright < 8U) {
        r = -129;
        bustup_on = 0;
        bustup_cw_disp[1].flag = bustup_cw_disp[1].flag & r;
        bustup_cw_disp[0].flag = bustup_cw_disp[0].flag & r;
        Sprites[0] = Sprites[0] | 0x80000000;
        Sprites[9] = Sprites[9] | 0x80000000;
        return r;
    }
}
