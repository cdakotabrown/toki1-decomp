/* Likely Symbol: card_file_make (from o.bin, offset +220 bytes from 0x80054AA0) */
/* Address: 0x80054B7C */
/* Size: 248 bytes (62 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: patch_stack_complete --old 96 --new 112 */
/* @PostProc: scheduler_normalizer */
#include "types.h"

extern void card_w_clear(void);
extern void save_name_set(void *buf, s32 arg1);
extern void close(s32 fd);
extern s32 open(void *buf, s32 mode);
extern s32 read(s32 fd, s32 buf, s32 size);

extern s32 card_fd;
extern s32 album_chr;

s32 card_file_read(s32 arg0)
{
    u8 buffer[16];
    volatile s32 pad[6];
    s32 retry;
    s32 result;

    retry = 0;
    card_w_clear();
    save_name_set(buffer, arg0);
    close(card_fd);
    result = open(buffer, 0x8001);
    card_fd = result;

    if (result == -1) {
        do {
            retry++;
            if (retry >= 16) {
                return -1;
            }
            result = open(buffer, 0x8001);
            card_fd = result;
        } while (result == -1);
    }

    retry = 0;
    result = read(card_fd, (s32)&album_chr, 0x2000);
    if (result == -1) {
        do {
            retry++;
            if (retry >= 16) {
                return -1;
            }
            result = read(card_fd, (s32)&album_chr, 0x2000);
        } while (result == -1);
    }

    return 1;
}
