/* @SymbolStart: 0x80139E44 */
/* @SymbolSize: 636 */
/* @CompileFlags: -Wc,-pic0 */
/* Address: 0x80139E44 (absorbed head) / 0x80139E4C (prologue) */
/* Size: 628 bytes (157 insns), 636 bytes / 159 words including the head */
#include "types.h"

extern void strncpy(void *buf, s32 value, s32 n);
extern void strcat(void *buf, u8 *str);
extern void set_kanji_string(s32 x, s32 y, s32 w, void *src, s32 flag);

extern u8  card_sys_present;
extern u8  last_album;
extern u8  now_album;
extern u8  album_slot;
extern u8  album_work[];
extern u8  sys_idx_tbl[];
extern u8  album_chr[];
extern s32 album_info_disp_tbl0[];
extern s32 album_info_disp_tbl1[];
extern u8  name_ent_album_info_disp_str0[];
extern u8  name_ent_album_info_disp_str1[];

void name_ent_album_info_disp(void)
{
    s32 unk_top[2];
    u8 buf[256];
    s32 unk_bot[2];
    s32 idx;

    if (card_sys_present != 0) {
        strncpy(buf, album_info_disp_tbl0[last_album], 5);
        strcat(buf, name_ent_album_info_disp_str0);
        set_kanji_string(-32, -92, 15, buf, 0);

        idx = last_album;
        set_kanji_string(32, -75, 15, &album_work[idx * 8 + 2184], 0);

        idx = last_album;
        set_kanji_string(78, -75, 15, &album_work[idx * 8 + 2312], 0);

        idx = last_album;
        set_kanji_string(32, -59, 15, &album_work[idx * 16 + 2440], 0);

        idx = last_album;
        set_kanji_string(32, -43, 15,
                      (void *)album_info_disp_tbl1[sys_idx_tbl[idx]], 0);
    } else {
        strncpy(buf, album_info_disp_tbl0[now_album], 5);
        strcat(buf, name_ent_album_info_disp_str1);
        set_kanji_string(-32, -92, 15, buf, 0);

        set_kanji_string(32, -75, 15,
                      &album_chr[album_slot * 3840 + 668], 0);
        set_kanji_string(78, -75, 15,
                      &album_chr[album_slot * 3840 + 676], 0);
        set_kanji_string(32, -59, 15,
                      &album_chr[album_slot * 3840 + 684], 0);
        set_kanji_string(32, -43, 15,
                      (void *)album_info_disp_tbl1[
                          (u32)*(u16 *)&album_chr[album_slot * 3840 + 700] >> 12], 0);
    }
}
