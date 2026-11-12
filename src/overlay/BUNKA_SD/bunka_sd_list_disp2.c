/* bunka_sd_list_disp2 - BUNKA_SD overlay, Address: 0x8013867C, Size: 404 bytes (101 insns) */
/* @SymbolSize: 404 */
/* @PostProc: patch_insn_reg_field 0x14:rt:v1:v0 0x18:rs:v1:v0 0x18:rt:v1:v0 0x2c:rs:v1:v0 0x34:rs:v1:v0 0x34:rt:v1:v0 0x40:rt:v1:v0 */
#include "types.h"

extern s32 set_kanji_string(s32, s32, s32, s32, s32);
extern void set_XAsector(s32, s32, s32);
extern void device_cmd_send(s32);
extern s32 get_CD_State(void);
extern s32 check_end_k(void);
extern void k_scroll_set(void);
extern void k_disp_inc(void);

extern s32 step_state;
extern s32 pad_data;
extern u8 scene_entry_c;
extern u8 now_girl;
extern s32 bunka_sd_cue_list_top;
extern u8 **bunka_sd_cue_line_ptr;
extern s32 *bunka_sd_cue_xa_ptr;
extern u8 bunka_sd_list_disp2_str0[];

void bunka_sd_list_disp2(void)
{
    s32 v0;
    s32 i;
    s32 dy;
    s32 idx;

    v0 = step_state;
    if (v0 == 1) {
        step_state = v0 + 1;
        set_kanji_string(-86, 64, 0, (s32)bunka_sd_list_disp2_str0, 0);
        for (i = bunka_sd_cue_list_top; i < 100; i++) {
            dy = (s16)(((i - bunka_sd_cue_list_top) << 4) + 64);
            set_kanji_string(-72, dy, 0, (s32)bunka_sd_cue_line_ptr[i] + 1, 0);
            if (bunka_sd_cue_line_ptr[i][0] == 103) {
                if ((pad_data & 0x20) == 0) {
                    idx = scene_entry_c;
                    set_XAsector(bunka_sd_cue_xa_ptr[idx], bunka_sd_cue_xa_ptr[idx + 1],
                                  now_girl);
                    device_cmd_send(768);
                }
                scene_entry_c = scene_entry_c + 2;
                bunka_sd_cue_list_top = i + 1;
                i = 100;
            }
        }
    }

    if (get_CD_State() == 2 && check_end_k() != 0) {
        step_state = 0;
    }
    k_scroll_set();
    k_disp_inc();
}
