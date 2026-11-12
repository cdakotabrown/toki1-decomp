/* @SymbolStart: 0x80132DFC */
/* @RodataBase: 0x80139FC4 */
/* Address: 0x80132DFC (prologue 0x80132E04), Size: 500 bytes (125 insns) */
/* @SymbolSize: 500 */
#include "types.h"

extern void set_kanji_string(s32, s32, s32, s32, s32);
extern void set_XAsector(s32, s32, s32);
extern void device_cmd_send(s32);
extern s32  get_CD_State(void);
extern s32  check_end_k(void);
extern void sub_sub_step_up(void);
extern void k_scroll_set(void);
extern void k_disp_inc(void);

extern s32 step_counter;
extern s32 work_takak_15;
extern s32 pad_data;
extern s32 bunka_sd_kind;
extern u8  bunka_sd_daimoku_disp_str0[];
extern u8  bunka_sd_daimoku_disp_str1[];
extern u8  bunka_sd_daimoku_disp_str2[];
extern u8  bunka_sd_daimoku_disp_str3[];
extern u8  bunka_sd_daimoku_disp_str4[];
extern u8  bunka_sd_daimoku_disp_str5[];

void bunka_sd_daimoku_disp(void) {
    if (step_counter == 1) {
        set_kanji_string(-120, 48, 0, (s32)bunka_sd_daimoku_disp_str0, 0);
        switch (bunka_sd_kind) {
        case 0:
            set_kanji_string(-86, 48, 0, (s32)bunka_sd_daimoku_disp_str1, 0);
            break;
        case 1:
            set_kanji_string(-86, 48, 0, (s32)bunka_sd_daimoku_disp_str2, 0);
            if ((pad_data & 0x20) == 0) {
                set_XAsector(0x1E00004C, 0x10000051, 14);
                device_cmd_send(768);
            }
            break;
        case 2:
            set_kanji_string(-86, 48, 0, (s32)bunka_sd_daimoku_disp_str3, 0);
            if ((pad_data & 0x20) == 0) {
                set_XAsector(0x1C0002E9, 0x410002F3, 0);
                device_cmd_send(768);
            }
            break;
        case 3:
            set_kanji_string(-86, 48, 0, (s32)bunka_sd_daimoku_disp_str4, 0);
            break;
        case 4:
        default:
            set_kanji_string(-86, 48, 0, (s32)bunka_sd_daimoku_disp_str5, 0);
            if ((pad_data & 0x20) == 0) {
                set_XAsector(0x1F0000AD, 0x100000B3, 11);
                device_cmd_send(768);
            }
            break;
        }
    } else {
        if (get_CD_State() == 2 && check_end_k() != 0) {
            sub_sub_step_up();
        }
    }
    k_scroll_set();
    k_disp_inc();
}
