/* @SymbolStart: 0x8014484C */
/* @SymbolSize: 212 */
/* Address: 0x80144854 */
/* Size: 204 bytes (51 insns) */
#include "types.h"
#include "tokimeki/class_w.h"

extern s32 rand();

extern u8 cw97_prio;
extern u8 cw97_mode;
extern u8 cw97_disp;
extern u8 cw97_sel;
extern s32 cw97_attr;
extern s32 cw97_hdr;
extern s32 cw97_img;
extern s32 cw97_anim_tbl;
extern s16 cw97_anim;
extern s16 cw97_x;
extern s16 cw97_y;
extern u8 cw97_mode;
extern CLASS_W class_w[];

void name_ent_cw97_anim_start(void)
{
    s32 result;

    if ((cw97_mode & 0x0001) != 0) {
        return;
    }

    result = rand();
    if ((result & 0x7F) != 127) {
        return;
    }

    cw97_prio = 4;
    cw97_mode = 3;
    cw97_attr = 0x41000000;
    cw97_disp = 164;
    cw97_img = 0x801B27B0;
    cw97_anim_tbl = 0x801B27D8;
    cw97_hdr = 0x801B27AC;
    cw97_anim = 0;
    class_w[97].cel_mode = 8;
    cw97_sel = 128;
    cw97_x = -160;
    cw97_y = -120;
}
