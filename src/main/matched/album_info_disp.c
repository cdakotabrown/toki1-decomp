/* Address: 0x80069788 */
/* Size: 2408 bytes (602 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @CompileEnv: IDO52_UGEN_TEMP_REG=47:t3,49:t7,60:t5,61:t6,59:t4 */
/* @CompileFlags: -Wc,-pic1 -cckr */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: swap_instructions --offset1 0x38 --offset2 0x40 */
/* @PostProc: swap_instructions --offset1 0x38 --offset2 0x4ac */
/* @PostProc: swap_instructions --offset1 0x4ac --offset2 0x4b0 */
/* @PostProc: swap_instructions --offset1 0x4b0 --offset2 0x4b4 */
/* @PostProc: swap_instructions --offset1 0x4b4 --offset2 0x4b8 */
/* @PostProc: swap_instructions --offset1 0x4b8 --offset2 0x4bc */
/* @PostProc: swap_instructions --offset1 0x4bc --offset2 0x4c0 */
/* @PostProc: swap_instructions --offset1 0x4c0 --offset2 0x4c4 */
/* @PostProc: swap_instructions --offset1 0x4c4 --offset2 0x4c8 */
/* @PostProc: patch_insn_reg_field 0x38:rt:t3:a3 0x4ac:rs:t3:a3 0x4ac:rt:t3:a3 0x4b0:rt:t7:t3 0x4b4:rt:t3:a3 0x4b8:rt:t3:a3 0x4c0:rs:t7:t3 0x4c0:rt:t7:t3 0x4c4:rt:t7:t3 0x5a4:rt:t9:a3 0x5a8:rs:t9:a3 0x5a8:rt:t9:a3 0x5ac:rt:t1:t9 0x5b0:rt:t9:a3 0x5b0:rd:t0:t8 0x5b4:rs:t0:t8 0x5b4:rt:t9:a3 0x5b4:rd:t0:t8 0x5b8:rt:t0:t8 0x5b8:rd:t0:t8 0x5bc:rs:t1:t9 0x5bc:rt:t0:t8 0x5bc:rd:t1:t9 0x5c0:rs:t1:t9 0x5c0:rt:t1:t9 0x5c8:rs:t1:t9 0x5d4:rd:t3:t0 0x5d8:rt:t3:t0 0x5d8:rd:t2:t1 0x5dc:rt:t2:t1 0x5ec:rt:t4:a3 0x5f0:rs:t4:a3 0x5f0:rt:t4:a3 0x5f4:rt:t6:t3 0x5f8:rt:t4:a3 0x5f8:rd:t5:t2 0x5fc:rs:t5:t2 0x5fc:rt:t4:a3 0x5fc:rd:t5:t2 0x600:rt:t5:t2 0x600:rd:t5:t2 0x604:rs:t6:t3 0x604:rt:t5:t2 0x604:rd:t6:t3 0x608:rs:t6:t3 0x608:rt:t6:t3 0x610:rs:t6:t3 0x620:rd:t7:t4 0x624:rt:t7:t4 0x624:rd:t8:t5 0x628:rt:t8:t5 0x64c:rt:t9:a3 0x650:rs:t9:a3 0x650:rt:t9:a3 0x654:rt:t1:t7 0x658:rt:t9:a3 0x658:rd:t0:t6 0x65c:rs:t0:t6 0x65c:rt:t9:a3 0x65c:rd:t0:t6 0x660:rt:t0:t6 0x660:rd:t0:t6 0x664:rs:t1:t7 0x664:rt:t0:t6 0x664:rd:t1:t7 0x668:rs:t1:t7 0x668:rt:t1:t7 0x670:rt:t1:t7 0x670:rd:t3:t8 0x674:rt:t3:t8 0x69c:rt:t2:a3 0x6a0:rs:t2:a3 0x6a0:rt:t2:a3 0x6a8:rt:t2:a3 0x6a8:rd:t4:t9 0x6ac:rs:t4:t9 0x6ac:rt:t2:a3 0x6ac:rd:t4:t9 0x6b0:rt:t4:t9 0x6b0:rd:t4:t9 0x6b4:rt:t4:t9 0x6dc:rd:t5:t0 0x6e0:rt:t5:t0 0x6e0:rd:t6:t1 0x6e4:rt:t6:t1 0x6f4:rt:t7:a3 0x6f8:rs:t7:a3 0x6f8:rt:t7:a3 0x700:rt:t7:a3 0x700:rd:t8:t2 0x704:rs:t8:t2 0x704:rt:t7:a3 0x704:rd:t8:t2 0x708:rt:t8:t2 0x708:rd:t8:t2 0x70c:rt:t8:t2 0x72c:rd:t9:t3 0x730:rt:t9:t3 0x730:rd:t0:t4 0x734:rt:t0:t4 0x784:rt:t1:a3 0x788:rs:t1:a3 0x788:rt:t1:a3 0x78c:rt:t4:t7 0x790:rt:t1:a3 0x790:rd:t3:t5 0x794:rs:t3:t5 0x794:rt:t1:a3 0x794:rd:t3:t5 0x798:rt:t3:t5 0x798:rd:t3:t5 0x79c:rs:t3:t5 0x79c:rt:t2:t6 0x7a0:rs:t4:t7 0x7a0:rt:t4:t7 0x7a4:rs:t2:t6 0x7a4:rt:t4:t7 0x7b4:rt:t5:a3 0x7b8:rs:t5:a3 0x7b8:rt:t5:a3 0x7bc:rt:t8:t0 0x7c0:rt:t5:a3 0x7c0:rd:t6:t8 0x7c4:rs:t6:t8 0x7c4:rt:t5:a3 0x7c4:rd:t6:t8 0x7c8:rt:t6:t8 0x7c8:rd:t6:t8 0x7cc:rs:t6:t8 0x7cc:rt:t7:t9 0x7d0:rs:t8:t0 0x7d0:rt:t8:t0 0x7d4:rs:t7:t9 0x7d4:rt:t8:t0 0x7f4:rt:t9:a3 0x7f8:rs:t9:a3 0x7f8:rt:t9:a3 0x800:rt:t9:a3 0x800:rd:t0:t1 0x804:rs:t0:t1 0x804:rt:t9:a3 0x804:rd:t0:t1 0x808:rt:t0:t1 0x808:rd:t0:t1 0x80c:rs:t0:t1 0x80c:rt:t1:t2 0x814:rs:t1:t2 0x864:rt:t2:a3 0x868:rs:t2:a3 0x868:rt:t2:a3 0x870:rt:t2:a3 0x874:rt:t2:a3 */
#include "types.h"

