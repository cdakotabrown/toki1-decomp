/* Address: 0x80054C74 */
/* Size: 624 bytes (156 instructions) */
/* Region: Game (0x80041000-0x80086000) */
/* Compiler: IDO 5.2 */
/* @CompileEnv: IDO52_UGEN_TEMP_REG=6:t2,7:t3,8:t4,9:t5,10:t6,12:t9 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"
#include "tokimeki/card_file.h"

typedef struct {
    u8 b[3840];
} SaveBlk;

extern u8 card_file_write_str0[];
extern u8 card_file_write_str1[];
extern u8 card_file_write_str2[];
extern u8 **card_file_save_tbl0;
extern u8 card_file_save_tbl1[];
extern u8 *card_file_save_tbl2[];
extern u8 album_chr[];
extern SaveBlk album_chr_rec0_sum;
extern SaveBlk card_save_blk;
extern s32 card_fd;

extern void card_w_clear(void);
extern s32 card_file_make(s32 slot);
extern void save_sys_set(s32 arg0);
extern void save_game_set(s32 arg0);
extern void save_name_set(void *buf, s32 slot);
extern u8 rec_table_trim(u8 *a, u8 *b, s32 slot);
extern void bzero(void *dst, s32 n);
extern void strncpy(void *dst, void *src, s32 n);
extern void memcpy(void *dst, void *src, s32 n);
extern void strncat(void *dst, void *src, s32 n);
extern void close(s32 fd);
extern s32 open(void *path, s32 mode);
extern s32 write(s32 fd, void *buf, s32 size);

s32 card_file_write(s32 slot)
{
    u8 buffer[28];
    s32 idx;
    volatile s32 pad[2];
    u8 hi;
    u8 lo;
    s32 retry;
    s32 result;

    retry = 0;
    card_w_clear();
    save_name_set(buffer, slot);
    close(card_fd);
    result = open(buffer, 0x8002);
    card_fd = result;

    if (result == -1) {
        do {
            retry++;
            if (card_file_make(slot) == -1) {
                return -1;
            }
            if (retry >= 16) {
                return -1;
            }
            result = open(buffer, 0x8002);
            card_fd = result;
        } while (result == -1);
        retry = 0;
    }

    strncpy(album_chr, card_file_write_str0, 2);
    (*(CARD_FILE *)album_chr).type = 19;
    (*(CARD_FILE *)album_chr).block_entry = 1;
    bzero((&(*(CARD_FILE *)album_chr).title[0]), 64);
    strncpy((&(*(CARD_FILE *)album_chr).title[0]), card_file_write_str1, 20);
    if (slot != 0) {
        strncat((&(*(CARD_FILE *)album_chr).title[0]), card_file_save_tbl0[slot - 1], 6);
    } else {
        strncat((&(*(CARD_FILE *)album_chr).title[0]), card_file_write_str2, 12);
    }
    bzero((&(*(CARD_FILE *)album_chr).reserve[0]), 28);

    rec_table_trim(&hi, &lo, slot);
    idx = hi;
    if (idx >= 13) {
        idx = 0;
    }
    memcpy((&(*(CARD_FILE *)album_chr).clut[0]), &card_file_save_tbl1[idx * 32], 32);
    memcpy((&(*(CARD_FILE *)album_chr).icon[0][0]), card_file_save_tbl2[idx], 384);

    if (slot != 0) {
        save_game_set(0);
    } else {
        save_sys_set(0);
    }

    card_save_blk = album_chr_rec0_sum;

    result = write(card_fd, album_chr, 0x2000);
    if (result == -1) {
        do {
            retry++;
            if (retry >= 16) {
                return -1;
            }
            result = write(card_fd, album_chr, 0x2000);
        } while (result == -1);
    }

    return 1;
}
