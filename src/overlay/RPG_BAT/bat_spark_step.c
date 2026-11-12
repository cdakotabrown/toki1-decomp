/* @SymbolSize: 828 */
/* @RodataBase: 0x8015DB80 */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=117:s0,115:s1,15:s2,59:s3,9:s1,77:a3,37:a2,40:v0,32:v0 IDO52_FORCE_WEB_SPILL=34 */
/* @CompileFlags: -Wo,-loopunroll,4 */
/* @PostProc: swap_instructions --offset1 0xf4 --offset2 0xf8 */
/* @PostProc: patch_insn_reg_field 0x150:rd:a2:v1 0x154:rs:a2:v1 0x188:rt:a3:t4 0x18c:rs:a3:t4 0x194:rt:a3:t4 0x198:rs:a3:t4 */
/* bat_spark_step - RPG_BAT overlay, Address: 0x80156064, Size: 828 bytes (207 insns) */
/* `@RodataBase: 0x8015DB80` (+2): the case-31..37 jr table; the target's */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"

extern s32 rpg_bat_anim_set2();
extern s32 rpg_bat_sprite_pos_set();
extern s32 get_rand_mod();
extern s32 rpg_bat_se_set2();
extern s32 rpg_bat_step_next();
extern s32 rpg_bat_state_next();

extern s32 rpg_bat_player_dmg[];
extern s16 cw147_scale_x;
extern s16 cw147_scale_y;
extern s16 cw147_x;
extern s16 cw147_y;
extern u8 cw4_active[];
extern u8 cw16_active[];
extern RPG_BAT_UNIT rpg_bat_unit0;

void bat_spark_step(void)
{
    s32 i;
    s32 j;
    s32 n;
    s32 v;
    s32 t;
    s32 xb;
    s32 *q;
    u8 *p;

    rpg_bat_se_set2(1295, 0, 1);
    switch (rpg_bat_unit0.sub) {
    case 0:
        q = rpg_bat_player_dmg;
        p = cw4_active;
        for (i = 0; i < 12; i++) {
            q[50 + i] = 0;
            p[6529 + i * 68] = 5;
        }
        rpg_bat_state_next();
        break;
    case 1:
        cw147_y = (s16)(cw147_y - 2);
        n = rpg_bat_unit0.cnt;
        v = n * 64 + 4096;
        cw147_scale_x = (s16)v;
        cw147_scale_y = (s16)v;
        rpg_bat_unit0.cnt = n + 1;
        q = rpg_bat_player_dmg;
        p = cw4_active;
        for (i = 4; i < 16; i++) {
            if (q[50] == 0) {
                if (get_rand_mod(45) == 0) {
                    q[50] = 1;
                    xb = get_rand_mod(128) + cw147_x - 64;
                    t = cw147_y - get_rand_mod(40);
                    rpg_bat_sprite_pos_set(i, xb + 160, t + 120);
                    rpg_bat_anim_set2(i, get_rand_mod(2) + 8, 1, 1);
                }
            }
            n = q[50];
            if (n != 0) {
                q[50] = n + 1;
                if (q[50] >= 31) {
                    switch (q[50]) {
                    case 31:
                        rpg_bat_anim_set2(i, 255, 1, 0);
                        break;
                    case 32:
                        rpg_bat_anim_set2(i, 255, 1, 1);
                        break;
                    case 33:
                        rpg_bat_anim_set2(i, 255, 1, 0);
                        break;
                    case 34:
                        rpg_bat_anim_set2(i, 255, 1, 1);
                        break;
                    case 35:
                        rpg_bat_anim_set2(i, 255, 1, 0);
                        break;
                    case 36:
                        rpg_bat_anim_set2(i, 255, 1, 1);
                        break;
                    case 37:
                        rpg_bat_anim_set2(i, 255, 1, 0);
                        break;
                    }
                }
            }
            *(s16 *)(p + 6570) = (s16)(*(s16 *)(p + 6570) + 1);
            v = rpg_bat_unit0.cnt * 64 + 4096;
            q += 1;
            p += 68;
            *(s16 *)(p + 6486) = (s16)v;
            *(s16 *)(p + 6488) = (s16)v;
        }
        if (cw147_y < -192) {
            rpg_bat_anim_set2(51, 255, 1, 0);
            p = cw4_active;
            for (i = 0; i < 12; i++) {
                *(s16 *)(p + 6554 + i * 68) = (s16)4096;
                *(s16 *)(p + 6556 + i * 68) = (s16)4096;
            }
            cw147_scale_x = (s16)4096;
            cw147_scale_y = (s16)4096;
            rpg_bat_step_next();
        }
        break;
    }
}
