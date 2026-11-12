/* Likely Symbol: card_file_check (from o.bin, offset +40 bytes from 0x80054EBC) */
/* Address: 0x80054EE4 (180 bytes) */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"

extern void save_name_set(char *buffer, s32 arg1);
extern s32 open(char *buffer, s32 mode);
extern void close(s32 handle);

extern s32 card_fd;

s32 card_file_check(s32 arg0) {
    char buffer[28];
    s32 pad[4];
    s32 maxRetries;
    s32 retryCount;

    if (arg0 != 0) {
        maxRetries = 8;
    } else {
        maxRetries = 16;
    }

    save_name_set(buffer, arg0);
    retryCount = 0;

    card_fd = open(buffer, 2);

    if (card_fd == -1) {
        do {
            retryCount++;
            if (retryCount > maxRetries) {
                return 0;
            }
            card_fd = open(buffer, 2);
        } while (card_fd == -1);
    }

    close(card_fd);
    return 1;
}