typedef struct {
    u8 pad0[4];
    u8 status[16];
    u8 got[13];
    u8 pad21[5];
    u8 idx;
    u8 pad27[2097];
    u8 age[16];
    u8 month[16];
    u8 day[16];
    u8 name1[16][8];
    u8 name2[16][8];
    u8 club[16][16];
    u8 pad[1088];
    u8 blood[16];
    u8 pad2[6];
    u8 mode;
} Info;

typedef struct {
    u8 pad0[517];
    u8 age;
    u8 month;
    u8 day;
    u8 pad1[148];
    u8 name1[8];
    u8 name2[8];
    u8 club[16];
    u16 bits;
    u8 pad2[970];
    u8 got[13];
    u8 pad3[2155];
} Chr;

extern Info album_work;
extern Chr album_chr[];
extern u8 album_slot;
extern u8 now_album;
extern u8 *album_info_disp_tbl0[];
extern u8 *album_info_disp_tbl1[];
extern char album_info_disp_str0[];
extern char album_info_disp_str1[];
extern char album_info_disp_str2[];
extern char album_info_disp_str3[];
extern char album_info_disp_str4[];
extern char album_info_disp_str5[];
extern char album_info_disp_str6[];
extern char album_info_disp_str7[];
extern char album_info_disp_str8[];
extern char album_info_disp_str9[];
extern char album_info_disp_str10[];
extern char album_info_disp_str11[];
extern char album_info_disp_str12[];
extern char album_info_disp_str13[];
extern char album_info_disp_str14[];
extern char album_info_disp_str15[];
extern char album_info_disp_str16[];
extern char album_info_disp_str17[];
extern char album_info_disp_str18[];
extern char album_info_disp_str19[];
extern char album_info_disp_str20[];

extern void k_sub_reset();
extern void k_sub_disp_start();
extern void set_kanji_string(s16, s16, s16, u8 *, s32);
extern void strcpy();
extern void strncat();

