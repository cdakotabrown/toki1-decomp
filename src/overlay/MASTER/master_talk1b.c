/* @RodataBase: 0x8013BE94 */
/* @CompileFlags: -cckr */
#include "types.h"
#include "tokimeki/class_w.h"
#include "tokimeki/sprite.h"

typedef struct { s16 h[13]; } Blk26;

typedef struct {
    u8  unk0[3];
    u8  flags;
    u8  unk4[3];
    u8  vol;
    u8  unk8[14];
    s16 kind;
} Rec;

extern Blk26 master_talk1b_tbl;
extern u8 now_sub_sub_step;
extern u8 master_talk_girl_no;
extern s16 now_serifu;
extern u8 girl4_basyo;
extern u16 basyo_pack;
extern SPRITE_REC Sprites[];
extern CLASS_W class_w[];

void master_talk1b()
{
    s32 i;
    Blk26 buf;

    buf = master_talk1b_tbl;
    bustup_ani();

    switch (now_sub_sub_step) {
    case 0:
        zyotai_sprite_set_chk();
        switch (master_talk_girl_no) {
        case 0:
            bustup_face_change(0);
            break;
        case 6:
            bustup_face_change(1);
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
            master_cw_init();
            if (master_talk_girl_no == 4 &&
                (girl4_basyo >> 4) != (basyo_pack >> 12)) {
                now_serifu = 64;
            } else if (master_talk_girl_no == 6) {
                now_serifu = 75;
            } else {
                now_serifu = buf.h[master_talk_girl_no];
            }
            if (master_talk_girl_no == 9) {
                (*(Rec *)&class_w[96]).flags |= 0x80;
                (*(Rec *)&class_w[96]).kind = 8;
            }
            sub_sub_step_up();
        }
        break;

    case 2:
        if (master_talk_girl_no == 11) {
            fade_in_step();
        } else {
            fade_in();
        }
        if (master_talk_girl_no == 9) {
            (*(Rec *)&class_w[96]).vol += 4;
            if ((*(Rec *)&class_w[96]).vol >= 125) {
                (*(Rec *)&class_w[96]).vol = 128;
            }
        }
        if (master_talk_girl_no != 11 && now_sub_sub_step == 3) {
            device_port_init(0, 0);
        }
        break;

    case 3:
        serifu_run();
        break;

    case 4:
        switch (master_talk_girl_no) {
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
                (*(Rec *)&class_w[96]).kind = 4;
                (*(Rec *)&class_w[96]).flags |= 0x80;
                (*(Rec *)&class_w[96]).vol = 128;
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
        if (master_talk_girl_no == 11) {
            fade_out_step();
        } else {
            fade_out();
        }
        if ((*(Rec *)&class_w[96]).vol < 4) {
            (*(Rec *)&class_w[96]).vol = 0;
        } else {
            (*(Rec *)&class_w[96]).vol -= 4;
        }
        break;

    case 7:
        master_log_girl_add();
        sub_step_up();
        sub_step_up();
        break;

    default:
        game_reset();
        break;
    }

    common_disp();
}
