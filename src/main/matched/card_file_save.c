/* Function: card_file_save */
/* Address: 0x80055064 */
/* Size: 504 bytes (126 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: remap_stack_slots --map 52:68 */
/* @PostProc: custom_script custom_sprite_set_light_effect1.py */
#include "types.h"
#include "tokimeki/card_file.h"

extern s32 card_fd;
extern u8 album_chr;
extern s32 card_save_blk[];

extern u8 card_file_save_str0;
extern u8 card_file_save_str1;
extern u8 card_file_save_str2;
extern u8 card_file_save_tbl1;
extern u8 *card_file_save_tbl2;
extern u8 *card_file_save_tbl0;

extern void card_w_clear(void);
extern void save_name_set(u8 *, s32);
extern void close(s32);
extern s32 open(u8 *, s32);
extern void strncpy(u8 *, u8 *, s32);
extern void bzero(u8 *, s32);
extern void strcpy(u8 *, u8 *);
extern void strcat(u8 *, u8 *);
extern void memcpy(u8 *, u8 *, s32);
extern void save_game_set(s32);
extern void save_sys_set(s32);
extern s32 write(s32, u8 *, s32);

s32 card_file_save(s32 arg0) {
    u8 sp44[28];
    u8 *buf;
    s32 written;
    s32 chunkSize;

    card_w_clear();
    save_name_set(sp44, arg0);
    close(card_fd);
    card_fd = open(sp44, 2);
    if (card_fd == -1) {
        return -1;
    }

    buf = &album_chr;
    strncpy(&album_chr, &card_file_save_str0, 2);
    (*(CARD_FILE *)&album_chr).type = 0x13;
    (*(CARD_FILE *)&album_chr).block_entry = 1;
    bzero(&(*(CARD_FILE *)&album_chr).title[0], 64);
    strcpy(&(*(CARD_FILE *)&album_chr).title[0], &card_file_save_str1);

    if (arg0 != 0) {
        strcat(&(*(CARD_FILE *)&album_chr).title[0], (u8 *)(*(s32 *)(card_file_save_tbl0 + (arg0 << 2) - 4)));
    } else {
        strcat(&(*(CARD_FILE *)&album_chr).title[0], &card_file_save_str2);
    }

    bzero(&(*(CARD_FILE *)&album_chr).reserve[0], 28);
    memcpy(&(*(CARD_FILE *)&album_chr).clut[0], &card_file_save_tbl1, 32);
    memcpy(&(*(CARD_FILE *)&album_chr).icon[0][0], card_file_save_tbl2, 384);

    if (arg0 != 0) {
        save_game_set(0);
    } else {
        save_sys_set(0);
    }

    written = (s32)(&(*(CARD_FILE *)&album_chr).rec[0].sum);
    chunkSize = (s32)card_save_blk;
    buf = (u8 *)(written + 3840);
    do {
        *(s32 *)chunkSize = *(s32 *)written;
        written += 12;
        *(s32 *)(chunkSize + 4) = *(s32 *)(written - 8);
        chunkSize += 12;
        *(s32 *)(chunkSize - 4) = *(s32 *)(written - 4);
    } while (written != (s32)buf);

    buf = &album_chr;
    written = 0;
    chunkSize = 0x2000;
    do {
        if (write(card_fd, buf, chunkSize) != chunkSize) {
            close(card_fd);
            return -1;
        }
        written += 0x2000;
        buf += 0x2000;
    } while (written != chunkSize);

    return 1;
}
