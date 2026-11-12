/* Address: 0x8006BAE0 */
/* Size: 488 bytes (122 instructions) */
/* Region: Game (0x80041000-0x80085FFF), IDO 5.2 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: patch_insn_reg_field 0x8:rt:a0:v1 0x14:rs:a0:v1 0x174:rt:v1:v0 0x19c:rs:v1:v0 0x19c:rt:v1:v0 0x1a4:rs:v1:v0 0x1b4:rs:v1:v0 0x1b8:rs:v1:v0 0x1c4:rs:v1:v0 */
/* @CompileEnv: IDO52_COPYPROP_SITE=lod:5,lod:6,lod:7 IDO52_AS1_NO_LA_PULL=14 IDO52_UGEN_TEMP_REG=5:t1,6:t2,7:t3,8:t4,9:t5,10:t6,12:t8,14:t0,15:t1,16:t2,17:t3,18:t4,19:t5,20:t6 */
#include "types.h"

struct Glob6CC {
    u8  pad0[3];
    u8  f6CF;
    u8  pad1[0x122];
    u8  f7F2;
};
extern struct Glob6CC icon_mode;
#define csr_speed (icon_mode.f6CF)
#define menu_mode (icon_mode.f7F2)

extern u8  cursor_flag;
extern u32 pad_data;
extern u32 pad_trig;
extern s16 cursor_cw_x;
extern s16 cursor_cw_y;
extern s16 cursor_cw_frame;
extern u8  cursor_init_tbl0;

void step_directional_scroll_once(void) {
    u32 step;
    s32 delta;
    s32 y;
    u32 pad;
    u32 k;

    if (cursor_flag != 0) {
        return;
    }
    cursor_flag = cursor_flag + 1;
    step = csr_speed;
    if (step == 0) {
        step = 1;
    }
    csr_speed = step;
    delta = step;
    if ((menu_mode & 3) == 0) {
        if ((pad_data & 0xFF) != 0) {
            delta = step << 1;
        }
    }
    pad = pad_data;
    if (pad & 0xF) {
        y = cursor_cw_x;
    } else {
        switch (pad & 0x5000) {
        case 0x1000:
            cursor_cw_y = cursor_cw_y - delta;
            break;
        case 0x4000:
            cursor_cw_y = cursor_cw_y + delta;
            break;
        }
        switch (pad & 0xA000) {
        case 0x2000:
            y = cursor_cw_x;
            y = (s16)(y + delta);
            break;
        case 0x8000:
            y = cursor_cw_x;
            y = (s16)(y - delta);
            break;
        default:
            y = cursor_cw_x;
            break;
        }
    }
    if (y >= 141) {
        y = 140;
    }
    cursor_cw_x = y;
    if (y < -150) {
        y = -150;
        cursor_cw_x = y;
    }
    if (cursor_cw_y < -100) {
        cursor_cw_y = -100;
    }
    if (cursor_cw_y >= 101) {
        cursor_cw_y = 100;
    }
    if (pad_trig & 0x80) {
        k = cursor_init_tbl0;
        if (k == 0) {
            k = 155;
        } else if (k == 172) {
            k = 0;
        } else {
            k = (u8)(k + 1);
        }
        cursor_cw_frame = k;
        cursor_init_tbl0 = k;
    }
}
