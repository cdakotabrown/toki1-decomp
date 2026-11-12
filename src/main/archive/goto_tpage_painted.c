/* Function: card_file_write (archived painted C for a card-file writer) */
/* Address: 0x80054C74 */
/* Size: 624 bytes (156 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: custom_script custom_goto_tpage.py */
#include "types.h"

extern s32 card_fd;

extern u8 album_chr;
extern u8 D_8012B602;
extern u8 D_8012B603;
extern u8 D_8012B604;
extern u8 D_8012B644;
extern u8 D_8012B660;
extern u8 D_8012B680;
extern s32 album_chr_rec0_sum[];
extern s32 card_save_blk[];

extern u8 card_file_write_str0;
extern u8 card_file_write_str1;
extern u8 card_file_write_str2;
extern u8 card_file_save_tbl1;
extern u8 *card_file_save_tbl0;
extern u8 *card_file_save_tbl2[];

extern void card_w_clear(void);
extern void save_name_set(u8 *, s32);
extern void close(s32);
extern s32 open(u8 *, s32);
extern s32 card_file_make(s32);
extern void strncpy(u8 *, u8 *, s32);
extern void bzero(u8 *, s32);
extern void strncat(u8 *, u8 *, s32);
extern void memcpy(u8 *, u8 *, s32);
extern void rec_table_trim(u8 *, u8 *, s32);
extern void save_game_set(s32);
extern void save_sys_set(s32);
extern s32 write(s32, u8 *, s32);

s32 card_file_write(s32 arg0) {
    u8 sp54[28];
    s32 s0;
    u8 month;
    u8 monthB;
    s32 src;
    s32 dst;
    u8 *end;

    s0 = 0;
    card_w_clear();
    save_name_set(sp54, arg0);
    close(card_fd);
    card_fd = open(sp54, 0x8002);
    if (card_fd == -1) {
        do {
            s0 += 1;
            if (card_file_make(arg0) == -1) {
                return -1;
            }
            if (s0 >= 16) {
                return -1;
            }
            card_fd = open(sp54, 0x8002);
        } while (card_fd == -1);
    }

    s0 = 0;
    strncpy(&album_chr, &card_file_write_str0, 2);
    D_8012B602 = 19;
    D_8012B603 = 1;
    bzero(&D_8012B604, 64);
    strncpy(&D_8012B604, &card_file_write_str1, 20);

    if (arg0 != 0) {
        strncat(&D_8012B604, (u8 *)(*(s32 *)((u32)card_file_save_tbl0 + (arg0 << 2) - 4)), 6);
    } else {
        strncat(&D_8012B604, &card_file_write_str2, 12);
    }

    bzero(&D_8012B644, 28);
    rec_table_trim(&month, &monthB, arg0);
    if ((s32)month >= 13) {
        month = 0;
    }

    memcpy(&D_8012B660, (u8 *)(&card_file_save_tbl1 + ((s32)month << 5)), 32);
    memcpy(&D_8012B680, card_file_save_tbl2[month], 384);

    if (arg0 != 0) {
        save_game_set(0);
    } else {
        save_sys_set(0);
    }

    src = (s32)album_chr_rec0_sum;
    dst = (s32)card_save_blk;
    end = (u8 *)(src + 3840);
    do {
        *(s32 *)dst = *(s32 *)src;
        src += 12;
        *(s32 *)(dst + 4) = *(s32 *)(src - 8);
        dst += 12;
        *(s32 *)(dst - 4) = *(s32 *)(src - 4);
    } while (src != (s32)end);

    if (write(card_fd, &album_chr, 0x2000) == -1) {
        do {
            s0 += 1;
            if (s0 >= 16) {
                return -1;
            }
        } while (write(card_fd, &album_chr, 0x2000) == -1);
    }

    return 1;
}
