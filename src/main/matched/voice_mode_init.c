/* Function: voice_mode_init */
/* Address: 0x80060024 */
/* Size: 380 bytes (95 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: patch_stack_complete --old 104 --new 112 */
/* @CompileFlags: -Wo,-loopunroll,0 */
#include "types.h"

extern u8 voice_level;
extern u8 menu_csr;

extern char voice_mode_init_str0[];
extern char voice_mode_init_str1[];
extern char voice_mode_init_str2[];
extern char voice_mode_init_str3[];
extern char voice_mode_init_str4[];

extern void text_sys_init(void);
extern void basyo_init(char *);
extern void system_config_disp(s32);
extern void set_kanji_string(s16, s16, s32, char *, s32);
extern void k_sub_reset_point_set(void);
extern void menu_set(s32, s32, s16*, s16*, s16*, s16*);
extern void k_sub_disp_start(s32);
extern void sub_sub_step_up(void);

void voice_mode_init(void) {
    s16 arr1[4];
    s16 arr2[4];
    s16 arr3[4];
    s16 arr4[4];
    s32 i;
    s16 pad[4];

    (void)&pad;

    text_sys_init();
    basyo_init(voice_mode_init_str0);
    system_config_disp(5);

    for (i = 0; i < 3; i++) {
        arr1[i] = -4;
        arr2[i] = (i * 16) - 64;
        arr3[i] = 108;
        arr4[i] = 16;
    }

    set_kanji_string(arr1[0], arr2[0], 3, voice_mode_init_str1, 0);
    set_kanji_string(arr1[1], arr2[1], 3, voice_mode_init_str2, 0);
    set_kanji_string(arr1[2], arr2[2], 3, voice_mode_init_str3, 0);

    k_sub_reset_point_set();
    set_kanji_string(-128, 50, 0, voice_mode_init_str4, 0);
    menu_set(1, 3, arr1, arr2, arr3, arr4);

    switch (voice_level) {
        case 1:
            menu_csr = 0;
            break;
        case 0:
            menu_csr = 1;
            break;
        case 2:
        default:
            menu_csr = 2;
            break;
    }
    k_sub_disp_start(0);
    sub_sub_step_up();
}
