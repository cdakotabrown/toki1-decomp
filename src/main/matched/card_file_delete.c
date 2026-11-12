/* Likely Symbol: card_file_make (from o.bin, offset +84 bytes from 0x80054AA0) */
/* Address: 0x80054AF4 */
/* Size: 136 bytes (34 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"

extern void save_name_set(void *buf, s32 arg);
extern s32 delete(void *buf);

typedef struct {
    u8 data[20];
    volatile s32 retry;
} FileReq;

s32 card_file_delete(s32 arg0) {
    FileReq req;
    s32 unk[2];
    s32 counter;

    save_name_set(&req, arg0);

    if (delete(&req) != 1) {
        counter = req.retry;
        do {
            counter++;
            if (counter >= 3) {
                return -1;
            }
        } while (delete(&req) != 1);
        req.retry = counter;
    }
    return 1;
}
