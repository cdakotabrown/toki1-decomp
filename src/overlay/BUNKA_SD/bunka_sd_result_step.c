/* Address: 0x80132FF8 (head absorbed from 0x80132FF0) */
/* Size: 532 bytes (133 insns incl. the hoisted cross-prologue head) */
/* @RodataBase: 0x80139FD8 */
/* @SymbolStart: 0x80132FF0 */
/* @SymbolSize: 532 */
/* @PostProc: patch_stack_complete --old 64 --new 80 */
/* @PostProc: remap_stack_slots --from 36 --to 56 --from 60 --to 52 */
/* @PostProc: patch_insn_reg_field 0x7c:rs:v0:t8 0x7c:rt:t8:v0 */
#include "types.h"

extern s32 strlen(u8 *);
extern void strcpy(u8 *, u8 *);
extern void strcat(u8 *, u8 *);
extern void set_kanji_string(s32, s32, s32, u8 *, s32);
extern void set_XAsector(s32, s32, s32);
extern void device_cmd_send(s32);
extern s32 get_CD_State(void);
extern s32 check_end_k(void);
extern void sub_sub_step_up(void);
extern void k_scroll_set(void);
extern void k_disp_inc(void);

extern s32 step_counter;
extern s32 pad_data;
extern s32 bunka_sd_kind;
extern u8  p_sei[];
extern u8  p_mei[];
extern u8  bunka_sd_result_step_str0[];
extern u8  bunka_sd_result_step_str1[];
extern u8  bunka_sd_result_step_str2[];
extern u8  bunka_sd_result_step_str3[];
extern u8  bunka_sd_result_step_str4[];

void bunka_sd_result_step(void) {
    s32 total;
    u8 buf[24];

    if (step_counter == 1) {
        switch ((u32)bunka_sd_kind) {
        case 0:
            total = strlen(p_sei);
            total = strlen(p_mei) + total;
            strcpy(buf, p_sei);
            strcat(buf, bunka_sd_result_step_str0);
            strcat(buf, p_mei);
            set_kanji_string((s16)(96 - total * 7), 64, 0, buf, 0);
            break;
        case 1:
            if ((pad_data & 0x20) == 0) {
                set_XAsector(0x0B00001B, 0x05000020, 14);
                device_cmd_send(768);
            }
            set_kanji_string(54, 64, 0, bunka_sd_result_step_str1, 0);
            break;
        case 2:
            if ((pad_data & 0x20) == 0) {
                set_XAsector(0x1D00005B, 0x0B000061, 0);
                device_cmd_send(768);
            }
            set_kanji_string(40, 64, 0, bunka_sd_result_step_str2, 0);
            break;
        case 3:
            set_kanji_string(40, 64, 0, bunka_sd_result_step_str3, 0);
            break;
        case 4:
        default:
            set_kanji_string(68, 64, 0, bunka_sd_result_step_str4, 0);
            break;
        }
    } else {
        if (get_CD_State() == 2) {
            if (check_end_k() != 0) {
                sub_sub_step_up();
            }
        }
    }
    k_scroll_set();
    k_disp_inc();
}
