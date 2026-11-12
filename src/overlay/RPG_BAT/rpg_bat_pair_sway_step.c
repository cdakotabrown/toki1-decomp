/* @SymbolSize: 1656 */
/* @CompileEnv: IDO52_CONST_INREG_VALUES=1:-1:9:9 IDO52_CONST_NOINREG_VALUES=1:48:48 IDO52_PRECOLOR_WEB=a0:2 */
/* @PostProc: swap_instructions --offset1 0x108 --offset2 0x10c */
/* @PostProc: swap_instructions --offset1 0x10c --offset2 0x110 */
/* @PostProc: patch_insn_reg_field 0x14:rt:a0:v0 0x18:rs:a0:v0 0x18:rt:a0:v0 0x2c:rs:a0:v0 0x38:rs:a0:v0 0x3c:rt:v0:v1 0x44:rs:a0:v0 0x48:rt:v0:v1 0x50:rs:a0:v0 0x54:rt:v0:v1 0x60:rt:v0:v1 0x64:rs:v0:v1 0x64:rt:v0:v1 0x6c:rs:v0:v1 0x78:rs:v0:v1 0x84:rs:v0:v1 0x90:rt:v0:v1 0xb4:rt:v0:v1 0xc0:rs:t8:t0 0xc0:rt:t0:t8 0xd0:rt:v0:v1 0xdc:rs:t9:t0 0xdc:rt:t0:t9 0xe0:rt:v0:v1 0xe8:rt:v0:v1 0xec:rt:v0:v1 0xf4:rd:v1:v0 0xf8:rs:v1:v0 0x104:rs:v1:v0 0x114:rt:v0:v1 0x1ac:rt:s3:s0 0x1b4:rs:s3:s0 0x1b4:rt:s3:s0 0x1b8:rd:s2:s3 0x1cc:rs:s3:s0 0x1d0:rs:s3:s0 0x1d4:rs:s3:s0 0x1d8:rs:s3:s0 0x1ec:rs:s2:s3 0x1ec:rt:s0:s1 0x1f4:rs:s0:s1 0x208:rs:s2:s3 0x208:rt:s1:s2 0x210:rs:s1:s2 0x214:rt:v0:v1 0x218:rs:v0:v1 0x218:rt:v0:v1 0x220:rs:v0:v1 0x22c:rs:s0:s1 0x238:rs:s0:s1 0x244:rs:s0:s1 0x250:rs:s0:s1 0x26c:rs:s1:s2 0x284:rs:s2:s3 0x284:rt:s2:s3 0x298:rs:s1:s2 0x2b0:rs:s2:s3 0x2b0:rt:s2:s3 0x2c4:rs:s1:s2 0x2dc:rs:s2:s3 0x2dc:rt:s2:s3 0x2f0:rs:s1:s2 0x304:rs:s2:s3 0x304:rt:s2:s3 0x308:rs:s2:s3 0x310:rs:s3:s0 0x310:rt:s3:s0 0x334:rt:s2:s3 0x344:rt:s2:s3 0x34c:rt:s2:s3 0x360:rd:v0:s0 0x36c:rs:v0:s0 0x370:rs:v0:s0 0x374:rs:v0:s0 0x378:rs:v0:s0 0x38c:rs:v0:s0 0x390:rs:v0:s0 0x394:rs:v0:s0 0x398:rs:v0:s0 0x3b8:rs:v0:v1 0x3b8:rt:v0:v1 0x3c0:rt:v0:v1 0x458:rt:v0:v1 0x45c:rs:v0:v1 0x45c:rt:v0:v1 0x464:rs:v0:v1 0x464:rt:v0:v1 0x468:rs:v0:v1 0x474:rs:v0:v1 0x474:rt:v0:v1 0x47c:rt:v0:v1 0x480:rs:v0:v1 0x4a4:rs:v0:v1 0x4a4:rt:v0:v1 0x4ac:rt:v0:v1 0x544:rt:v0:v1 0x548:rs:v0:v1 0x548:rt:v0:v1 0x550:rs:v0:v1 0x550:rt:v0:v1 0x554:rs:v0:v1 0x560:rs:v0:v1 0x560:rt:v0:v1 0x568:rt:v0:v1 0x56c:rs:v0:v1 0x590:rs:v0:v1 0x590:rt:v0:v1 0x598:rt:v0:v1 0x630:rt:v0:v1 0x634:rs:v0:v1 0x634:rt:v0:v1 0x63c:rs:v0:v1 0x63c:rt:v0:v1 0x640:rs:v0:v1 0x64c:rs:v0:v1 0x64c:rt:v0:v1 0x654:rt:v0:v1 0x658:rs:v0:v1 */
/* rpg_bat_pair_sway_step - RPG_BAT overlay, Address: 0x80154760, Size: 1656 bytes */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"

