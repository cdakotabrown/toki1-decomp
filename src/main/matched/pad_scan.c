/* Address: 0x80043330 (the pad_data read is hoisted above the prologue) */
/* Size: 856 bytes (214 instructions) -- ends at the `jr ra` before the */
/* Region: Game (IDO 5.2) */
/* @SymbolStart: 0x80043330 */
/* @SymbolSize: 856 */
/* @CompileFlags: -Wc,-pic1 -cckr */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern s32 pad_get();
extern void konami_command_check();
extern void overlay_work_clear();
extern void disp_mode_set2();
extern void system_reset();
extern void goto_step();
extern void prim_init();
extern void class_clear();
extern void back_clear_switch();
extern void dec_bg_reset();
extern void device_state_init();
extern void tpage_buf_clear();
extern void device_cmd_send();
extern void SD_Exit();
extern void SD_Init();
extern void CDQ_init();
extern void _game_work_init_set();
extern void game_work_init2();
extern void work_clear();
extern void card_state_clear();

typedef struct {
    u8 key;
    u8 pad1[11];
    u32 cur;
    u32 prev;
    u32 trig;
    u32 rel;
    u8 pad2[0x168];
    u32 frame;
    u8 pad3[12];
    u8 mode;
    u8 pad4[7];
    u8 f870;
    u8 f871;
    u8 f872;
} Sys;
extern Sys pad_type;
extern u8 pad_main_tbl0;
extern u8 pad_scan_tbl0;
extern u8 card_busy;

void pad_scan()
{
    u32 r;

    pad_type.prev = pad_type.cur;
    r = pad_get();
    if ((r & 0xFFFF0000) == 0xFFFF0000) {
        pad_type.cur = r & 0xFFFF;
        if (pad_type.cur == 0xFFFF) {
            pad_type.cur = 0;
        }
    } else if ((r & 0xFFFF) == 0xFFFF) {
        pad_type.cur = r & 0xFFFF0000;
        if ((r & 0xFFFF0000) == 0xFFFF0000) {
            pad_type.cur = 0;
        }
    } else {
        pad_type.cur = r;
    }
    pad_type.trig = ~pad_type.prev & pad_type.cur;
    pad_type.rel = pad_type.prev;
    pad_type.rel &= ~pad_type.cur;
    if (pad_main_tbl0 != 0 && (pad_type.frame & 1)) {
        pad_type.trig |= 0x20;
    }
    if ((pad_type.f871 | 0) == 1) {
        if (pad_type.trig & 0x08000800) {
            if (pad_type.f870 == 0) {
                pad_type.f870 = 1;
                pad_type.f872 = 0;
            } else {
                pad_type.f872 = 1;
                pad_type.f870 = 0;
            }
        }
    }
    if (pad_type.mode != 208 && pad_type.mode != 32 && pad_type.mode != 144 && (pad_type.key | 0) == 1) {
        if ((pad_type.cur & 0x90C) == 0x90C) {
            if (pad_scan_tbl0++ >= 129) {
                pad_scan_tbl0 = 0;
                overlay_work_clear();
                disp_mode_set2(0);
                system_reset(0);
                goto_step(0);
                prim_init();
                class_clear(0);
                back_clear_switch(1);
                dec_bg_reset();
                device_state_init();
                tpage_buf_clear();
                device_cmd_send(127);
                SD_Exit();
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
        } else if ((pad_type.cur & 0x090C0000) == 0x090C0000) {
            if (pad_scan_tbl0++ >= 129) {
                pad_scan_tbl0 = 0;
                overlay_work_clear();
                disp_mode_set2(0);
                system_reset(0);
                goto_step(17);
                prim_init();
                class_clear(0);
                back_clear_switch(1);
                dec_bg_reset();
                device_state_init();
                tpage_buf_clear();
                device_cmd_send(127);
                SD_Exit();
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
        } else {
            pad_scan_tbl0 = 0;
        }
    }
    konami_command_check();
}