void album_info_disp(a0, a1)
s16 a0;
s32 a1;
{
    s32 n;
    s32 i;
    s32 pos;
    u8 buf200[36];
    u8 buf168[32];
    u8 buf136[32];
    u8 buf104[32];
    u8 buf72[32];
    u8 c;
    u8 d;

    if (a1 != 0) {
        pos = -140;
    } else {
        pos = -8;
    }
    k_sub_reset();
    if (album_work.mode != 0) {
        n = 0;
        for (i = 0; i < 13; i++) {
            if (album_work.got[i] != 0) {
                n++;
            }
        }
        strcpy(buf200, album_info_disp_tbl0[album_work.idx]);
        strncat(buf200, album_info_disp_str0, 2);
        set_kanji_string(pos + 12, -80, 15, buf200, 0);
        if (album_work.idx == 0) {
            set_kanji_string(pos, -64, 15, album_info_disp_str1, 0);
        } else if (album_work.status[album_work.idx] == 0) {
            set_kanji_string(pos, -64, 15, album_info_disp_str2, 0);
        } else if (album_work.status[album_work.idx] == 255) {
            set_kanji_string(pos, -64, 15, album_info_disp_str3, 0);
        } else {
            strcpy(buf200, album_info_disp_tbl0[album_work.age[album_work.idx] / 10]);
            strncat(buf200, album_info_disp_tbl0[album_work.age[album_work.idx] % 10], 2);
            strncat(buf200, album_info_disp_str4, 2);
            strncat(buf200, album_info_disp_tbl0[album_work.month[album_work.idx]], 4);
            strncat(buf200, album_info_disp_str5, 2);
            if (album_work.day[album_work.idx] >= 10) {
                strncat(buf200, album_info_disp_tbl0[album_work.day[album_work.idx] / 10], 2);
            }
            strncat(buf200, album_info_disp_tbl0[album_work.day[album_work.idx] % 10], 2);
            strncat(buf200, album_info_disp_str6, 2);
            set_kanji_string(pos, -64, 15, buf200, 0);
            strcpy(buf168, album_info_disp_str7);
            strncat(buf168, album_work.name1[album_work.idx], 6);
            strncat(buf168, album_work.name2[album_work.idx], 6);
            strcpy(buf136, album_info_disp_str8);
            strncat(buf136, album_work.club[album_work.idx], 13);
            set_kanji_string(pos, -48, 15, buf168, 0);
            set_kanji_string(pos, -32, 15, buf136, 0);
            strcpy(buf104, album_info_disp_str9);
            strncat(buf104, album_info_disp_tbl1[album_work.blood[album_work.idx]]);
            set_kanji_string(pos, -16, 15, buf104, 0);
            strcpy(buf72, album_info_disp_str10);
            if (n >= 100) {
                n = 99;
            }
            if (n >= 11) {
                strncat(buf72, album_info_disp_tbl0[n / 10], 2);
            }
            strncat(buf72, album_info_disp_tbl0[n % 10], 2);
            set_kanji_string(pos, 0, 15, buf72, 0);
        }
    } else {
        n = 0;
        for (i = 0; i < 13; i++) {
            if (album_chr[album_slot].got[i] != 0) {
                n++;
            }
        }
        if (album_work.status[now_album] == 0) {
            set_kanji_string(pos, -64, 15, album_info_disp_str11, 0);
        } else if (album_work.status[now_album] == 255) {
            set_kanji_string(pos, -64, 15, album_info_disp_str12, 0);
        } else {
            strcpy(buf200, album_info_disp_tbl0[now_album]);
            strncat(buf200, album_info_disp_str13, 2);
            set_kanji_string(pos + 12, -80, 15, buf200, 0);
            strcpy(buf200, album_info_disp_tbl0[album_chr[album_slot].age / 10]);
            strncat(buf200, album_info_disp_tbl0[album_chr[album_slot].age % 10], 2);
            strncat(buf200, album_info_disp_str14, 2);
            strncat(buf200, album_info_disp_tbl0[album_chr[album_slot].month], 4);
            strncat(buf200, album_info_disp_str15, 2);
            if (album_chr[album_slot].day >= 10) {
                strncat(buf200, album_info_disp_tbl0[album_chr[album_slot].day / 10], 2);
            }
            strncat(buf200, album_info_disp_tbl0[album_chr[album_slot].day % 10], 2);
            strncat(buf200, album_info_disp_str16, 2);
            set_kanji_string(pos, -64, 15, buf200, 0);
            strcpy(buf168, album_info_disp_str17);
            strncat(buf168, album_chr[album_slot].name1, 6);
            strncat(buf168, album_chr[album_slot].name2, 6);
            strcpy(buf136, album_info_disp_str18);
            strncat(buf136, album_chr[album_slot].club, 13);
            set_kanji_string(pos, -48, 15, buf168, 0);
            set_kanji_string(pos, -32, 15, buf136, 0);
            strcpy(buf104, album_info_disp_str19);
            strncat(buf104, album_info_disp_tbl1[album_chr[album_slot].bits >> 12]);
            set_kanji_string(pos, -16, 15, buf104, 0);
            strcpy(buf72, album_info_disp_str20);
            if (n >= 100) {
                n = 99;
            }
            if (n >= 11) {
                strncat(buf72, album_info_disp_tbl0[n / 10], 2);
            }
            strncat(buf72, album_info_disp_tbl0[n % 10], 2);
            set_kanji_string(pos, 0, 15, buf72, 0);
        }
    }
    k_sub_disp_start(6);
}
