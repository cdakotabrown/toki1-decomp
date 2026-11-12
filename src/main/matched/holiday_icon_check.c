/* Address: 0x800760B4 */
/* Size: 1160 bytes (290 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @SymbolSize: 1160 */
/* @CompileEnv: IDO52_UGEN_TEMP_REG=15:t7,25:t6,73:t6 */
/* @CompileFlags: -cckr -Wc,-pic1 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: patch_insn_reg_field 0xf8:rt:t8:t6 0xfc:rt:t6:v0 0x100:rt:t7:t9 0x104:rs:t7:t9 0x104:rt:t7:t9 0x108:rs:t6:v0 0x108:rt:t6:v0 0x10c:rs:t7:t9 0x110:rt:t6:v0 0x110:rd:t9:t8 0x114:rs:t9:t8 0x13c:rt:t6:v0 0x170:rs:t8:t6 0x170:rt:t8:t6 0x188:rt:t8:t6 0x18c:rt:t6:t8 0x190:rs:t6:t8 0x190:rt:t6:t8 0x198:rt:t6:t8 0x1bc:rd:t8:t6 0x1c4:rt:t8:t6 0x1e0:rd:t6:t8 0x1e4:rs:t6:t8 0x1e4:rd:t6:t8 0x1f0:rt:t6:t8 0x1f0:rd:t6:t8 0x1f4:rs:t6:t8 0x240:rd:t8:t6 0x248:rs:t8:t6 0x248:rd:t8:t6 0x250:rt:t8:t6 0x250:rd:t8:t6 0x254:rt:t8:t6 0x298:rt:t8:t6 0x29c:rs:t8:t6 0x29c:rt:t8:t6 0x2a4:rt:t8:t6 0x2b0:rt:t6:t8 0x2bc:rs:t6:t8 0x2f4:rt:t6:t8 0x2fc:rs:t6:t8 0x2fc:rt:t6:t8 0x304:rs:t6:t8 */
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
    u8 a;
    s8 month;
    u8 result;
    u8 d;
} Ent;

typedef struct {
    u8 lo : 4;
    u8 hi : 4;
} Nib;

typedef struct {
    u8 pad0[0x3E];
    u8 x3E;
    u8 x3F;
    u8 x40;
    u8 x41;
    u8 pad42[0xF4 - 0x42];
    u16 date;
    u8 padF6[448 - 0xF6];
    Rec recs[22];
    u8 pad690[0x69C - 0x690];
    Ent ents[32];
    u8 sel;
    u8 pad71D[0xF68 - 0x71D];
    Nib nib;
    u8 padF69[3];
    u8 xF6C;
    u8 padF6D[0xF80 - 0xF6D];
    s32 xF80;
    u8 padF84[4];
    s32 xF88;
    u8 padF8C[0x1093 - 0xF8C];
    s8 x1093;
} Save;

extern Save screen_w;
extern int biorhythm_sf_set();

holiday_icon_check()
{
    s32 unused0;
    s32 unused1;
    s32 unused2;
    s32 unused3;
    s32 r;
    s32 t;

    if (screen_w.xF6C == 2 && (screen_w.xF80 & 1)) {
        return 1;
    }
    r = 0;
    if (screen_w.xF88 & 0x20) {
        if (screen_w.x1093 > 0) {
            r = 1;
            cd_command_issue(9, 0, 0);
            if (device_init_flag_get() == 0) {
                device_port_init(0, 0);
            }
        }
        switch (screen_w.x1093) {
        case 4:
            if (get_icon_can_use(screen_w.x1093)) {
                device_cmd_send(1280);
                if (screen_w.x41 == 6) {
                    if ((screen_w.date >> 12) == 11) {
                        screen_w.nib.hi = screen_w.x1093;
                        screen_w.ents[screen_w.sel].month = screen_w.x1093;
                        screen_w.ents[screen_w.sel].result = biorhythm_sf_set(1);
                        goto_sub_step(7);
                    } else {
                        screen_w.ents[screen_w.sel].month = screen_w.x1093;
                        screen_w.ents[screen_w.sel].result = biorhythm_sf_set(1);
                        screen_w.nib.hi = screen_w.x1093;
                        t = basyo_girl_get(screen_w.date >> 12);
                        if (screen_w.recs[t].met) {
                            screen_w.recs[t].count++;
                        }
                        goto_sub_step(6);
                    }
                } else {
                    t = basyo_girl_get(screen_w.date >> 12);
                    if (screen_w.recs[t].met) {
                        screen_w.recs[t].count++;
                    }
                    screen_w.ents[screen_w.sel].month = screen_w.x1093;
                    screen_w.ents[screen_w.sel].result = biorhythm_sf_set(1);
                    screen_w.nib.hi = screen_w.x1093;
                    goto_sub_step(5);
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
            if (get_icon_can_use(screen_w.x1093)) {
                screen_w.nib.hi = screen_w.x1093;
                device_cmd_send(1280);
                screen_w.ents[screen_w.sel].month = screen_w.x1093;
                screen_w.ents[screen_w.sel].result = biorhythm_sf_set(1);
                goto_sub_step(5);
            }
            break;
        case 9:
            if (get_icon_can_use(screen_w.x1093)) {
                if (reserved_date_check(screen_w.x3E, screen_w.x3F, screen_w.x40) > 0) {
                    screen_w.nib.hi = screen_w.x1093;
                    device_cmd_send(1280);
                    goto_sub_step(254);
                }
            }
            break;
        case 8:
            if (get_icon_can_use(screen_w.x1093)) {
                screen_w.nib.hi = screen_w.x1093;
                device_cmd_send(1293);
                goto_sub_step(16);
            }
            break;
        case 10:
            if (get_icon_can_use(screen_w.x1093)) {
                device_cmd_send(1293);
                goto_sub_step(32);
            }
            break;
        case 12:
            device_cmd_send(1293);
            goto_sub_step(48);
            break;
        case 11:
            device_cmd_send(1293);
            goto_sub_step(64);
            break;
        }
    }
    return r;
}
