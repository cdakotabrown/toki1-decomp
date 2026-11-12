/* @SymbolStart: 0x80140C6C */
/* @SymbolSize: 132 */
/* Address: 0x80140C74 (window starts at 0x80140C6C, the 2-word */
/* @CompileEnv: IDO52_COPYPROP_SITE=lod:1:deny,lod:4:deny,lod:5:deny,lod:6:deny,lod:7:deny,lod:8:deny,lod:9:deny */
#include "types.h"
#include "tokimeki/libgs_types.h"

extern void set_dec_bri(s32 a0);
extern u8 screen_bri;
extern LIBGS_BG Bg[];

s32 name_ent_bri_fade(void)
{
    s32 val;
    u8 v0;

    val = screen_bri;
    if ((u32)val >= 3) {
        val = val - 2;
        val = val & 0xFF;
        v0 = val;
        Bg[1].b = v0;
        Bg[1].g = v0;
        Bg[1].r = v0;
        Bg[0].b = v0;
        Bg[0].g = v0;
        Bg[0].r = v0;
        screen_bri = val;
        if ((u32)val < 128) {
            screen_bri = val & 0xFF;
            set_dec_bri(val);
        }
    }
}
