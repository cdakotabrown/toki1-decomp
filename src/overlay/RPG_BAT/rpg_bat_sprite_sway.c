/* rpg_bat_sprite_sway - RPG_BAT overlay, Address: 0x801328C4, Size: 464 bytes */
/* @CompileEnv: IDO52_FORM_SITE=lda:0xffffffbc:3:0 IDO52_FORCE_WEB_COLOR=4:s1,64:s2,10:s3 */
/* @CompileFlags: -Wo,-loopunroll,0 -Wc,-pic0 */
/* @PostProc: patch_insn_reg_field 0x15c:rs:t8:s5 0x15c:rt:s5:t8 */
#include "types.h"

extern s32 rsin();
extern s32 rpg_bat_anim_set2();
extern s32 rpg_bat_sprite_pos_set();

typedef struct { s32 w[16]; } Blk64;
extern Blk64 rpg_bat_sprite_sway_tbl;
extern volatile s32 rpg_bat_unit0_cnt;

void rpg_bat_sprite_sway(s32 arg0, s32 arg1)
{
    s32 x;
    Blk64 buf;
    s32 *p;
    s32 divisor;
    s32 i;
    s32 frame;
    s32 z;
    s32 y;

    buf = rpg_bat_sprite_sway_tbl;

    i = 0;
    p = buf.w;
    divisor = 1000;
    for (; i < 8; i++) {
        frame = rpg_bat_unit0_cnt;
        z = rsin((frame << 11) / divisor);
        frame = rpg_bat_unit0_cnt;
        x = (p[0] * frame) / divisor + arg0;
        y = z >> 7;
        rpg_bat_sprite_pos_set(i + 40, x, (arg1 + (p[1] * frame) / divisor) - y);
        p += 2;
    }

    frame = rpg_bat_unit0_cnt + 25;
    rpg_bat_unit0_cnt = frame;
    if (frame < 1001) {
        return;
    }
    for (i = 0; i < 8; i++) {
        rpg_bat_anim_set2(i + 40, 9, 1, 0);
    }
}
