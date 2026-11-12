/* Likely Symbol: card_file_make (from o.bin, offset +28 bytes from 0x80054AA0) */
/* Address: 0x80054A84 */
/* Size: 112 bytes (28 instructions) */
/* Region: GCC (< 0x80081000, OR encoding) */
/* Compiler: IDO 5.2 via ido-static-recomp */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"

extern void save_name_set(void *buf, s32 arg1);
extern void close(s32 fd);
extern s32 open(void *buf, s32 size);

extern s32 card_fd;

s32 card_file_make(s32 arg0)
{
    u8 buffer[24];
    s32 pad1, pad2;
    s32 result;

    save_name_set(buffer, arg0);
    close(card_fd);
    result = open(buffer, 0x10200);
    card_fd = result;

    if (result == -1) {
        return -1;
    }

    close(card_fd);
    return 1;
}
