/* @SymbolStart: 0x8013F82C */
/* @CompileFlags: -Wc,-pic1 */
/* @RodataBase: 0x8015BFB4 */
/* @SymbolSize: 1376 */
/* @PostProc: patch_stack_complete --old 120 --new 96 */
/* @PostProc: patch_insn_reg_field 0xbc:rt:v0:t8 0xc0:rs:v0:t8 0xc0:rt:v0:t8 0xc4:rt:t8:t3 0xc8:rs:t8:t3 0xc8:rt:t8:t3 0xd0:rs:v0:t8 0xd0:rt:v0:t9 0xd4:rt:v0:t9 0xdc:rs:t8:t3 0xdc:rt:t9:t4 0xe0:rt:t9:t4 0xe4:rs:v0:t9 0xe4:rt:t3:t5 0xe8:rs:t3:t5 0xf0:rd:t3:t5 0xf0:rt:t3:t5 0xf8:rt:t3:t5 0xfc:rt:t3:t5 0x104:rs:t3:t5 0x114:rt:t4:t6 0x118:rt:t4:t6 0x12c:rt:t5:t7 0x138:rt:t5:t7 0x144:rt:t6:t8 0x148:rt:t6:t8 0x16c:rt:t7:t3 0x170:rt:t7:t3 0x194:rt:t8:t4 0x198:rt:t8:t4 0x1e4:rt:t3:t5 0x1e8:rt:t3:t5 0x208:rt:t4:t6 0x20c:rs:t4:t6 0x20c:rt:t4:t6 0x210:rd:t5:t7 0x214:rs:t4:t6 0x21c:rt:t6:t8 0x220:rd:t6:t8 0x220:rs:t6:t8 0x220:rt:t5:t7 0x224:rs:t6:t8 0x224:rt:t6:t8 0x22c:rs:t6:t8 0x284:rt:t7:t3 0x288:rs:t7:t3 0x288:rt:t7:t3 0x290:rs:t7:t3 0x2a4:rt:t8:t4 0x2ac:rs:t8:t4 0x2b8:rt:t4:t6 0x2bc:rs:t4:t6 0x2bc:rt:t4:t6 0x2c0:rd:t3:t5 0x2c4:rs:t3:t5 0x2c4:rt:t4:t6 0x2e8:rt:t5:t7 0x2ec:rs:t5:t7 0x2ec:rt:t5:t7 0x2f0:rt:t6:t8 0x2f4:rs:t5:t7 0x300:rs:t6:t8 0x300:rt:t6:t8 0x308:rs:t6:t8 0x308:rt:t7:t3 0x30c:rt:t7:t3 0x338:rt:t8:t4 0x340:rt:t8:t4 0x360:rt:t3:t5 0x364:rt:t7:t3 0x368:rt:t7:t3 0x36c:rt:t3:t5 0x384:rd:t4:t6 0x388:rs:t4:t0 0x388:rt:t0:t6 0x38c:rd:t5:t7 0x390:rd:t6:t8 0x390:rs:t5:t1 0x390:rt:t1:t7 0x398:rt:t6:t8 0x3b4:rt:t8:t4 0x3b8:rs:t8:t4 0x3b8:rt:t8:t4 0x3c0:rs:t8:t4 0x3d4:rt:t3:t5 0x3d8:rs:t3:t5 0x3d8:rt:t3:t5 0x3e0:rs:t3:t5 0x458:rt:t4:t6 0x45c:rs:t4:t6 0x45c:rt:t4:t6 0x464:rs:t4:t6 0x464:rt:t5:t7 0x468:rt:t5:t7 0x480:rt:t7:t3 0x484:rs:t7:t3 0x484:rt:t7:t3 0x488:rt:t6:t8 0x490:rt:t8:t4 0x494:rt:t6:t8 0x498:rs:t8:t4 0x498:rt:t7:t3 0x4b8:rt:t6:t8 0x4bc:rt:t3:t5 0x4c0:rt:t3:t5 0x4c4:rt:t6:t8 0x4e0:rs:t9:t1 0x4e0:rt:t1:t9 0x4e4:rd:t4:t6 0x4e8:rd:t5:t7 0x4e8:rs:t4:t2 0x4e8:rt:t2:t6 0x4f0:rt:t5:t7 0x50c:rt:t8:t4 0x510:rs:t8:t4 0x510:rt:t8:t4 0x518:rs:t8:t4 */
/* rpg_bat_box_step - RPG_BAT overlay, Address: 0x8013F834, two-level jr-table dispatcher */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"
#include "tokimeki/class_w.h"


extern s32 sub_step_up();
extern s32 palette_upload_enqueue();
extern s32 dtd_on_tpage();
extern s32 _sprite_set_box_shade();
extern s32 k_disp_switch();
extern s32 strcpy();
extern s32 rpg_bat_effect_rec_set();
extern s32 rpg_bat_anim_set();
extern s32 rpg_bat_anim_set2();
extern s32 rpg_bat_sprite_pos_set();
extern s32 get_rand_mod();
extern s32 rpg_bat_se_set();
extern s32 rpg_bat_msg_set();
extern s32 rpg_bat_seq_step_next();
extern s32 rpg_bat_seq_wait();
extern s32 rpg_bat_seq_step_gate();

