/* @CompileFlags: -Wc,-pic0 */
/* @SymbolSize: 1308 */
/* @RodataBase: 0x8013C240 */
/* @PostProc: patch_insn_reg_field 0x110:rs:t5:t4 0x110:rt:t5:t4 0xa0:rt:t2:v1 0xa4:rs:t2:v1 0xa4:rt:t2:v1 0xac:rs:t2:v1 0xe8:rt:t3:t2 0xec:rs:t3:t2 0xfc:rt:v0:v1 0x100:rs:v0:v1 0x100:rt:v0:v1 0x108:rd:t4:t3 0x108:rt:v0:v1 0x10c:rd:t5:t4 0x10c:rt:t4:t3 0x114:rt:t9:t5 0x118:rt:t5:t4 0x128:rs:v0:v1 0x12c:rt:t6:t8 0x130:rs:t9:t5 0x130:rt:t9:t5 0x138:rs:t9:t5 0x138:rt:t8:t9 0x13c:rt:t8:t9 0x144:rt:t6:t8 0x14c:rs:v0:v1 0x150:rt:t7:t6 0x154:rs:t7:t6 0x154:rt:t7:t6 0x15c:rs:t7:t6 0x15c:rt:t0:t7 0x160:rt:t0:t7 0x168:rt:t1:t0 0x16c:rt:t1:t0 0x180:rt:t2:v1 0x184:rs:t2:v1 0x184:rt:t2:v1 0x18c:rs:t2:v1 0x1ac:rt:v0:v1 0x1b0:rs:v0:v1 0x1b0:rt:v0:v1 0x1b8:rs:v0:v1 0x1bc:rt:a1:v0 0x1c4:rs:v0:v1 0x1cc:rs:a1:v0 0x1cc:rt:v1:v0 0x1d4:rs:v1:v0 0x1dc:rs:v1:v0 0x1dc:rt:a1:t1 0x1e0:rt:a1:v0 0x1ec:rt:a1:v0 0x1f4:rt:a1:t1 0x1fc:rs:v0:v1 0x200:rt:t3:t2 0x204:rs:t3:t2 0x204:rt:t3:t2 0x20c:rs:t3:t2 0x234:rt:t4:v1 0x238:rs:t4:v1 0x238:rt:t4:v1 0x240:rs:t4:v1 0x248:rd:t4:t3 0x248:rt:t4:v1 0x250:rt:t4:t3 0x254:rt:t4:t3 0x25c:rs:t4:t3 0x264:rt:t5:t4 0x268:rs:t5:t4 0x268:rt:t5:t4 0x270:rs:t5:t4 0x274:rt:t7:t6 0x278:rt:t9:t5 0x280:rt:t9:t5 0x284:rt:t8:t9 0x288:rs:t8:t9 0x288:rt:t8:t9 0x290:rs:t8:t9 0x290:rt:t6:t8 0x294:rt:t6:t8 0x298:rt:a1:v0 0x2a0:rt:a1:v0 0x2bc:rt:t7:t6 0x2c8:rt:t0:t7 0x2cc:rs:t0:t7 0x2cc:rt:t0:t7 0x2d4:rs:t0:t7 0x2d8:rt:t4:t3 0x2dc:rt:t1:t0 0x2e4:rt:t1:t0 0x2e8:rt:t2:t1 0x2ec:rs:t2:t1 0x2ec:rt:t2:t1 0x2f4:rs:t2:t1 0x2f4:rt:t3:t2 0x2f8:rt:t3:t2 0x2fc:rt:a1:v0 0x304:rt:a1:v0 0x320:rt:t4:t3 0x32c:rt:t5:t4 0x338:rt:t5:t4 0x344:rt:t9:t5 0x350:rt:t9:t5 0x35c:rt:t8:t9 0x360:rs:t8:t9 0x360:rt:t8:t9 0x368:rs:t8:t9 0x36c:rt:t1:t0 0x370:rt:t6:t8 0x378:rt:t6:t8 0x37c:rt:t7:t6 0x380:rs:t7:t6 0x380:rt:t7:t6 0x388:rs:t7:t6 0x388:rt:t0:t7 0x38c:rt:t0:t7 0x390:rt:a1:v0 0x398:rt:a1:v0 0x3b4:rt:t1:t0 0x3c0:rt:t2:t1 0x3c4:rs:t2:t1 0x3c4:rt:t2:t1 0x3cc:rs:t2:t1 0x3d0:rt:t3:t2 0x3fc:rt:t3:t2 0x408:rt:t4:t3 0x40c:rs:t4:t3 0x40c:rt:t4:t3 0x414:rs:t4:t3 0x418:rt:t6:t8 0x41c:rt:t5:t4 0x420:rs:t5:t4 0x420:rt:t5:t4 0x428:rs:t5:t4 0x428:rt:t9:t5 0x42c:rt:t9:t5 0x434:rt:t8:t9 0x438:rt:t8:t9 0x43c:rt:a1:v0 0x444:rt:a1:v0 0x460:rt:t6:t8 0x46c:rt:t7:t6 0x478:rt:t7:t6 0x484:rt:t0:t7 0x490:rt:t0:t7 0x49c:rt:t1:t0 0x4a8:rt:t1:t0 0x4b4:rt:t2:t1 0x4c0:rt:t2:t1 0x4dc:rt:t3:t2 0x4e8:rt:t3:t2 */
/* master_bukatu_serifu36_talk - MASTER overlay, Address: 0x8013ACF0, Size: 1308 bytes (327 insns) */
#include "types.h"

