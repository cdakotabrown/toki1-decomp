/* Function: config_click_speed_init */
/* Address: 0x8005F184 */
/* Size: 344 bytes (86 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern void text_sys_init(void);
extern void basyo_init(void*);
extern void system_config_disp(s32);
extern void set_kanji_string(s16, s16, s32, void*, s32);
extern void k_sub_reset_point_set(void);
extern void menu_set(s32, s32, s16*, s16*, s16*, s16*);
extern void k_sub_disp_start(s32);
extern void sub_sub_step_up(void);

extern s8 menu_csr;
extern u8 config_click_speed_init_str0;
extern u8 config_click_speed_init_str1;
extern u8 config_click_speed_init_str2;
extern u8 config_click_speed_init_str3;
extern u8 config_click_speed_init_str4;
extern u8 config_click_speed_init_str5;
extern s32 work_fuku[];

void config_click_speed_init(void)
{
    s32 pad[2];
    s16 arr1[4];
    s16 arr2[4];
    s16 arr3[4];
    s16 arr4[4];
    s32 i;

    (void)pad;

    text_sys_init();
    basyo_init(&config_click_speed_init_str0);
    system_config_disp(2);

    for (i = 0; i < 2; i++) {
        arr1[i] = -4;
        arr2[i] = i * 16 - 64;
        arr3[i] = 108;
        arr4[i] = 16;
    }

    set_kanji_string(arr1[0], arr2[0], 3, &config_click_speed_init_str1, 0);
    set_kanji_string(arr1[1], arr2[1], 3, &config_click_speed_init_str2, 0);
    k_sub_reset_point_set();
    set_kanji_string(-128, 50, 0, &config_click_speed_init_str3, 0);
    set_kanji_string(-128, 66, 0, &config_click_speed_init_str4, 0);
    set_kanji_string(-128, 82, 0, &config_click_speed_init_str5, 0);

    menu_set(1, 2, arr1, arr2, arr3, arr4);

    menu_csr = 0;
    work_fuku[5] = 0;

    k_sub_disp_start(0);
    sub_sub_step_up();
}