extern s32 rpg_bat_box_step_str0;
extern s32 rpg_bat_msg_buf2;
extern s32 rpg_bat_msg_tbl2;
extern s32 rpg_bat_seq_msg_line_cnt;
extern s32 rpg_bat_res_pal_6;
extern s32 rpg_bat_seq_work[];
extern s32 rpg_bat_target_no[];
extern u8  class_w[];
extern RPG_BAT_UNIT rpg_bat_unit0;
extern RPG_BAT_UNIT rpg_bat_unit1;
extern RPG_BAT_UNIT rpg_bat_unit3;

void rpg_bat_box_step(void)
{
    s32 old0, old1, newv;
    s32 s0, s1, tmp2;
    s32 val, base, color;
    u8 *pb;

    switch ((u32)rpg_bat_unit1.step) {
    case 0:
        strcpy(&rpg_bat_msg_buf2, &rpg_bat_box_step_str0);
        rpg_bat_msg_set(&rpg_bat_msg_tbl2, 1, 90);
        rpg_bat_effect_rec_set(4);
        rpg_bat_unit0.flag = 4096;
        rpg_bat_se_set(1296, 1, 0);
        rpg_bat_seq_step_next();
        break;
    case 1:
        rpg_bat_seq_step_gate(1);
        break;
    case 2:
        old0 = rpg_bat_unit1.cnt;
        old1 = rpg_bat_seq_msg_line_cnt;
        newv = old0 + 1;
        rpg_bat_unit1.cnt = newv;
        rpg_bat_seq_msg_line_cnt = old1 + 1;
        switch ((u32)(newv - 1)) {
        case 0:
            palette_upload_enqueue(rpg_bat_res_pal_6, 19, 1, 4, 10);
            ((CLASS_W *)class_w)[155].clut = 19;
            break;
        case 9:
            palette_upload_enqueue(rpg_bat_res_pal_6, 19, 1, 4, 9);
            break;
        case 19:
            palette_upload_enqueue(rpg_bat_res_pal_6, 19, 1, 4, 8);
            break;
        case 29:
            palette_upload_enqueue(rpg_bat_res_pal_6, 19, 1, 4, 7);
            break;
        case 39:
            palette_upload_enqueue(rpg_bat_res_pal_6, 19, 1, 4, 6);
            break;
        case 49:
            palette_upload_enqueue(rpg_bat_res_pal_6, 19, 1, 4, 5);
            break;
        }
        pb = class_w;
        for (s0 = 0; s0 < 8; s0++) {
            if (rpg_bat_seq_msg_line_cnt >= 2 && rpg_bat_seq_work[s0] == 0 &&
                get_rand_mod(5) == 0) {
                rpg_bat_seq_msg_line_cnt = 0;
                s1 = get_rand_mod(64);
                tmp2 = get_rand_mod(64);
                rpg_bat_sprite_pos_set(s0 + 5, s1 + 40, tmp2 + 64);
                rpg_bat_anim_set2(s0 + 5, 23, 5, 1);
                if (rpg_bat_unit1.wait < 176) {
                    rpg_bat_se_set(1293, 2, 1);
                }
            }
            if ((pb[6870] & 0x1) == 0) {
                rpg_bat_anim_set2(s0 + 5, 255, 1, 0);
                (&rpg_bat_target_no[s0])[50] = 0;
            }
            pb += 68;
        }
        if (rpg_bat_unit1.cnt >= 61) {
            rpg_bat_unit1.wait = rpg_bat_unit1.wait + 2;
            rpg_bat_sprite_pos_set(4, 0, 0);
            rpg_bat_anim_set(4, 24, 1, 1);
            val = rpg_bat_unit1.wait;
            ((CLASS_W *)class_w)[100].frame = 5;
            ((CLASS_W *)class_w)[100].scale_x = (s16)(val << 4);
            ((CLASS_W *)class_w)[100].scale_y = (s16)(val << 4);
            color = val / 3;
            color = color + (color << 8);
            color = color + (color << 8);
            _sprite_set_box_shade(-160, -120, 320, 168, 2, color, 129);
            dtd_on_tpage(0, 0, 2, 1, 0);
        }
        if (rpg_bat_unit1.wait == 176) {
            rpg_bat_se_set(1292, 1, 0);
        }
        if (rpg_bat_unit1.wait >= 337) {
            for (s0 = 0; s0 != 8; s0++) {
                rpg_bat_anim_set2(s0 + 5, 255, 1, 0);
                rpg_bat_anim_set2(59, 255, 1, 0);
                ((CLASS_W *)class_w)[100].frame = 5;
            }
            k_disp_switch(rpg_bat_unit1.disp, 0);
            k_disp_switch(rpg_bat_unit3.disp, 0);
            rpg_bat_seq_step_next();
        }
        break;
    case 3:
        rpg_bat_unit1.wait = rpg_bat_unit1.wait + 4;
        rpg_bat_anim_set(4, 24, 1, 1);
        base = 336 - rpg_bat_unit1.wait;
        ((CLASS_W *)class_w)[100].frame = 5;
        ((CLASS_W *)class_w)[100].scale_x = (s16)(base << 4);
        ((CLASS_W *)class_w)[100].scale_y = (s16)(base << 4);
        color = base / 3;
        color = color + (color << 8);
        color = color + (color << 8);
        _sprite_set_box_shade(-160, -120, 320, 168, 2, color, 129);
        dtd_on_tpage(0, 0, 2, 1, 0);
        if (rpg_bat_unit1.wait >= 329) {
            rpg_bat_anim_set2(4, 255, 1, 0);
            rpg_bat_seq_step_next();
        }
        break;
    case 4:
        rpg_bat_seq_wait(60);
        break;
    case 5:
        sub_step_up();
        break;
    }
}
