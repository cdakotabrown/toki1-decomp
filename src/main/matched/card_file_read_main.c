/* Address: 0x80054674  Size: 260 bytes (65 instructions) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern volatile s32 card_cnt;
extern volatile u8 card_busy;
extern u8 card_step;
extern u8 now_album;
extern u8 card_result;

extern void Sw_Clear(void);
extern s32 card_file_read(u8);
extern void card_step_next(void);
extern s32 card_status_wait(void);

u8 card_file_read_main(void) {
    card_cnt = card_cnt + 1;
    if (!card_busy) {
        goto done;
    }

    switch (card_step) {
    case 0:
        Sw_Clear();
        now_album = 0;
        if (card_file_read(0) == 1) {
            card_step_next();
            break;
        }
        card_result = 208;
        card_busy = 0;
        goto done;
    case 1:
        if (card_status_wait() == 1) {
            card_step_next();
        }
        break;
    case 2:
        card_result = 3;
        card_busy = 0;
        goto done;
    case 16:
        card_busy = 0;
        card_result = 240;
        goto done;
    }

done:
    return card_result;
}
