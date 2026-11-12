/* Function: func_80043338 */
/* Address: 0x80043338 */
/* Size: 848 bytes (212 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* Compiler: IDO 5.2 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: patch_stack_complete --old 24 --new 40 */
/* @PostProc: custom_script custom_func_80043338.py */
#include "types.h"

extern s32 pad_get(void);
extern void konami_command_check(void);
extern void overlay_work_clear(void);
extern void disp_mode_set2(s32);
extern void system_reset(s32);
extern void goto_step(s32);
extern void prim_init(void);
extern void class_clear(s32);
extern void func_80043AFC(s32);
extern void dec_bg_reset(void);
extern void device_state_init(void);
extern void tpage_buf_clear(void);
extern void device_cmd_send(s32);
extern void func_8007BE78(void);
extern void SD_Init(void);
extern void CDQ_init(void);
extern void _game_work_init_set(void);
extern void game_work_init2(void);
extern void work_clear(void);
extern void card_state_clear(void);

extern volatile s32 pad_data;
extern s32 pad_prev;
extern s32 pad_trig;
extern s32 pad_rel;
extern u8 pad_main_tbl0;
extern s32 frame_counter;
extern u8 D_800EF871;
extern u8 pause_flag;
extern u8 class_flag;
extern u8 now_step;
extern u8 pad_type;
extern u8 pad_scan_tbl0;
extern u8 card_busy;

void func_80043338(void)
{
    s32 result;
    s32 mask;
    s32 hiPart;
    s32 buttons;
    s32 newButtons;
    s32 released;
    u8 mode;
    u8 counter;
    s32 one;
    s32 cond;
    u8 newCounter;

    pad_prev = pad_data;

    result = pad_get();

    mask = (s32)0xFFFF0000;
    hiPart = result & mask;
    if (mask == hiPart) {
        buttons = result & 0xFFFF;
        if (buttons == 0xFFFF) {
            buttons = 0;
        }
    } else {
        buttons = result;
        if ((result & 0xFFFF) == 0xFFFF) {
            buttons = hiPart;
            if (mask == hiPart) {
                buttons = 0;
            }
        }
    }

    released = pad_prev;
    newButtons = ~released & buttons;
    pad_rel = released & ~buttons;

    if (pad_main_tbl0 != 0 && (frame_counter & 1)) {
        newButtons |= 0x20;
    }

    pad_trig = newButtons;
    one = 1;

    if (one == D_800EF871) {
        pad_trig = newButtons;
        if (newButtons & 0x08000800) {
            if (pause_flag == 0) {
                pause_flag = one;
                class_flag = 0;
            } else {
                class_flag = one;
                pause_flag = 0;
            }
        }
    }

    mode = now_step;
    pad_data = buttons;
    if (mode == 0xD0) {
        goto call_handler;
    }
    pad_data = buttons;
    if (mode == 0x20) {
        goto call_handler;
    }
    pad_data = buttons;
    if (mode == 0x90) {
        goto call_handler;
    }
    pad_data = buttons;
    if (one != pad_type) {
        goto call_handler;
    }

    if ((buttons & 0x090C) == 0x090C) {
        counter = pad_scan_tbl0;
        cond = (u32)counter < 129u;
        newCounter = (u8)(counter + 1);
        cond ^= 1;
        pad_data = buttons;
        if (cond == 0) {
            pad_scan_tbl0 = newCounter;
            goto call_handler;
        }
        pad_scan_tbl0 = 0;
        overlay_work_clear();
        disp_mode_set2(0);
        system_reset(0);
        goto_step(0);
        prim_init();
        class_clear(0);
        func_80043AFC(1);
        dec_bg_reset();
        device_state_init();
        tpage_buf_clear();
        device_cmd_send(0x7F);
        func_8007BE78();
        SD_Init();
        CDQ_init();
        _game_work_init_set();
        game_work_init2();
        work_clear();
        if (card_busy != 0) {
            card_state_clear();
        }
        return;
    }

    pad_data = buttons;
    if ((buttons & (u32)0x090C0000) == (u32)0x090C0000) {
        counter = pad_scan_tbl0;
        cond = (u32)counter < 129u;
        newCounter = (u8)(counter + 1);
        cond ^= 1;
        if (cond == 0) {
            pad_scan_tbl0 = newCounter;
            goto call_handler;
        }
        pad_scan_tbl0 = 0;
        overlay_work_clear();
        disp_mode_set2(0);
        system_reset(0);
        goto_step(0x11);
        prim_init();
        class_clear(0);
        func_80043AFC(1);
        dec_bg_reset();
        device_state_init();
        tpage_buf_clear();
        device_cmd_send(0x7F);
        func_8007BE78();
        SD_Init();
        CDQ_init();
        _game_work_init_set();
        game_work_init2();
        work_clear();
        if (card_busy != 0) {
            card_state_clear();
        }
        return;
    }

    pad_scan_tbl0 = 0;

call_handler:
    konami_command_check();
}
