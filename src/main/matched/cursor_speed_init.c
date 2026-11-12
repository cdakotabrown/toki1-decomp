/* Function: cursor_speed_init */
/* Address: 0x8005F970 */
/* Size: 744 bytes (186 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"

extern u8 csr_speed;
extern u8 menu_csr;

extern char cursor_speed_init_str0[];
extern char cursor_speed_init_str1[];
extern char cursor_speed_init_str2[];
extern char cursor_speed_init_str3[];
extern char cursor_speed_init_str4[];
extern char cursor_speed_init_str5[];
extern char cursor_speed_init_str6[];
extern char cursor_speed_init_str7[];
extern char cursor_speed_init_str8[];
extern char cursor_speed_init_str9[];
extern char cursor_speed_init_str10[];
extern char cursor_speed_init_str11[];
extern u8 mascot_flag2[];
extern u8 mascot_flag[];

extern void text_sys_init(void);
extern void basyo_init(char *);
extern void system_config_disp(s32);
extern void set_kanji_string(s16, s16, s32, char *, s32);
extern void k_sub_reset_point_set(void);
extern void menu_set(s32, s32, s16 *, s16 *, s16 *, s16 *);
extern void k_sub_disp_start(s32);
extern void sub_sub_step_up(void);

void cursor_speed_init(void) {
    s16 pad_hi[6];
    s16 arr1[6];
    s16 arr2[6];
    s16 arr3[6];
    s16 arr4[6];
    s32 i;
    s32 pad0, pad1;

    text_sys_init();
    basyo_init(cursor_speed_init_str0);
    system_config_disp(4);

    for (i = 0; i < 5; i++) {
        arr1[i] = -4;
        arr2[i] = (i * 16) - 64;
        arr3[i] = 108;
        arr4[i] = 16;
    }

    if (mascot_flag[8] != 0 || mascot_flag2[8] != 0) {
        set_kanji_string(arr1[0], arr2[0], 3, cursor_speed_init_str1, 0);
        set_kanji_string(arr1[1], arr2[1], 3, cursor_speed_init_str2, 0);
        set_kanji_string(arr1[2], arr2[2], 3, cursor_speed_init_str3, 0);
        set_kanji_string(arr1[3], arr2[3], 3, cursor_speed_init_str4, 0);
        set_kanji_string(arr1[4], arr2[4], 3, cursor_speed_init_str5, 0);
    } else {
        set_kanji_string(arr1[0], arr2[0], 3, cursor_speed_init_str6, 0);
        set_kanji_string(arr1[1], arr2[1], 3, cursor_speed_init_str7, 0);
        set_kanji_string(arr1[2], arr2[2], 3, cursor_speed_init_str8, 0);
        set_kanji_string(arr1[3], arr2[3], 3, cursor_speed_init_str9, 0);
        set_kanji_string(arr1[4], arr2[4], 3, cursor_speed_init_str10, 0);
    }

    k_sub_reset_point_set();
    set_kanji_string(-128, 50, 0, cursor_speed_init_str11, 0);
    menu_set(1, 5, arr1, arr2, arr3, arr4);

    switch (csr_speed - 1) {
    case 0:
    case 4:
    default:
        menu_csr = 0;
        break;
    case 1:
        menu_csr = 1;
        break;
    case 2:
        menu_csr = 2;
        break;
    case 3:
        menu_csr = 3;
        break;
    case 5:
        menu_csr = 4;
        break;
    }

    k_sub_disp_start(0);
    sub_sub_step_up();
}
