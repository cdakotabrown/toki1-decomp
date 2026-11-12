/* Function: ending_main */
/* Address: 0x80079628  (registered address corrected 2026-09-17; see below) */
/* Size: 612 bytes (153 instructions) */
/* Region: Game (0x80041000-0x80085FFF), IDO 5.2 */
/* @SymbolStart: 0x80079628 */
/* @SymbolSize: 612 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: patch_insn_reg_field 0x0:rt:t6:a1 0x4:rt:t7:t6 0x8:rs:t7:t6 0x8:rt:t7:t6 0xc:rs:t6:a1 0xc:rt:t6:a1 0x14:rt:t6:a1 0x1c:rs:t7:t6 */
/* @PostProc: patch_insn_reg_field 0x28:rt:t7:t6 0x28:rd:t7:t6 0x30:rt:t7:t6 0x34:rt:t7:t6 0x3c:rs:t7:t6 0x44:rt:t8:t7 0x48:rs:t8:t7 */
#include "types.h"

extern u8 now_sub_step;
extern u8 bunkaken_flags;
extern u8 _debug_number_show_str0;
extern u8 ending_main_tbl0;
extern u32 ending_main_tbl1;
extern u32 overlay_load_init_tbl0;
extern void (*game_mode_step)(void);
extern u32 VB;
extern u8 option_on;
extern u8 ret_step;
extern u8 ret_sub_step;
extern u8 ret_sub_sub_step;
extern void D_8013E830(void);

extern void vram_area_clear(s32);
extern void class_clear_all(s32);
extern void prim_init(void);
extern void cd_read_start2(s32, u32, s32);
extern void sub_step_up(void);
extern s32 get_CD_State2(void);
extern void goto_sub_step(s32);
extern void SetDispMask(s32);
extern void DrawSync(s32);
extern void ResetGraph(s32);
extern void CloseEvent(u32);
extern s32 block_checksum_check(s32);
extern void cache_flush_critical(void);
extern void goto_step(s32);
extern void goto_sub_sub_step(s32);
extern void draw2d3d(s32, s32);
extern void tpage_buf_clear(void);
extern void vblank_counter_event_init(void);
extern void device_state_init(void);
extern void dec_bg_reset(void);
extern void CDQ_init(void);
extern void device_cmd_send(s32);
extern void change_trans_pointer(u32);
extern void ending_card_step(void);

void ending_main(void) {
    s32 mode = now_sub_step;

    ending_main_tbl1 = 0x80132000;
    switch (mode) {
    case 0:
        bunkaken_flags |= 1;
        _debug_number_show_str0 = 0;
        vram_area_clear(255);
        class_clear_all(0);
        prim_init();
        ending_main_tbl0 = 96;
        overlay_load_init_tbl0 = 0x9A6A;
        cd_read_start2(96, ending_main_tbl1, 0x9A6A);
        sub_step_up();
        break;
    case 1:
        if (get_CD_State2() & 1) {
            sub_step_up();
        }
        break;
    case 2:
        if (block_checksum_check(ending_main_tbl0) != 0) {
            sub_step_up();
            cache_flush_critical();
            SetDispMask(0);
        } else {
            goto_sub_step(0);
        }
        break;
    case 3:
        game_mode_step = D_8013E830;
        DrawSync(0);
        ResetGraph(3);
        CloseEvent(VB);
        game_mode_step();
        vram_area_clear(255);
        sub_step_up();
        break;
    case 4:
        DrawSync(0);
        ResetGraph(3);
        vblank_counter_event_init();
        device_state_init();
        dec_bg_reset();
        CDQ_init();
        device_cmd_send(127);
        change_trans_pointer(0x801B8000);
        sub_step_up();
        break;
    case 5:
        ending_card_step();
        break;
    case 6:
        if (option_on == 0) {
            goto_step(ret_step);
            goto_sub_step(ret_sub_step);
            goto_sub_sub_step(ret_sub_sub_step);
        } else {
            goto_step(0);
            class_clear_all(0);
            draw2d3d(0, 0);
            prim_init();
            tpage_buf_clear();
        }
        break;
    }
}
