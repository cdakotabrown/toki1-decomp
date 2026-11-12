/* Function: game_main */
/* Address: 0x800426F0 */
/* Size: 268 bytes (67 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern u8 now_step;
extern u8 attract_wait;
extern s32 step_frame_cnt;
extern s32 game_main_str0;
extern void (*game_mode_step)(void);

extern void boot_main(void);
extern void goto_step(s32);
extern void exit_function(void);
extern void screen_offset_adjust(void);
extern void schedule_init(void);
extern void schedule_main(void);
extern void step34_main(void);
extern void title_main(void);
extern void week_day(void);
extern void holiday(void);
extern void uwasa_main(void);
extern void pre_syogatu_main(void);
extern void pre_xmas_main(void);
extern void movie_play_main(void);
extern void schedule_main38(void);
extern void schedule_main39(void);
extern void byouki_main(void);
extern void overlay_load_main(void);
extern void ending_main(void);
extern void debug_string_show(s32*, s32, s32);
extern void game_reset(void);

void game_main(void) {
    u32 mode;

    mode = now_step;

    switch (mode) {
    case 0:
        boot_main();
        break;
    case 1:
        if (attract_wait == 0) {
            goto_step(0);
        }
        break;
    case 255:
        exit_function();
        break;
    case 254:
        screen_offset_adjust();
        break;
    case 48:
        schedule_init();
        break;
    case 49:
        schedule_main();
        break;
    case 52:
        step34_main();
        break;
    case 18:
        title_main();
        break;
    case 64:
        week_day();
        break;
    case 70:
        holiday();
        break;
    case 68:
        uwasa_main();
        break;
    case 65:
        pre_syogatu_main();
        break;
    case 66:
        pre_xmas_main();
        break;
    case 3:
        game_mode_step();
        break;
    case 16:
        movie_play_main();
        break;
    case 56:
        schedule_main38();
        break;
    case 57:
        schedule_main39();
        break;
    case 50:
        byouki_main();
        break;
    case 2:
        overlay_load_main();
        break;
    case 208:
        ending_main();
        break;
    case 17:
    case 19:
    case 20:
    case 21:
    case 32:
    case 33:
    case 34:
    case 53:
    case 54:
    case 55:
    case 58:
    case 67:
    case 69:
    case 80:
    case 81:
    case 82:
    case 83:
    case 84:
    case 85:
    case 86:
    case 87:
    case 88:
    case 89:
    case 90:
    case 91:
    case 92:
    case 93:
    case 94:
    case 95:
    case 96:
    case 97:
    case 98:
    case 99:
    case 112:
    case 113:
    case 114:
    case 115:
    case 116:
    case 117:
    case 118:
    case 128:
    case 129:
    case 130:
    case 144:
    case 145:
    case 194:
    case 195:
    case 196:
        debug_string_show(&game_main_str0, 20, 44);
        game_mode_step();
        break;
    default:
        game_reset();
        break;
    }
    step_frame_cnt++;
}
