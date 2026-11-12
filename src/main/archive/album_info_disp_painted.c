/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: custom_script custom_func_80069788.py */
/* Address: 0x80069788 */
/* Size: 2408 bytes (602 instructions, main body only) */
/* Region: Game (0x80041000-0x80085FFF) */
#include "types.h"

extern void k_sub_reset(void);
extern void k_sub_disp_start(s32);
extern void set_kanji_string(s32, s32, s32, void*, s32);
extern void strcpy(void*, void*);
extern void strncat(void*, void*, s32);

extern void* album_info_disp_tbl0[];
extern u8 album_info_disp_str0[];
extern u8 album_info_disp_str1[];
extern u8 album_info_disp_str2[];
extern u8 album_info_disp_str3[];
extern u8 album_info_disp_str4[];
extern u8 album_info_disp_str5[];
extern u8 album_info_disp_str6[];
extern u8 album_info_disp_str7[];
extern u8 album_info_disp_str8[];
extern u8 album_info_disp_str9[];
extern u8 album_info_disp_str10[];
extern u8 album_info_disp_str11[];
extern u8 album_info_disp_str12[];
extern u8 album_info_disp_str13[];
extern u8 album_info_disp_str14[];
extern u8 album_info_disp_str15[];
extern u8 album_info_disp_str16[];
extern u8 album_info_disp_str17[];
extern u8 album_info_disp_str18[];
extern u8 album_info_disp_str19[];
extern u8 album_info_disp_str20[];
extern void* album_info_disp_tbl1[];

extern u8 card_sys_present;
extern u8 last_album;
extern u8 D_800F0216b;
extern u8 album_work;
extern u8 album_slot;
extern u8 now_album;
extern u8 album_chr;

