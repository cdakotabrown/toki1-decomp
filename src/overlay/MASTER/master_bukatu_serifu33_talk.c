/* @CompileFlags: -Wc,-pic0 */
/* @SymbolSize: 1388 */
/* @RodataBase: 0x8013C1BC */
/* @PostProc: remap_stack_slots --map '44:40' */
/* @PostProc: swap_instructions --offset1 0x3c8 --offset2 0x3cc */
/* @PostProc: patch_insn_reg_field 0x98:rt:t2:v1 0x9c:rs:t2:v1 0x9c:rt:t2:v1 0xa4:rs:t2:v1 0xac:rt:t2:v1 0x230:rt:t1:v1 0x234:rs:t1:v1 0x234:rt:t1:v1 0x23c:rs:t1:v1 0x27c:rt:t2:t1 0x280:rt:t2:t1 0x284:rs:t2:t1 0x28c:rt:t3:v0 0x294:rt:t3:v0 0x2a0:rt:t4:t2 0x2a4:rs:t4:t2 0x2a4:rt:t4:t2 0x2ac:rs:t4:t2 0x2d4:rt:t5:v1 0x2d8:rs:t5:v1 0x2d8:rt:t5:v1 0x2e0:rs:t5:v1 0x2e8:rd:t5:t3 0x2e8:rt:t5:v1 0x2f0:rt:t5:t3 0x2f4:rt:t5:t3 0x2fc:rs:t5:t3 0x304:rt:t9:t4 0x308:rs:t9:t4 0x308:rt:t9:t4 0x310:rs:t9:t4 0x314:rt:t8:t5 0x340:rt:t8:t5 0x34c:rt:t6:t9 0x358:rt:t6:t9 0x364:rt:t0:t8 0x370:rt:t0:t8 0x37c:rt:t7:t6 0x388:rt:t7:t6 0x394:rt:t1:t0 0x398:rs:t1:t0 0x398:rt:t1:t0 0x3a0:rs:t1:t0 0x3a4:rt:t6:t5 0x3a8:rt:t2:t7 0x3b0:rt:t2:t7 0x3b4:rt:t3:t1 0x3b8:rs:t3:t1 0x3b8:rt:t3:t1 0x3c0:rs:t3:t1 0x3c0:rt:t4:t2 0x3c4:rt:t4:t2 0x3c8:rt:t5:v0 0x3d0:rt:t5:v0 0x3dc:rt:t9:t3 0x3e4:rt:t9:t3 0x3ec:rt:t8:t4 0x3f4:rt:t8:t4 0x410:rt:t6:t5 0x41c:rt:t0:t9 0x428:rt:t0:t9 0x434:rt:t7:t8 0x438:rs:t7:t8 0x438:rt:t7:t8 0x440:rs:t7:t8 0x444:rt:t1:t6 0x470:rt:t1:t6 0x47c:rt:t2:t0 0x480:rs:t2:t0 0x480:rt:t2:t0 0x488:rs:t2:t0 0x48c:rt:t3:t7 0x4b8:rt:t3:t7 0x4c4:rt:t4:t1 0x4d0:rt:t4:t1 0x4dc:rt:t5:t2 0x4e8:rt:t5:t2 0x4f4:rt:t9:t3 0x500:rt:t9:t3 0x51c:rt:t8:t4 0x528:rt:t8:t4 */
/* master_bukatu_serifu33_talk - MASTER overlay, Address: 0x8013A784, Size: 1388 bytes (347 insns) */
#include "types.h"
#include "tokimeki/sprite.h"

typedef struct {
    s16 h[13];
} Table26;

extern struct {
    u8  pad0[3];
    u8  b33;
    u8  pad1[3];
    u8  b37;
    u8  pad2[14];
    s16 b46;
} cw96_active;
extern Table26 master_talk4_tbl;
extern u8 now_sub_sub_step;
extern u8 master_bukatu_girl_no;
extern u16 basyo_pack;
extern u8 girl4_basyo;
extern s16 now_serifu;
extern SPRITE_REC Sprites[];

extern void bustup_ani(void);
extern void zyotai_sprite_set_chk(void);
extern void bustup_face_change();
extern void cd_read_start2();
extern void indexed_byte_toggle();
extern s32 get_CD_State2(void);
extern void master_cw_init2(void);
extern void sub_sub_step_up(void);
extern void fade_in_step(void);
extern void fade_in(void);
extern void device_port_init();
extern void serifu_run(void);
extern void goto_sub_sub_step();
extern void bustup_kuchi_ani(void);
extern void master_log_girl_stat_rec_add(void);
extern void goto_sub_step();
extern void game_reset(void);
extern void common_disp(void);

