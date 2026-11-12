/* Function: card_status_wait */
/* Address: 0x800541CC */
/* Size: 296 bytes (74 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern s32 _card_status(s32);
extern void close(s32);
extern void card_step_set(s32);

extern s32 card_fd;
extern u8 card_event_cnt;

s32 card_status_wait(void)
{
    s32 result;

    result = _card_status(0);
    if (result == 0) {
        do {
            result = _card_status(0);
        } while (result == 0);
    }

    switch (result) {
        case 1:
            close(card_fd);
            break;
        case 2:
            close(card_fd);
            break;
        case 4:
            close(card_fd);
            break;
        case 8:
            close(card_fd);
            break;
        case 17:
            card_event_cnt++;
            card_step_set(0);
            close(card_fd);
            break;
        case 33:
            card_event_cnt++;
            card_step_set(0);
            close(card_fd);
            break;
    }

    return result;
}
