/* Address: 0x80133FA4 */
/* Size: 508 bytes (127 insns) */
/* @SymbolSize: 508 */
/* @PostProc: swap_instructions --offset1 0x13c --offset2 0x148 */
/* @PostProc: patch_insn_reg_field 0x68:rt:v0:v1 0x6c:rs:v0:v1 0xac:rs:s0:t0 0xac:rt:t0:s0 0xe0:rs:s0:t2 0xe0:rt:t2:s0 0x13c:rt:t1:t9 0x148:rt:t9:t0 0x14c:rs:t9:t0 0x14c:rt:t9:t0 0x150:rs:t1:t9 0x150:rt:t1:t9 0x154:rd:t0:t1 0x154:rt:t9:t0 0x158:rs:t0:t9 0x190:rt:v0:v1 0x198:rt:v0:v1 */
#include "types.h"

extern void set_kanji_string(s32, s32, s32, s32, s32);
extern void set_XAsector(s32, s32, s32);
extern void device_cmd_send(s32);
extern s32 get_CD_State(void);
extern s32 check_end_k(void);
extern void k_scroll_set(void);
extern void k_disp_inc(void);

extern s32 step_state;
extern u8  now_sub_sub_step;
extern u8  scene_entry_c;
extern s32 pad_data;
extern u8  now_girl;
extern u8  bunka_sd_list_disp3_str0[];
extern u8  bunka_sd_list_disp3_str1[];
extern u8  bunka_sd_list_disp3_str2[];
extern s32 bunka_sd_rec2_list_top;
extern s32 bunka_sd_rec2_line_ptr;
extern s32 bunka_sd_rec2_xa_ptr;

void bunka_sd_list_disp3(void) {
    s32 i;
    s32 *p;
    u8 *p1;
    u8 *p2;

    if (step_state == 1) {
        step_state = step_state + 1;
        set_kanji_string(-86, 64, 0, (s32)bunka_sd_list_disp3_str0, 0);
        p1 = bunka_sd_list_disp3_str1;
        p2 = bunka_sd_list_disp3_str2;
        for (i = bunka_sd_rec2_list_top; i < 100; i++) {
            set_kanji_string(-72, (s16)((i - bunka_sd_rec2_list_top) * 16 + 64), 0,
                          *(s32 *)(bunka_sd_rec2_line_ptr + i * 4) + 1, 0);
            if (*(u8 *)*(s32 *)(bunka_sd_rec2_line_ptr + i * 4) == 103) {
                if (now_sub_sub_step == 35) {
                    set_kanji_string(-128, 112, 0, (s32)p1, 0);
                    set_kanji_string(-86, 128, 0, (s32)p2, 0);
                }
                if ((pad_data & 0x20) == 0) {
                    p = (s32 *)(bunka_sd_rec2_xa_ptr + scene_entry_c * 4);
                    set_XAsector(p[0], p[1], now_girl);
                    device_cmd_send(768);
                }
                scene_entry_c = scene_entry_c + 2;
                bunka_sd_rec2_list_top = i + 1;
                i = 100;
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
