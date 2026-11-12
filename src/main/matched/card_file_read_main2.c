/* Address: 0x8005487C (prologue at 0x80054890) */
/* Size: 260 bytes (65 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @SymbolStart: 0x8005487C */
/* @SymbolSize: 260 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

typedef struct {
    u8  pad000[0x35];
    u8  clear_active;
    u8  pad036[0x1115 - 0x36];
    u8  clear_result;
    u8  pad1116[2];
    u8  clear_arg;
    u8  pad1119[3];
    u8  clear_step;
    u8  pad111D[3];
    s32 clear_frames;
} GameBlock;

extern GameBlock screen_w;

extern void Sw_Clear(void);
extern s32 card_file_read(u8);
extern void card_step_next(void);
extern s32 card_status_wait(void);

u8 card_file_read_main2(void) {
    screen_w.clear_frames++;
    if (!screen_w.clear_active) {
        goto done;
    }

    switch (screen_w.clear_step) {
    case 0:
        Sw_Clear();
        if (card_file_read(screen_w.clear_arg) == 1) {
            card_step_next();
            break;
        }
        screen_w.clear_result = 208;
        screen_w.clear_active = 0;
        goto done;
    case 1:
        if (card_status_wait() == 1) {
            card_step_next();
        }
        break;
    case 2:
        screen_w.clear_result = 4;
        screen_w.clear_active = 0;
        goto done;
    case 16:
        screen_w.clear_active = 0;
        screen_w.clear_result = 240;
        goto done;
    }

done:
    return screen_w.clear_result;
}
