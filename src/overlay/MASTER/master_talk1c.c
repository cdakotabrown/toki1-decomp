/* @RodataBase: 0x8013BF1C */
/* @SymbolSize: 1396 */
/* @CompileFlags: -cckr */
#include "types.h"
#include "tokimeki/class_w.h"

typedef struct { s16 h[13]; } Blk26;

extern Blk26 master_talk1c_tbl;
extern u8  now_sub_sub_step;
extern u8  master_talk_girl_no;
extern s16 now_serifu;
extern s16 now_serifu_in;
extern CLASS_W class_w[];
typedef struct {
    u8  unk0[3];
    u8  flags;
    u8  unk4[3];
    u8  vol;
    u8  unk8[14];
    s16 kind;
} Rec;

void master_talk1c()
{
    s32 i;
    Blk26 buf;

    buf = master_talk1c_tbl;
    bustup_ani();

    switch (now_sub_sub_step) {
    case 0:
        zyotai_sprite_set_chk();
        bustup_face_change(5);
        if (master_talk_girl_no == 12) {
            bustup_face_change(0);
        }
        indexed_byte_toggle(1);
        cd_read_start2(14, 0x80197000, 0xA9D0);
        break;
    case 1:
        if (get_CD_State2() & 1) {
            master_cw_init();
            now_serifu = buf.h[master_talk_girl_no];
            now_serifu_in = 0;
            if (master_talk_girl_no == 9) {
                (*(Rec *)&class_w[96]).flags |= 0x80;
                (*(Rec *)&class_w[96]).kind = 8;
            }
            if (master_talk_girl_no == 10) {
                (*(Rec *)&class_w[96]).flags |= 0x80;
                (*(Rec *)&class_w[96]).kind = 10;
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
        if (master_talk_girl_no == 10 || master_talk_girl_no == 9) {
            if ((*(Rec *)&class_w[96]).vol > 128) {
                (*(Rec *)&class_w[96]).vol = 128;
            } else {
                (*(Rec *)&class_w[96]).vol += 4;
            }
        }
        if (master_talk_girl_no != 11 && now_sub_sub_step == 3) {
            device_port_init(0, 0);
        }
        break;
    case 3:
    case 5:
        serifu_run();
        break;
    case 4:
        switch (master_talk_girl_no) {
        case 0:
            if (now_serifu == 44) {
                (*(Rec *)&class_w[96]).kind = 0;
                (*(Rec *)&class_w[96]).flags |= 0x80;
                (*(Rec *)&class_w[96]).vol = 128;
                goto_sub_sub_step(3);
            } else {
                now_serifu = 36;
                sub_sub_step_up();
            }
            break;
        case 1:
            if (now_serifu == 49) {
                (*(Rec *)&class_w[96]).kind = 2;
                (*(Rec *)&class_w[96]).flags |= 0x80;
                (*(Rec *)&class_w[96]).vol = 128;
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
                (*(Rec *)&class_w[96]).kind = 4;
                (*(Rec *)&class_w[96]).flags |= 0x80;
                (*(Rec *)&class_w[96]).vol = 128;
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
                (*(Rec *)&class_w[96]).flags |= 0x80;
                (*(Rec *)&class_w[96]).kind = 6;
                (*(Rec *)&class_w[96]).vol = 128;
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
        master_log_fixed_add();
        sub_step_up();
        break;
    default:
        game_reset();
        break;
    }
    common_disp();
}