void master_bukatu_serifu33_talk(void)
{
    s32 v1;
    Table26 buf;
    u32 v0;
    u32 nv;

    buf = master_talk4_tbl;
    bustup_ani();
    switch ((u32)now_sub_sub_step) {
    case 0:
        zyotai_sprite_set_chk();
        switch ((u32)master_bukatu_girl_no) {
        case 0:
            bustup_face_change(0);
            break;
        case 6:
            if (((u32)basyo_pack >> 12) != 9) {
                bustup_face_change(1);
            }
            break;
        case 7:
            bustup_face_change(1);
            break;
        case 9:
            bustup_face_change(5);
            Sprites[0].tpage = 7;
            Sprites[0].u = 112;
            break;
        case 12:
            bustup_face_change(0);
            break;
        }
        cd_read_start2(14, 0x80197000, 0xA9D0);
        indexed_byte_toggle(1);
        break;
    case 1:
        if (get_CD_State2() & 1) {
            master_cw_init2();
            v1 = master_bukatu_girl_no;
            if (v1 == 4 && ((u32)girl4_basyo >> 4) != ((u32)basyo_pack >> 12)) {
                now_serifu = 64;
            } else if (v1 == 6 && ((u32)basyo_pack >> 12) != 9) {
                now_serifu = 75;
            } else {
                now_serifu = buf.h[v1];
            }
            if (v1 == 9) {
                cw96_active.b33 |= 0x80;
                cw96_active.b46 = 8;
            }
            sub_sub_step_up();
        }
        break;
    case 2:
        if (master_bukatu_girl_no == 11) {
            fade_in_step();
        } else {
            fade_in();
        }
        v1 = master_bukatu_girl_no;
        if (v1 == 9) {
            v0 = cw96_active.b37 + 4;
            nv = v0 & 0xFF;
            cw96_active.b37 = (u8)nv;
            if (nv >= 125) {
                v0 = 128;
                cw96_active.b37 = (u8)v0;
            }
        }
        if (v1 != 11) {
            if (now_sub_sub_step == 3) {
                device_port_init(0, 0);
            }
        }
        break;
    case 3:
        serifu_run();
        break;
    case 4:
        switch ((u32)master_bukatu_girl_no) {
        case 0:
            if (now_serifu == 42) {
                bustup_face_change(1);
                bustup_kuchi_ani();
                goto_sub_sub_step(3);
            } else {
                now_serifu = 33;
                sub_sub_step_up();
            }
            break;
        case 1:
            now_serifu = 33;
            sub_sub_step_up();
            break;
        case 2:
            now_serifu = 33;
            sub_sub_step_up();
            break;
        case 3:
            now_serifu = 33;
            sub_sub_step_up();
            break;
        case 4:
            if (now_serifu == 63) {
                cw96_active.b46 = 5;
                cw96_active.b33 |= 0x80;
                cw96_active.b37 = 128;
                bustup_face_change(5);
                Sprites[0].tpage = 7;
                Sprites[0].u = 112;
                bustup_kuchi_ani();
                goto_sub_sub_step(3);
            } else {
                now_serifu = 33;
                sub_sub_step_up();
            }
            break;
        case 5:
            now_serifu = 33;
            sub_sub_step_up();
            break;
        case 6:
            if (now_serifu == 76) {
                bustup_face_change(0);
                bustup_kuchi_ani();
                goto_sub_sub_step(3);
            } else {
                now_serifu = 33;
                sub_sub_step_up();
            }
            break;
        case 7:
            if (now_serifu == 82) {
                bustup_face_change(0);
                bustup_kuchi_ani();
                goto_sub_sub_step(3);
            } else {
                now_serifu = 33;
                sub_sub_step_up();
            }
            break;
        case 8:
            now_serifu = 33;
            sub_sub_step_up();
            break;
        case 9:
            now_serifu = 33;
            sub_sub_step_up();
            break;
        case 10:
            now_serifu = 33;
            sub_sub_step_up();
            break;
        case 11:
            goto_sub_sub_step(6);
            break;
        case 12:
            now_serifu = 35;
            sub_sub_step_up();
            break;
        }
        break;
    case 5:
        serifu_run();
        break;
    case 6:
        master_log_girl_stat_rec_add();
        goto_sub_step(6);
        break;
    default:
        game_reset();
        break;
    }
    common_disp();
}
