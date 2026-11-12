/* Function: system_config_speed_init */
/* Address: 0x8005E390 */
/* Size: 368 bytes (92 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: patch_stack_complete --old 104 --new 112 */
#include "types.h"

extern void text_sys_init(void);
extern void basyo_init(u8 *);
extern void system_config_disp(s32);
extern s16 set_kanji_string(s16 y, s16 x, s32 type, u8 *str, s32 flags);
extern void k_sub_reset_point_set(void);
extern s32 menu_set(s32 mode, s32 count, s16 *yArr, s16 *xArr, s16 *wArr, s16 *hArr);
extern s32 get_k_speed(void);
extern void k_sub_disp_start(s32);
extern void sub_sub_step_up(void);

extern u8 system_config_speed_init_str0;
extern u8 system_config_speed_init_str1;
extern u8 system_config_speed_init_str2;
extern u8 system_config_speed_init_str3;
extern u8 system_config_speed_init_str4;
extern u8 menu_csr;

void system_config_speed_init(void) {
    s16 yArr[4];
    s16 xArr[4];
    s16 wArr[4];
    s16 hArr[4];
    s32 pad1, pad2;
    s32 result;
    s32 i;

    (void)&pad1; (void)&pad2;

    text_sys_init();
    basyo_init(&system_config_speed_init_str0);
    system_config_disp(0);

    for (i = 0; i < 3; i++) {
        yArr[i] = -4;
        xArr[i] = -64 + i * 16;
        wArr[i] = 108;
        hArr[i] = 16;
    }

    set_kanji_string(yArr[0], xArr[0], 3, &system_config_speed_init_str1, 0);
    set_kanji_string(yArr[1], xArr[1], 3, &system_config_speed_init_str2, 0);
    set_kanji_string(yArr[2], xArr[2], 3, &system_config_speed_init_str3, 0);
    k_sub_reset_point_set();
    set_kanji_string(-128, 50, 0, &system_config_speed_init_str4, 0);

    menu_set(1, 3, yArr, xArr, wArr, hArr);
    result = get_k_speed();

    switch (result) {
    case 0:
    default:
        menu_csr = 2;
        break;
    case 1:
        menu_csr = 1;
        break;
    case 3:
        menu_csr = 0;
        break;
    }

    k_sub_disp_start(0);
    sub_sub_step_up();
}
