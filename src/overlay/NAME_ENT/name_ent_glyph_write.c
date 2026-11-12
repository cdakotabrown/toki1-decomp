/* @SymbolStart: 0x80134254 */
/* @SymbolSize: 2256 */
/* @CompileFlags: -cckr */
#include "types.h"

extern u8 name_ent_cell_no;
extern s32 name_ent_upper_col;
extern s32 name_ent_upper_row;
extern s32 name_ent_yomi_no;
extern s32 name_ent_tab_no;
extern s32 name_ent_kanji_row;
extern u8 name_ent_sei[];
extern u8 name_ent_mei[];
extern u8 name_ent_adana[];
extern u8 space_str[];
extern u8 *name_ent_hira_tbl[];
extern u8 *name_ent_kata_tbl[];
extern u8 *name_ent_yomi_tbl[];
extern u16 *name_ent_kanji_cnt;
extern u8 **name_ent_kanji_txt;

extern s32 strlen();

#define BLANK() { base[a1 * 2] = space_str[0]; base[a1 * 2 + 1] = space_str[1]; }

void name_ent_glyph_write(sel)
s32 sel;
{
    s32 a1;
    s32 unused1;
    s32 unused2;
    u8 *base;
    s32 idx;
    s32 unused3;
    s32 unused4;
    s32 unused5;

    a1 = name_ent_cell_no;
    switch (sel) {
    case 0:
        if (a1 >= 3) a1 = 2;
        break;
    case 1:
        a1 = name_ent_cell_no - 3;
        if (a1 >= 3) a1 = 2;
        break;
    case 2:
        a1 = name_ent_cell_no - 6;
        if (a1 >= 5) a1 = 4;
        break;
    }
    switch (sel) {
    case 0:
        base = name_ent_sei;
        break;
    case 1:
        base = name_ent_mei;
        break;
    case 2:
        base = name_ent_adana;
        break;
    }
    switch (name_ent_tab_no) {
    case 0:
        if (name_ent_upper_col % 6 == 5) {
            BLANK()
        } else if (name_ent_upper_row == 2 && (name_ent_upper_col == 9 || name_ent_upper_col == 10)) {
            BLANK()
        } else if (name_ent_upper_row == 3 && (name_ent_upper_col == 3 || name_ent_upper_col == 4)) {
            BLANK()
        } else if (name_ent_upper_row == 5 && name_ent_upper_col >= 11) {
            BLANK()
        } else {
            base[a1 * 2] = name_ent_hira_tbl[name_ent_upper_col / 6 + name_ent_upper_row * 3][name_ent_upper_col % 6 * 2];
            base[a1 * 2 + 1] = name_ent_hira_tbl[name_ent_upper_col / 6 + name_ent_upper_row * 3][name_ent_upper_col % 6 * 2 + 1];
        }
        break;
    case 1:
        if (name_ent_upper_col % 6 == 5) {
            BLANK()
        } else if (name_ent_upper_row == 2 && (name_ent_upper_col == 9 || name_ent_upper_col == 10)) {
            BLANK()
        } else if (name_ent_upper_row == 3 && (name_ent_upper_col == 3 || name_ent_upper_col == 4)) {
            BLANK()
        } else if (name_ent_upper_row == 5 && name_ent_upper_col >= 11) {
            BLANK()
        } else {
            base[a1 * 2] = name_ent_kata_tbl[name_ent_upper_col / 6 + name_ent_upper_row * 3][name_ent_upper_col % 6 * 2];
            base[a1 * 2 + 1] = name_ent_kata_tbl[name_ent_upper_col / 6 + name_ent_upper_row * 3][name_ent_upper_col % 6 * 2 + 1];
        }
        break;
    case 2:
        if (name_ent_upper_col >= 16) break;
        idx = name_ent_upper_row * 16 + name_ent_kanji_row * 16 + name_ent_upper_col;
        if (idx < name_ent_kanji_cnt[38]) {
            base[a1 * 2] = name_ent_kanji_txt[38][idx * 2];
            base[a1 * 2 + 1] = name_ent_kanji_txt[38][idx * 2 + 1];
        } else {
            BLANK()
        }
        break;
    case 3:
        if (name_ent_upper_col >= 16) break;
        if (name_ent_yomi_no == 38) {
            idx = name_ent_upper_row * 16 + name_ent_kanji_row * 16 + name_ent_upper_col;
            if (idx < name_ent_kanji_cnt[name_ent_yomi_no]) {
                base[a1 * 2] = name_ent_kanji_txt[name_ent_yomi_no][idx * 2];
                base[a1 * 2 + 1] = name_ent_kanji_txt[name_ent_yomi_no][idx * 2 + 1];
            } else {
                BLANK()
            }
        } else if (name_ent_upper_row == 0) {
            if (name_ent_upper_col < strlen(name_ent_yomi_tbl[name_ent_yomi_no]) / 2) {
                base[a1 * 2] = name_ent_yomi_tbl[name_ent_yomi_no][name_ent_upper_col * 2];
                base[a1 * 2 + 1] = name_ent_yomi_tbl[name_ent_yomi_no][name_ent_upper_col * 2 + 1];
            } else {
                BLANK()
            }
        } else {
            idx = name_ent_upper_row * 16 + name_ent_kanji_row * 16 + name_ent_upper_col - 16;
            if (idx < name_ent_kanji_cnt[name_ent_yomi_no]) {
                base[a1 * 2] = name_ent_kanji_txt[name_ent_yomi_no][idx * 2];
                base[a1 * 2 + 1] = name_ent_kanji_txt[name_ent_yomi_no][idx * 2 + 1];
            } else {
                BLANK()
            }
        }
        break;
    }
    name_ent_cell_no++;
}