void album_info_disp(s32 arg0, s32 arg1)
{
    s32 spEC;
    u8 buf_C8[32];
    u8 buf_A8[32];
    u8 buf_88[32];
    u8 buf_68[32];
    u8 buf_48[32];
    s32 sp40;
    s32 s0;

    if (arg1 != 0) {
        spEC = -140;
    } else {
        spEC = -8;
    }

    k_sub_reset();

    if (card_sys_present != 0) {
        u8 *v0;
        u8 *v1;
        u8 val;

        s0 = 0;
        v1 = &album_work + 13;
        v0 = &album_work;
        do {
            if (v0[20] != 0) {
                s0 = s0 + 1;
            }
            v0 = v0 + 1;
        } while (v0 != v1);

        strcpy(buf_C8, album_info_disp_tbl0[last_album]);
        strncat(buf_C8, album_info_disp_str0, 2);
        set_kanji_string((s16)(spEC + 12), -80, 15, buf_C8, 0);

        if (last_album == 0) {
            set_kanji_string(*(s16*)&spEC, -64, 15, album_info_disp_str1, 0);
        } else {
            v0 = &album_work + D_800F0216b;
            val = v0[4];

            if (val == 0) {
                set_kanji_string(*(s16*)&spEC, -64, 15, album_info_disp_str2, 0);
            } else if (val == 255) {
                set_kanji_string(*(s16*)&spEC, -64, 15, album_info_disp_str3, 0);
            } else {
                strcpy(buf_C8, album_info_disp_tbl0[v0[2136] / 10U]);
                strncat(buf_C8, album_info_disp_tbl0[(&album_work)[last_album + 2632] % 10U], 2);
                strncat(buf_C8, album_info_disp_str4, 2);
                strncat(buf_C8, album_info_disp_tbl0[(&album_work)[last_album + 2648]], 4);
                strncat(buf_C8, album_info_disp_str5, 2);

                val = (&album_work)[last_album + 2664];
                if (val >= 10U) {
                    strncat(buf_C8, album_info_disp_tbl0[val / 10U], 2);
                    val = (&album_work)[last_album + 2664];
                    strncat(buf_C8, album_info_disp_tbl0[val % 10U], 2);
                }
                strncat(buf_C8, album_info_disp_str6, 2);

                sp40 = *(s16*)&spEC;
                set_kanji_string(*(s16*)&spEC, -64, 15, buf_C8, 0);

                strcpy(buf_A8, album_info_disp_str7);
                strncat(buf_A8, &album_work + last_album * 8 + 2184, 6);
                strncat(buf_A8, &album_work + last_album * 8 + 2312, 6);

                strcpy(buf_88, album_info_disp_str8);
                strncat(buf_88, &album_work + last_album * 16 + 2440, 13);

                set_kanji_string(*(s16*)&sp40, -48, 15, buf_A8, 0);
                set_kanji_string(*(s16*)&sp40, -32, 15, buf_88, 0);

                strcpy(buf_68, album_info_disp_str9);
                strncat(buf_68, album_info_disp_tbl1[(&album_work)[last_album + 4280]], 0);
                set_kanji_string(*(s16*)&sp40, -16, 15, buf_68, 0);

                strcpy(buf_48, album_info_disp_str10);
                if (s0 >= 100) {
                    s0 = 99;
                }
                if (s0 >= 11) {
                    strncat(buf_48, album_info_disp_tbl0[s0 / 10], 2);
                    strncat(buf_48, album_info_disp_tbl0[s0 % 10], 2);
                }
                set_kanji_string(*(s16*)&sp40, 0, 15, buf_48, 0);
            }
        }
    } else {
        u8 *v0;
        s32 v1;
        u8 val;

        s0 = 0;
        v0 = &album_chr + (u32)album_slot * 3840;
        v1 = 0;
        do {
            if (v0[1672] != 0) {
                s0 = s0 + 1;
            }
            v1 = v1 + 1;
            v0 = v0 + 1;
        } while (v1 != 13);

        val = (&album_work)[now_album + 4];

        if (val == 0) {
            set_kanji_string(*(s16*)&spEC, -64, 15, album_info_disp_str11, 0);
        } else if (val == 255) {
            set_kanji_string(*(s16*)&spEC, -64, 15, album_info_disp_str12, 0);
        } else {
            strcpy(buf_C8, album_info_disp_tbl0[now_album]);
            strncat(buf_C8, album_info_disp_str13, 2);
            set_kanji_string((s16)(spEC + 12), -80, 15, buf_C8, 0);

            strcpy(buf_C8, album_info_disp_tbl0[(&album_chr)[(u32)album_slot * 3840 + 0x205] / 10U]);
            strncat(buf_C8, album_info_disp_tbl0[(&album_chr)[(u32)album_slot * 3840 + 0x205] % 10U], 2);
            strncat(buf_C8, album_info_disp_str14, 2);

            strncat(buf_C8, album_info_disp_tbl0[(&album_chr)[(u32)album_slot * 3840 + 0x206]], 4);
            strncat(buf_C8, album_info_disp_str15, 2);

            val = (&album_chr)[(u32)album_slot * 3840 + 0x207];
            if (val >= 10U) {
                strncat(buf_C8, album_info_disp_tbl0[val / 10U], 2);
                val = (&album_chr)[(u32)album_slot * 3840 + 0x207];
                strncat(buf_C8, album_info_disp_tbl0[val % 10U], 2);
            }
            strncat(buf_C8, album_info_disp_str16, 2);

            sp40 = *(s16*)&spEC;
            set_kanji_string(*(s16*)&spEC, -64, 15, buf_C8, 0);

            strcpy(buf_A8, album_info_disp_str17);
            strncat(buf_A8, &album_chr + (u32)album_slot * 3840 + 668, 6);
            strncat(buf_A8, &album_chr + (u32)album_slot * 3840 + 676, 6);

            strcpy(buf_88, album_info_disp_str18);
            strncat(buf_88, &album_chr + (u32)album_slot * 3840 + 684, 13);

            set_kanji_string(*(s16*)&sp40, -48, 15, buf_A8, 0);
            set_kanji_string(*(s16*)&sp40, -32, 15, buf_88, 0);

            strcpy(buf_68, album_info_disp_str19);
            {
                u32 hval;
                hval = *(u16*)(&album_chr + (u32)album_slot * 3840 + 0x2BC);
                strncat(buf_68, album_info_disp_tbl1[hval >> 12], 0);
            }
            set_kanji_string(*(s16*)&sp40, -16, 15, buf_68, 0);

            strcpy(buf_48, album_info_disp_str20);
            if (s0 >= 100) {
                s0 = 99;
            }
            if (s0 >= 11) {
                strncat(buf_48, album_info_disp_tbl0[s0 / 10], 2);
                strncat(buf_48, album_info_disp_tbl0[s0 % 10], 2);
            }
            set_kanji_string(*(s16*)&sp40, 0, 15, buf_48, 0);
        }
    }

    k_sub_disp_start(6);
}
