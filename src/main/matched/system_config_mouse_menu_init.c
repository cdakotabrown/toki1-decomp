/* Address: 0x8005EA30 */
/* Size: 308 bytes (77 instructions) */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: custom_script custom_func_8005EA30.py */
#include "types.h"

extern void text_sys_init(void);
extern void basyo_init(s32 arg0);
extern void system_config_disp(s32 arg0);
extern void set_kanji_string(s16 x, s16 y, s32 count, void *data, s32 flag);
extern void k_sub_reset_point_set(void);
extern void menu_set(s32 arg0, s32 arg1, s16 *arg2, s16 *arg3, s16 *arg4, s16 *arg5);
extern void k_sub_disp_start(s32 arg0);
extern void sub_sub_step_up(void);

extern u8 system_config_mouse_menu_init_str0;
extern u8 system_config_mouse_menu_init_str1;
extern u8 system_config_mouse_menu_init_str2;
extern u8 system_config_mouse_menu_init_str3;
extern u8 system_config_mouse_menu_init_str4;
extern u8 menu_csr;

void system_config_mouse_menu_init(void) {
    s16 xCoords[4];
    s16 yCoords[4];
    s16 heights[4];
    s16 widths[4];
    s32 pad[2];
    s32 i;
    s32 y;

    text_sys_init();
    basyo_init((s32)&system_config_mouse_menu_init_str0);
    system_config_disp(2);

    y = -64;
    for (i = 0; i < 3; i++) {
        yCoords[i] = y;
        y += 16;
        xCoords[i] = -4;
        heights[i] = 108;
        widths[i] = 16;
    }

    set_kanji_string(xCoords[0], yCoords[0], 3, &system_config_mouse_menu_init_str1, 0);
    set_kanji_string(xCoords[1], yCoords[1], 3, &system_config_mouse_menu_init_str2, 0);
    set_kanji_string(xCoords[2], yCoords[2], 3, &system_config_mouse_menu_init_str3, 0);
    k_sub_reset_point_set();
    set_kanji_string(-128, 50, 0, &system_config_mouse_menu_init_str4, 0);
    menu_set(1, 3, xCoords, yCoords, heights, widths);
    menu_csr = 0;
    k_sub_disp_start(0);
    sub_sub_step_up();
}