extern s32 rpg_bat_target_no;
extern s32 rpg_bat_toriko_flag;
extern s32 rpg_bat_toriko_flag_1;
extern s32 rpg_bat_toriko_flag_2;
extern s32 rpg_bat_seq_msg_line_cnt;
extern s32 rpg_bat_pair_sel;
extern s32 rpg_bat_res_pal_4;
extern s32 rpg_bat_res_pal_5;
extern s32 rpg_bat_target_x;
extern s32 rpg_bat_target_y;
extern u8 class_w[];
extern RPG_BAT_UNIT rpg_bat_unit0;
extern RPG_BAT_UNIT rpg_bat_unit1;

extern void palette_upload_enqueue(s32, s32, s32, s32, s32);
extern s32 rsin(s32);
extern void rpg_bat_anim_set2(s32, s32, s32, s32);
extern void rpg_bat_sprite_pos_set(s32, s32, s32);
extern void rpg_bat_se_set2(s32, s32, s32);
extern void rpg_bat_step_next(void);
extern void rpg_bat_state_next(void);

void rpg_bat_pair_sway_step(void)
{
    s32 idx;
    s32 i;
    s32 mask;
    s32 amp;
    u8 *p;

    switch (rpg_bat_unit0.sub) {
    case 0:
        switch (rpg_bat_target_no) {
        case 0:
            if (rpg_bat_toriko_flag == 1) {
                rpg_bat_seq_msg_line_cnt = 1;
            }
            idx = 59;
            break;
        case 1:
            if (rpg_bat_toriko_flag_1 == 1) {
                rpg_bat_seq_msg_line_cnt = 1;
            }
            idx = 60;
            break;
        case 2:
            if (rpg_bat_toriko_flag_2 == 1) {
                rpg_bat_seq_msg_line_cnt = 1;
            }
            idx = 61;
            break;
        default:
            idx = 59;
            break;
        }
        p = class_w + idx * 68;
        p[6531] ^= 0x80;
        rpg_bat_pair_sel = idx;
        palette_upload_enqueue(rpg_bat_res_pal_4, 24, 1, 3, 10);
        palette_upload_enqueue(rpg_bat_res_pal_5, 25, 1, 3, 1);
        palette_upload_enqueue(rpg_bat_res_pal_5, 26, 1, 3, 0);
        class_w[9315] = 24;
        class_w[9519] = 24;
        class_w[9383] = 25;
        class_w[9587] = 25;
        class_w[9451] = 26;
        class_w[9655] = 26;
        p = class_w;
        for (i = 0; i < 3; i++) {
            s32 a;
            s32 b;
            *(s32 *)(p + 9304) = 0x51000000;
            *(s32 *)(p + 9508) = 0x41000000;
            p[9255] = 128;
            p[9459] = 255;
            a = i + 40;
            rpg_bat_sprite_pos_set(a, rpg_bat_target_x, rpg_bat_target_y);
            b = i + 43;
            rpg_bat_sprite_pos_set(b, rpg_bat_target_x, rpg_bat_target_y);
            mask = rpg_bat_unit1.flag & 0xFFFF0000;
            switch (mask) {
            case 0x00100000:
                rpg_bat_anim_set2(a, 5, 1, 1);
                rpg_bat_anim_set2(b, 5, 1, 1);
                break;
            case 0x00200000:
                rpg_bat_anim_set2(a, 6, 1, 1);
                rpg_bat_anim_set2(b, 6, 1, 1);
                break;
            case 0x00400000:
                rpg_bat_anim_set2(a, 7, 1, 1);
                rpg_bat_anim_set2(b, 7, 1, 1);
                break;
            case 0x08000000:
            default:
                rpg_bat_anim_set2(a, 8, 1, 1);
                rpg_bat_anim_set2(b, 8, 1, 1);
                break;
            }
            p += 68;
        }
        if (rpg_bat_seq_msg_line_cnt == 1U) {
            for (i = 0; i < 6; i++) {
                class_w[i * 68 + 9251] |= 1;
            }
        }
        rpg_bat_se_set2(1291, 1, 0);
        rpg_bat_state_next();
        break;
    case 1:
        amp = rsin((rpg_bat_unit0.wait << 11) / 1000);
        amp = (amp << 3) >> 12;
        if (amp >= 8) {
            amp = 7;
        }
        rpg_bat_sprite_pos_set(40, rpg_bat_target_x - amp, rpg_bat_target_y);
        rpg_bat_sprite_pos_set(43, rpg_bat_target_x - amp, rpg_bat_target_y);
        rpg_bat_sprite_pos_set(42, rpg_bat_target_x + amp, rpg_bat_target_y);
        rpg_bat_sprite_pos_set(45, rpg_bat_target_x + amp, rpg_bat_target_y);
        rpg_bat_unit0.wait += 10;
        if (rpg_bat_unit0.wait >= 501) {
            rpg_bat_unit0.wait += 10;
        }
        if (rpg_bat_unit0.wait >= 1001) {
            rpg_bat_se_set2(1291, 1, 0);
            rpg_bat_state_next();
        }
        break;
    case 2:
        amp = rsin((rpg_bat_unit0.wait << 11) / 1000);
        amp = (amp << 4) >> 12;
        if (amp >= 16) {
            amp = 15;
        }
        rpg_bat_sprite_pos_set(40, rpg_bat_target_x + amp, rpg_bat_target_y);
        rpg_bat_sprite_pos_set(43, rpg_bat_target_x + amp, rpg_bat_target_y);
        rpg_bat_sprite_pos_set(42, rpg_bat_target_x - amp, rpg_bat_target_y);
        rpg_bat_sprite_pos_set(45, rpg_bat_target_x - amp, rpg_bat_target_y);
        rpg_bat_unit0.wait += 10;
        if (rpg_bat_unit0.wait >= 501) {
            rpg_bat_unit0.wait += 10;
        }
        if (rpg_bat_unit0.wait >= 1001) {
            rpg_bat_se_set2(1291, 1, 0);
            rpg_bat_state_next();
        }
        break;
    case 3:
        amp = rsin((rpg_bat_unit0.wait << 11) / 1000);
        amp = (amp << 5) >> 12;
        if (amp >= 32) {
            amp = 31;
        }
        rpg_bat_sprite_pos_set(40, rpg_bat_target_x - amp, rpg_bat_target_y);
        rpg_bat_sprite_pos_set(43, rpg_bat_target_x - amp, rpg_bat_target_y);
        rpg_bat_sprite_pos_set(42, rpg_bat_target_x + amp, rpg_bat_target_y);
        rpg_bat_sprite_pos_set(45, rpg_bat_target_x + amp, rpg_bat_target_y);
        rpg_bat_unit0.wait += 10;
        if (rpg_bat_unit0.wait >= 501) {
            rpg_bat_unit0.wait += 10;
        }
        if (rpg_bat_unit0.wait >= 1001) {
            rpg_bat_step_next();
        }
        break;
    default:
        break;
    }
}