typedef struct {
    s16 h[13];
} Table26;

extern Table26 master_talk5_tbl;

extern struct {
    u8  pad0[3];
    u8  b33;
    u8  pad1[3];
    u8  pad37;
    u8  pad2[14];
    s16 b46;
} cw96_active;

extern u8 now_sub_sub_step;
extern u8 master_bukatu_girl_no;
extern s16 now_serifu;
extern s16 now_serifu_in;
extern u8 cal_cw_bright;

extern void bustup_ani(void);
extern void zyotai_sprite_set_chk(void);
extern void bustup_face_change();
extern void indexed_byte_toggle();
extern void cd_read_start2();
extern s32 get_CD_State2(void);
extern void master_cw_init2(void);
extern void sub_sub_step_up(void);
extern void fade_in_step(void);
extern void fade_in(void);
extern void device_port_init();
extern void serifu_run(void);
extern void goto_sub_sub_step();
extern void bustup_kuchi_ani(void);
extern void master_log_add(void);
extern void sub_step_up(void);
extern void game_reset(void);
extern void common_disp(void);

void master_bukatu_serifu36_talk(void)
{
    s32 pad;
    Table26 buf;
    s32 v1;
    u32 v0;

    buf = master_talk5_tbl;
    bustup_ani();
    switch ((u32)now_sub_sub_step) {
    case 0:
        zyotai_sprite_set_chk();
        bustup_face_change(5);
        if (master_bukatu_girl_no == 12) {
            bustup_face_change(0);
        }
        indexed_byte_toggle(1);
        cd_read_start2(14, 0x80197000, 0xA9D0);
        break;
    case 1:
        if (get_CD_State2() & 1) {
            master_cw_init2();
            v1 = master_bukatu_girl_no;
            now_serifu = buf.h[v1];
            now_serifu_in = 0;
            if (v1 == 9) {
                cw96_active.b33 |= 0x80;
                cw96_active.b46 = 9;
            }
            if (v1 == 10) {
                cw96_active.b33 |= 0x80;
                cw96_active.b46 = 11;
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
        if (v1 == 10 || v1 == 9) {
            v0 = cal_cw_bright;
            if (v0 >= 129) {
                v0 = 128;
                cal_cw_bright = (u8)v0;
            } else {
                v0 = v0 + 4;
                cal_cw_bright = (u8)v0;
            }
        }
        if (v1 != 11) {
            if (now_sub_sub_step == 3) {
                device_port_init(0, 0);
            }
        }
        break;
    case 3:
    case 5:
        serifu_run();
        break;
    case 4:
        switch ((u32)master_bukatu_girl_no) {
        case 0:
            if (now_serifu == 44) {
                cw96_active.b46 = 1;
                cw96_active.b33 |= 0x80;
                cal_cw_bright = 128;
                goto_sub_sub_step(3);
            } else {
                now_serifu = 36;
                sub_sub_step_up();
            }
            break;
        case 1:
            if (now_serifu == 49) {
                cw96_active.b46 = 3;
                cw96_active.b33 |= 0x80;
                cal_cw_bright = 128;
                goto_sub_sub_step(3);
            } else {
                now_serifu = 36;
                sub_sub_step_up();
            }
            break;
        case 2:
            now_serifu = 36;
            sub_sub_step_up();
            break;
        case 3:
            now_serifu = 36;
            sub_sub_step_up();
            break;
        case 4:
            if (now_serifu == 66) {
                cw96_active.b46 = 5;
                cw96_active.b33 |= 0x80;
                cal_cw_bright = 128;
                goto_sub_sub_step(3);
            } else {
                now_serifu = 36;
                sub_sub_step_up();
            }
            break;
        case 5:
            if (now_serifu == 71) {
                bustup_face_change(2);
                bustup_kuchi_ani();
                goto_sub_sub_step(3);
            } else {
                now_serifu = 36;
                sub_sub_step_up();
            }
            break;
        case 6:
            if (now_serifu == 78) {
                cw96_active.b33 |= 0x80;
                cw96_active.b46 = 7;
                cal_cw_bright = 128;
                goto_sub_sub_step(3);
            } else {
                now_serifu = 36;
                sub_sub_step_up();
            }
            break;
        case 7:
            now_serifu = 36;
            sub_sub_step_up();
            break;
        case 8:
            now_serifu = 36;
            sub_sub_step_up();
            break;
        case 9:
            now_serifu = 36;
            sub_sub_step_up();
            break;
        case 10:
            now_serifu = 36;
            sub_sub_step_up();
            break;
        case 11:
            goto_sub_sub_step(6);
            break;
        case 12:
            now_serifu = 38;
            sub_sub_step_up();
            break;
        }
        break;
    case 6:
        master_log_add();
        sub_step_up();
        break;
    default:
        game_reset();
        break;
    }
    common_disp();
}
