/* Address: 0x80137620 */
/* Size: 516 bytes (129 insns) per .sym, but the REAL reachable body */
/* @SymbolSize: 436 */
#include "types.h"

extern void set_kanji_string(s32, s32, s32, s32, s32);
extern void set_XAsector(s32, s32, s32);
extern void device_cmd_send(s32);
extern s32 get_CD_State(void);
extern s32 check_end_k(void);
extern void k_scroll_set(void);
extern void k_disp_inc(void);

extern s32 step_state;
extern s32 bunka_sd_rec_list_top;
extern s32 bunka_sd_rec_line_ptr;
extern s32 bunka_sd_rec_xa_ptr;
extern u8  scene_entry_c;
extern s32 pad_data;
extern u8  now_girl;
extern u8  bunka_sd_list_disp_str0[];

void bunka_sd_list_disp(void)
{
    s32 step;
    s32 row;
    s32 yval;
    s32 *p;

    if (step_state == 1) {
        step_state = step_state + 1;
        step = bunka_sd_rec_list_top;

        if (step != 21 && step != 22) {
            set_kanji_string(-86, 64, 0, (s32)bunka_sd_list_disp_str0, 0);
        }

        for (row = bunka_sd_rec_list_top; row < 100; row++) {
            yval = (s16)((row - bunka_sd_rec_list_top) * 16 + 64);
            set_kanji_string(-72, yval, 0,
                          ((s32 *)bunka_sd_rec_line_ptr)[row] + 1, 0);

            if (103 == *(u8 *)((s32 *)bunka_sd_rec_line_ptr)[row]) {
                if ((pad_data & 0x20) == 0) {
                    p = (s32 *)bunka_sd_rec_xa_ptr + scene_entry_c;
                    set_XAsector(p[0], p[1], now_girl);
                    device_cmd_send(768);
                }
                scene_entry_c = scene_entry_c + 2;
                bunka_sd_rec_list_top = row + 1;
                row = 100;
            }
        }
    }

    if (get_CD_State() == 2) {
        if (check_end_k() != 0) {
            step_state = 0;
        }
    }
    k_scroll_set();
    k_disp_inc();
}
