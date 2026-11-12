/* Address: 0x8005B6CC */
/* Size: 852 bytes (213 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @CompileFlags: -cckr -Wc,-pic1 -Wo,-noprecolor */
/* @PostProc: strip_leading_instructions --count auto */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=16:a1,31:a1 IDO52_UGEN_TEMP_REG=8:t2 */
#include "types.h"

typedef struct {
    s16 f0;
    s16 count;
    s16 f4;
    s16 f6;
    u32 b0 : 1;
    u32 b1 : 1;
    u32 met : 1;
    u32 rest : 29;
    u8 pad[44];
} Rec;

typedef struct {
    u8 lo : 4;
    u8 hi : 4;
} Nib;

typedef struct {
    u8 pad0[0x40];
    u8 x40;
    u8 pad41[0x65 - 0x41];
    u8 x65[0xF4 - 0x65];
    u16 date;
    u8 padF6[448 - 0xF6];
    Rec recs[22];
    u8 pad690[0xF68 - 0x690];
    Nib nib;
    u8 padF69[3];
    u8 xF6C;
    u8 padF6D[0xF80 - 0xF6D];
    s32 xF80;
    u8 padF84[4];
    s32 xF88;
    u8 padF8C[0x1093 - 0xF8C];
    s8 x1093;
    u8 pad1094[0x111B - 0x1094];
    u8 x111B;
} Save;

extern Save screen_w;
extern u8 icon_select_check_str0[];
extern u8 icon_select_check_str1[];

icon_select_check()
{
    s32 unused0;
    s32 unused1;
    s32 unused2;
    s32 r;
    s32 t;
    s32 m;

    if (screen_w.xF6C == 2 && (screen_w.xF80 & 1)) {
        return 1;
    }
    r = 0;
    if (screen_w.xF88 & 0x20) {
        if (screen_w.x1093 > 0) {
            r = 1;
            cd_command_issue(9, 0, 0);
            device_cmd_send(3);
            if (device_init_flag_get() == 0) {
                device_port_init(0, 0);
            }
        }
        switch (screen_w.x1093) {
        case 4:
            if (get_icon_can_use(screen_w.x1093)) {
                device_cmd_send(1280);
                m = screen_w.date >> 12;
                if (m == 11) {
                    screen_w.nib.lo = screen_w.x1093;
                    goto_sub_step(5);
                } else {
                    t = basyo_girl_get(m);
                    if (screen_w.recs[t].met) {
                        screen_w.recs[t].count++;
                    }
                    goto_step(48);
                    screen_w.nib.lo = screen_w.x1093;
                }
            }
            break;
        case 0:
        case 1:
        case 2:
        case 3:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
            if (get_icon_can_use(screen_w.x1093)) {
                if (screen_w.x1093 != 8 && screen_w.x1093 != 10) {
                    device_cmd_send(1280);
                } else {
                    device_cmd_send(1293);
                }
                goto_step(48);
                screen_w.nib.lo = screen_w.x1093;
            }
            break;
        case 12:
            device_cmd_send(1293);
            goto_sub_step(2);
            break;
        case 13:
            if (get_icon_can_use(screen_w.x1093)) {
                if (screen_w.x111B == 0) {
                    cursor_work_copy(0);
                    device_cmd_send(1293);
                    goto_sub_step(3);
                }
            }
            break;
        case 11:
            device_cmd_send(1293);
            goto_sub_step(4);
            break;
        case 14:
            if (get_icon_can_use(screen_w.x1093)) {
                screen_w.nib.lo = 14;
                if (screen_w.x65[screen_w.x40] == 5) {
                    device_cmd_send(1280);
                    goto_step(48);
                    sndisp(icon_select_check_str0, 0, 31);
                } else if (screen_w.x65[screen_w.x40] == 11) {
                    device_cmd_send(1280);
                    goto_step(48);
                    sndisp(icon_select_check_str1, 0, 31);
                } else {
                    goto_step(48);
                    screen_w.nib.lo = 7;
                }
            }
            break;
        }
    }
    return r;
}
