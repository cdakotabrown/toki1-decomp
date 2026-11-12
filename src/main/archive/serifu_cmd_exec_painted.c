/* Function: serifu_cmd_exec */
/* Likely Symbol: step_wait (from o.bin) */
/* Address: 0x8007F08C */
/* Size: 8876 bytes (2219 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* Compiler: IDO 5.2 -O2 -G0 -mips1 -EL -non_shared */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: patch_stack_complete --old 640 --new 656 */
/* @PostProc: custom_script custom_func_8007F08C.py */
/* @PostProc: strip_trailing_instructions --count 1 --nop-only */
/* @PostProc: scheduler_normalizer */
/* @PostProc: custom_script remap_post_scheduler_8007F08C.py */
































































































#include "types.h"

extern void strcpy(u8 *dst, u8 *src);
extern void strncpy(u8 *dst, u8 *src, s32 count);
extern void sprintf(u8 *dst, u8 *fmt, u8 *src);
extern s16 strlen(u8 *str);
extern s32 strcmp(u8 *str1, u8 *str2);
extern void set_kanji_string(s16 a0, s16 a1, u8 a2, void *a3, s32 a4);
extern s32 msg_str_disp(s16 a0, s16 a1, u8 a2, void *a3, s32 a4, void *a5, void *a6, u8 a7);
extern void serifu_skip(void);
extern s32 device_cmd_send(s32 a0);
extern s32 get_CD_State(void);
extern void set_XAsector(s32 a0, s32 a1, s32 a2);
extern void cmd_que_clear(void);
extern void msg_order_set(void);
extern void msg_part_reorder(u8 *a0);
extern void k_disp_start(s32 a0);
extern s32 game_reset(void);
extern void sub_sub_step_up(void);

typedef struct { s32 val[4]; } PalTbl;
extern PalTbl serifu_cmd_exec_tbl56;
extern s32 serifu_voice;
extern s32 serifu_line;
extern u8 sound_seek_wait;
extern s16 sound_seek_time;
extern s32 serifu_state;
extern u8 now_girl;
extern u8 voice_level;
extern s32 serifu_no;
extern s32 serifu_wait;
extern s16 last_x_y;
extern u8 xcut;
extern u8 junban;
extern u8 menu3_select_tbl0;
extern u8 menu3_select_tbl1;
extern s32 msg_wait;
extern u8 p_name;
extern u8 g_name;

extern u8 serifu_cmd_exec_str0; extern u8 serifu_cmd_exec_str1; extern u8 serifu_cmd_exec_tbl0; extern u8 serifu_cmd_exec_tbl1;
extern u8 serifu_cmd_exec_str2; extern u8 serifu_cmd_exec_str3; extern u8 serifu_cmd_exec_tbl2; extern u8 serifu_cmd_exec_tbl3;
extern u8 serifu_cmd_exec_str4; extern u8 serifu_cmd_exec_str5; extern u8 serifu_cmd_exec_tbl4; extern u8 serifu_cmd_exec_tbl5;
extern u8 serifu_cmd_exec_str6; extern u8 serifu_cmd_exec_str7; extern u8 serifu_cmd_exec_str8; extern u8 serifu_cmd_exec_tbl6;
extern u8 serifu_cmd_exec_str9; extern u8 serifu_cmd_exec_tbl7; extern u8 serifu_cmd_exec_tbl8;
extern u8 serifu_cmd_exec_str10; extern u8 serifu_cmd_exec_str11; extern u8 serifu_cmd_exec_str12; extern u8 serifu_cmd_exec_str13;
extern u8 serifu_cmd_exec_tbl9; extern u8 serifu_cmd_exec_str14;
extern u8 serifu_cmd_exec_str15; extern u8 serifu_cmd_exec_tbl10; extern u8 serifu_cmd_exec_tbl11;
extern u8 serifu_cmd_exec_str16; extern u8 serifu_cmd_exec_tbl12; extern u8 serifu_cmd_exec_tbl13;
extern u8 serifu_cmd_exec_tbl15; extern u8 serifu_cmd_exec_tbl16;
extern u8 serifu_cmd_exec_tbl17; extern u8 serifu_cmd_exec_tbl18;
extern u8 serifu_cmd_exec_tbl19; extern u8 serifu_cmd_exec_str17;
extern u8 serifu_cmd_exec_tbl20; extern u8 serifu_cmd_exec_tbl21;
extern u8 serifu_cmd_exec_tbl22; extern u8 serifu_cmd_exec_tbl23;
extern u8 serifu_cmd_exec_str18; extern u8 serifu_cmd_exec_str19; extern u8 serifu_cmd_exec_tbl24; extern u8 serifu_cmd_exec_tbl25;
extern u8 serifu_cmd_exec_str20; extern u8 serifu_cmd_exec_str21; extern u8 serifu_cmd_exec_tbl26; extern u8 serifu_cmd_exec_tbl27;
extern u8 serifu_cmd_exec_str22; extern u8 serifu_cmd_exec_str23; extern u8 serifu_cmd_exec_str24; extern u8 serifu_cmd_exec_tbl28;
extern u8 serifu_cmd_exec_str25; extern u8 serifu_cmd_exec_str26; extern u8 serifu_cmd_exec_tbl29; extern u8 serifu_cmd_exec_tbl30;
extern u8 serifu_cmd_exec_str27; extern u8 serifu_cmd_exec_str28; extern u8 serifu_cmd_exec_tbl31; extern u8 serifu_cmd_exec_tbl32;
extern u8 serifu_cmd_exec_str29; extern u8 serifu_cmd_exec_str30; extern u8 serifu_cmd_exec_tbl33; extern u8 serifu_cmd_exec_tbl34;
extern u8 serifu_cmd_exec_str31; extern u8 serifu_cmd_exec_str32; extern u8 serifu_cmd_exec_tbl35; extern u8 serifu_cmd_exec_tbl36;
extern u8 serifu_cmd_exec_str33; extern u8 serifu_cmd_exec_str34; extern u8 serifu_cmd_exec_tbl37; extern u8 serifu_cmd_exec_tbl38;
extern u8 serifu_cmd_exec_tbl39; extern u8 serifu_cmd_exec_tbl40; extern u8 serifu_cmd_exec_str35;
extern u8 serifu_cmd_exec_tbl41; extern u8 serifu_cmd_exec_tbl42; extern u8 serifu_cmd_exec_str36;
extern u8 serifu_cmd_exec_tbl43; extern u8 serifu_cmd_exec_tbl44;
extern u8 serifu_cmd_exec_str37; extern u8 serifu_cmd_exec_str38; extern u8 serifu_cmd_exec_str39;
extern u8 serifu_cmd_exec_tbl45; extern u8 serifu_cmd_exec_tbl46; extern u8 serifu_cmd_exec_str40;
extern u8 serifu_cmd_exec_str41; extern u8 serifu_cmd_exec_str42; extern u8 serifu_cmd_exec_tbl47;
extern u8 serifu_cmd_exec_tbl48; extern u8 serifu_cmd_exec_str43; extern u8 serifu_cmd_exec_str44;
extern u8 serifu_cmd_exec_str45; extern u8 serifu_cmd_exec_tbl49; extern u8 serifu_cmd_exec_tbl50;
extern u8 serifu_cmd_exec_str46; extern u8 serifu_cmd_exec_str47; extern u8 serifu_cmd_exec_str48;
extern u8 serifu_cmd_exec_tbl51; extern u8 serifu_cmd_exec_tbl52; extern u8 serifu_cmd_exec_str49;
extern u8 serifu_cmd_exec_str50; extern u8 serifu_cmd_exec_str51; extern u8 serifu_cmd_exec_tbl53;
extern u8 serifu_cmd_exec_str52; extern u8 serifu_cmd_exec_str53; extern u8 serifu_cmd_exec_str54;
extern u8 serifu_cmd_exec_str55; extern u8 serifu_cmd_exec_str56; extern u8 serifu_cmd_exec_str57;
extern u8 serifu_cmd_exec_str58; extern u8 serifu_cmd_exec_str59; extern u8 serifu_cmd_exec_tbl54; extern u8 serifu_cmd_exec_tbl55;

s32 serifu_cmd_exec(u8 *a0_param, s32 a1_param, s32 a2_param, s32 *a3_param, u16 idx1, u16 idx2, u8 color) {
    u8 textbuf[508];
    u8 strbuf[8];
    u8 fmtbuf[12];
    PalTbl tbl;
    s16 slen;
    register u8 *savedptr;
    s32 v0val;
    s32 *t0_base;
    s32 t1_off;
    s32 *entry;
    s32 padret;
    register s16 ycoord;

    tbl = serifu_cmd_exec_tbl56;
    savedptr = &textbuf[(u32)a2_param * 0];

    if (*(u8 *)&a1_param != 0xFF) {
#line 1
        strcpy(savedptr, a0_param);
    }

#line 1
    serifu_voice = 0;
    ycoord = (s16)(serifu_line * 16 + 48);
    sound_seek_wait = 0;

    if (*(u8 *)&a1_param >= 20U) {
        if (*(u8 *)&a1_param == 0xFF) {
            sub_sub_step_up();
            return 0;
        }
        goto default_other;
    }

    switch (*(u8 *)&a1_param) {
    case 7:
        t0_base = (s32 *)((u8 *)a3_param + (idx1 << 2));
        t1_off = idx2 << 2;
        entry = (s32 *)(*((s32 *)((u8 *)(*t0_base) + t1_off)));
        if (entry[0] != 0) {
            goto case7_check_ee79c;
        }
        if (entry[1] == 0) {
            goto case7_text;
        }
    case7_check_ee79c:
        if (voice_level == 0) {
            goto case7_text;
        }
        if (sound_seek_time++ == 0) {
            if (serifu_state == 0) {
                entry = (s32 *)(*((s32 *)((u8 *)(*t0_base) + t1_off)));
                set_XAsector(entry[0], entry[1], ((u8 *)&tbl)[now_girl]);
                device_cmd_send(768);
            }
        }
        if (sound_seek_time < 1024) {
            padret = get_CD_State();
            if (!(padret & 4)) {
                if (serifu_state == 0) {
                    sound_seek_wait = 1;
                    serifu_skip();
                }
            }
            if (sound_seek_wait == 1) {
                return 0;
            }
        } else {
            sound_seek_time = 0;
            cmd_que_clear();
            device_cmd_send(116);
            return 100;
        }

    case7_set_and_text:
        serifu_voice = 1;
        sound_seek_time = 0;
    case7_text:
        if (textbuf[0] == 62) {
            strncpy(strbuf, &savedptr[1], 4);
            strbuf[4] = 0;
            sprintf(fmtbuf, &serifu_cmd_exec_str0, strbuf);
            set_kanji_string(-120, ycoord, color, fmtbuf, 0);
            set_kanji_string(-86, ycoord, color, &serifu_cmd_exec_str1, 0);
            msg_str_disp(-72, ycoord, color, &savedptr[5], 0, &serifu_cmd_exec_tbl0, &serifu_cmd_exec_tbl1, 0);
            goto set_flags;
        }
        if (textbuf[0] == 45) {
            strncpy(strbuf, &savedptr[1], 6);
            strbuf[6] = 0;
            sprintf(fmtbuf, &serifu_cmd_exec_str2, strbuf);
            set_kanji_string(-128, ycoord, color, fmtbuf, 0);
            set_kanji_string(-86, ycoord, color, &serifu_cmd_exec_str3, 0);
            msg_str_disp(-72, ycoord, color, &savedptr[7], 0, &serifu_cmd_exec_tbl2, &serifu_cmd_exec_tbl3, 0);
            goto set_flags;
        }
        if (textbuf[0] == 43) {
            strncpy(strbuf, &savedptr[1], 2);
            strbuf[2] = 0;
            sprintf(fmtbuf, &serifu_cmd_exec_str4, strbuf);
            set_kanji_string(-112, ycoord, color, fmtbuf, 0);
            set_kanji_string(-86, ycoord, color, &serifu_cmd_exec_str5, 0);
            msg_str_disp(-72, ycoord, color, &savedptr[3], 0, &serifu_cmd_exec_tbl4, &serifu_cmd_exec_tbl5, 0);
            goto set_flags;
        }
        if (textbuf[0] == 60) {
            strncpy(strbuf, &savedptr[1], 4);
            strbuf[4] = 0;
            sprintf(fmtbuf, &serifu_cmd_exec_str6, strbuf);
            set_kanji_string(-120, ycoord, color, fmtbuf, 0);
            set_kanji_string(-86, ycoord, color, &serifu_cmd_exec_str7, 0);
            msg_str_disp(-72, ycoord, color, &savedptr[5], 0, &serifu_cmd_exec_str8, &serifu_cmd_exec_tbl6, 0);
            goto set_flags;
        }
        if (strcmp(savedptr, &serifu_cmd_exec_str9) == 0) {
            msg_str_disp(-40, ycoord + 16, color, savedptr, 0, &serifu_cmd_exec_tbl7, &serifu_cmd_exec_tbl8, 0);
            goto set_flags;
        }
        if (strcmp(savedptr, &serifu_cmd_exec_str10) == 0 ||
            strcmp(savedptr, &serifu_cmd_exec_str11) == 0 ||
            strcmp(savedptr, &serifu_cmd_exec_str12) == 0) {
            msg_str_disp(-128, ycoord, 2, savedptr, 0, &serifu_cmd_exec_tbl9, &serifu_cmd_exec_str14, 0);
            goto set_flags;
        }
        if (strcmp(savedptr, &serifu_cmd_exec_str13) != 0) {
            if (strcmp(savedptr, &serifu_cmd_exec_str15) == 0) {
                msg_str_disp(-128, ycoord, 1, savedptr, 0, &serifu_cmd_exec_tbl10, &serifu_cmd_exec_tbl11, 0);
                goto set_flags;
            }
            if (strcmp(savedptr, &serifu_cmd_exec_str16) == 0) {
                msg_str_disp(-40, ycoord + 16, 2, savedptr, 0, &serifu_cmd_exec_tbl12, &serifu_cmd_exec_tbl13, 0);
                goto set_flags;
            }
            msg_str_disp(-128, ycoord, color, savedptr, 0, &serifu_cmd_exec_tbl15, &serifu_cmd_exec_tbl16, 0);
            serifu_state = 1;
            serifu_voice = 1;
            sound_seek_time = 0;
            return 0;
        }
    set_flags:
        serifu_state = 1;
        return 0;

    case 16:
        msg_str_disp(-128, ycoord, color, savedptr, 0, &serifu_cmd_exec_tbl16, &serifu_cmd_exec_tbl17, 0);
        serifu_wait = 1;
        return 0;

    case 17:
        msg_str_disp(-72, last_x_y, color, savedptr, 0, &serifu_cmd_exec_tbl18, &serifu_cmd_exec_tbl19, 0);
        serifu_wait = 1;
        return 0;

    case 9:
        if (textbuf[0] == 0) {
            return 0;
        }
        if (strcmp(savedptr, &serifu_cmd_exec_str17) == 0) {
            msg_str_disp(-128, ycoord, 1, savedptr, 0, &serifu_cmd_exec_tbl20, &serifu_cmd_exec_tbl21, 0);
        }
        msg_str_disp(-128, ycoord, color, savedptr, 0, &serifu_cmd_exec_tbl22, &serifu_cmd_exec_tbl23, 0);
        return 0;

    case 0:
        slen = strlen(savedptr);
        if (savedptr[slen - 1] == 110 && savedptr[slen - 2] == 110) {
            if (serifu_no != 0) {
                return 0;
            }
            savedptr[slen - 2] = 0;
            slen = strlen(&p_name);
            sprintf(fmtbuf, &serifu_cmd_exec_str18, &p_name);
            set_kanji_string(-104 - slen * 4, ycoord, color, fmtbuf, 0);
            set_kanji_string(-86, ycoord, color, &serifu_cmd_exec_str19, 0);
            msg_str_disp(-72, ycoord, color, savedptr, 0, &serifu_cmd_exec_tbl24, &serifu_cmd_exec_tbl25, 0);
            goto set_flags;
        }
        slen = strlen(&p_name);
        sprintf(fmtbuf, &serifu_cmd_exec_str20, &p_name);
        set_kanji_string(-104 - slen * 4, ycoord, color, fmtbuf, 0);
        set_kanji_string(-86, ycoord, color, &serifu_cmd_exec_str21, 0);
        msg_str_disp(-72, ycoord, color, savedptr, 0, &serifu_cmd_exec_tbl26, &serifu_cmd_exec_tbl27, 0);
        goto set_flags;

    case 14:
        slen = strlen(&p_name);
        sprintf(fmtbuf, &serifu_cmd_exec_str22, &p_name);
        set_kanji_string(-104 - slen * 4, ycoord, color, fmtbuf, 0);
        set_kanji_string(-86, ycoord, color, &serifu_cmd_exec_str23, 0);
        msg_str_disp(-72, ycoord, color, savedptr, 0, &serifu_cmd_exec_str24, &serifu_cmd_exec_tbl28, 0);
        return 0;

    case 1:
        t0_base = (s32 *)((u8 *)a3_param + (idx1 << 2));
        t1_off = idx2 << 2;
        entry = (s32 *)(*((s32 *)((u8 *)(*t0_base) + t1_off)));
        if (entry[0] != 0) {
            goto case1_ee79c;
        }
        if (entry[1] == 0) {
            goto case1_text;
        }
    case1_ee79c:
        if (voice_level == 0) {
            goto case1_text;
        }
        if (sound_seek_time++ == 0) {

            if (serifu_state == 0) {
                entry = (s32 *)(*((s32 *)((u8 *)(*t0_base) + t1_off)));
                set_XAsector(entry[0], entry[1], ((u8 *)&tbl)[now_girl]);
                device_cmd_send(768);
            }
        }
        do {
            if (sound_seek_time >= 1024) goto do_cleanup_1;
            padret = get_CD_State();
            if (!(padret & 4)) {
                if (serifu_state == 0) {
                    sound_seek_wait = 1;
                    serifu_skip();
                }
            }
            if (sound_seek_wait == 1) {
                return 0;
            }
            break;
        do_cleanup_1:
                sound_seek_time = 0;
                cmd_que_clear();
                device_cmd_send(116);
                return 100;
        } while(0);

    case1_set_and_text:
        serifu_voice = 1;
        sound_seek_time = 0;
    case1_text:
        slen = strlen(&g_name);
        sprintf(fmtbuf, &serifu_cmd_exec_str25, &g_name);
        set_kanji_string(-104 - slen * 4, ycoord, color, fmtbuf, 0);
        set_kanji_string(-86, ycoord, color, &serifu_cmd_exec_str26, 0);
        msg_str_disp(-72, ycoord, color, savedptr, 0, &serifu_cmd_exec_tbl29, &serifu_cmd_exec_tbl30, 0);
        goto set_flags;

    case 15:
        xcut = 0;
        slen = strlen(savedptr);
        t0_base = (s32 *)((u8 *)a3_param + (idx1 << 2));
        t1_off = idx2 << 2;
        entry = (s32 *)(*((s32 *)((u8 *)(*t0_base) + t1_off)));
        if (entry[0] != 0) {
            goto case15_ee79c;
        }
        if (entry[1] == 0) {
            goto case15_text;
        }
    case15_ee79c:
        if (voice_level == 0) {
            goto case15_text;
        }
        if (sound_seek_time++ == 0) {

            if (serifu_state == 0) {
                entry = (s32 *)(*((s32 *)((u8 *)(*t0_base) + t1_off)));
                set_XAsector(entry[0], entry[1], ((u8 *)&tbl)[now_girl]);
                device_cmd_send(768);
            }
        }
        do {
            if (sound_seek_time >= 1024) goto do_cleanup_2;
            padret = get_CD_State();
            if (!(padret & 4)) {
                if (serifu_state == 0) {
                    sound_seek_wait = 1;
                    serifu_skip();
                }
            }
            if (sound_seek_wait == 1) {
                return 0;
            }
            break;
        do_cleanup_2:
                sound_seek_time = 0;
                cmd_que_clear();
                device_cmd_send(116);
                return 100;
        } while(0);

    case15_set_and_text:
        serifu_voice = 1;
        sound_seek_time = 0;
    case15_text:
        if (savedptr[slen - 1] == 110 && savedptr[slen - 2] == 110) {
            if (serifu_no != 0) {
                serifu_voice = 1;
                sound_seek_time = 0;
                return 0;
            }
            savedptr[slen - 2] = 0;
            slen = strlen(&g_name);
            sprintf(fmtbuf, &serifu_cmd_exec_str27, &g_name);
            set_kanji_string(-104 - slen * 4, ycoord, color, fmtbuf, 0);
            set_kanji_string(-86, ycoord, color, &serifu_cmd_exec_str28, 0);
            msg_str_disp(-72, ycoord, color, savedptr, 0, &serifu_cmd_exec_tbl31, &serifu_cmd_exec_tbl32, 0);
        } else {
            slen = strlen(&g_name);
            sprintf(fmtbuf, &serifu_cmd_exec_str29, &g_name);
            set_kanji_string(-104 - slen * 4, ycoord, color, fmtbuf, 0);
            set_kanji_string(-86, ycoord, color, &serifu_cmd_exec_str30, 0);
            msg_str_disp(-72, ycoord, color, savedptr, 0, &serifu_cmd_exec_tbl33, &serifu_cmd_exec_tbl34, 0);
        }
        goto set_flags;

    case 12:
        xcut = 0;
        slen = strlen(savedptr);
        t0_base = (s32 *)((u8 *)a3_param + (idx1 << 2));
        t1_off = idx2 << 2;
        entry = (s32 *)(*((s32 *)((u8 *)(*t0_base) + t1_off)));
        if (entry[0] != 0) {
            goto case12_ee79c;
        }
        if (entry[1] == 0) {
            goto case12_text;
        }
    case12_ee79c:
        if (voice_level == 0) {
            goto case12_text;
        }
        if (sound_seek_time++ == 0) {

            if (serifu_state == 0) {
                entry = (s32 *)(*((s32 *)((u8 *)(*t0_base) + t1_off)));
                set_XAsector(entry[0], entry[1], ((u8 *)&tbl)[now_girl]);
                device_cmd_send(768);
            }
        }
        do {
            if (sound_seek_time >= 1024) goto do_cleanup_3;
            padret = get_CD_State();
            if (!(padret & 4)) {
                if (serifu_state == 0) {
                    sound_seek_wait = 1;
                    serifu_skip();
                }
            }
            if (sound_seek_wait == 1) {
                return 0;
            }
            break;
        do_cleanup_3:
                sound_seek_time = 0;
                cmd_que_clear();
                device_cmd_send(116);
                return 100;
        } while(0);

    case12_set_and_text:
        serifu_voice = 1;
        sound_seek_time = 0;
    case12_text:
        if (savedptr[slen - 1] == 110 && savedptr[slen - 2] == 110) {
            if (serifu_no != 0) {
                serifu_voice = 1;
                sound_seek_time = 0;
                return 0;
            }
            savedptr[slen - 2] = 0;
            slen = strlen(&g_name);
            sprintf(fmtbuf, &serifu_cmd_exec_str31, &g_name);
            set_kanji_string(-104 - slen * 4, ycoord, color, fmtbuf, 0);
            set_kanji_string(-86, ycoord, color, &serifu_cmd_exec_str32, 0);
            msg_str_disp(-72, ycoord, color, savedptr, 0, &serifu_cmd_exec_tbl35, &serifu_cmd_exec_tbl36, 0);
            serifu_wait = 1;
        } else {
            slen = strlen(&g_name);
            sprintf(fmtbuf, &serifu_cmd_exec_str33, &g_name);
            set_kanji_string(-104 - slen * 4, ycoord, color, fmtbuf, 0);
            set_kanji_string(-86, ycoord, color, &serifu_cmd_exec_str34, 0);
            msg_str_disp(-72, ycoord, color, savedptr, 0, &serifu_cmd_exec_tbl37, &serifu_cmd_exec_tbl38, 0);
            serifu_wait = 1;
        }
        return 0;

    case 13:
        slen = strlen(savedptr);
        t0_base = (s32 *)((u8 *)a3_param + (idx1 << 2));
        t1_off = idx2 << 2;
        entry = (s32 *)(*((s32 *)((u8 *)(*t0_base) + t1_off)));
        if (entry[0] != 0) {
            goto case13_ee79c;
        }
        if (entry[1] == 0) {
            goto case13_text;
        }
    case13_ee79c:
        if (voice_level == 0 || xcut != 0) {
            goto case13_text;
        }
        if (sound_seek_time++ == 0) {

            if (serifu_state == 0) {
                entry = (s32 *)(*((s32 *)((u8 *)(*t0_base) + t1_off)));
                set_XAsector(entry[0], entry[1], ((u8 *)&tbl)[now_girl]);
                device_cmd_send(768);
            }
        }
        do {
            if (sound_seek_time >= 1024) goto do_cleanup_4;
            padret = get_CD_State();
            if (!(padret & 4)) {
                if (serifu_state == 0) {
                    sound_seek_wait = 1;
                    serifu_skip();
                }
            }
            if (sound_seek_wait == 1) {
                return 0;
            }
            break;
        do_cleanup_4:
                sound_seek_time = 0;
                cmd_que_clear();
                device_cmd_send(116);
                return 100;
        } while(0);

    case13_set_and_text:
        serifu_voice = 1;
        sound_seek_time = 0;
    case13_text:
        if (savedptr[slen - 1] == 110 && savedptr[slen - 2] == 110) {
            if (serifu_no != 0) {
                return 0;
            }
            savedptr[slen - 2] = 0;
            msg_str_disp(-72, last_x_y, color, savedptr, 0, &serifu_cmd_exec_tbl39, &serifu_cmd_exec_tbl40, 0);
            serifu_state = 1;
            return 0;
        }
        set_kanji_string(-86, ycoord, color, &serifu_cmd_exec_str35, 0);
        msg_str_disp(-72, last_x_y, color, savedptr, 0, &serifu_cmd_exec_tbl41, &serifu_cmd_exec_tbl42, 0);
        serifu_state = 1;
        return 0;

    case 18:
        slen = strlen(savedptr);
        t0_base = (s32 *)((u8 *)a3_param + (idx1 << 2));
        t1_off = idx2 << 2;
        entry = (s32 *)(*((s32 *)((u8 *)(*t0_base) + t1_off)));
        if (entry[0] != 0) {
            goto case18_ee79c;
        }
        if (entry[1] == 0) {
            goto case18_text;
        }
    case18_ee79c:
        if (voice_level == 0 || xcut != 0) {
            goto case18_text;
        }
        if (sound_seek_time++ == 0) {

            if (serifu_state == 0) {
                entry = (s32 *)(*((s32 *)((u8 *)(*t0_base) + t1_off)));
                set_XAsector(entry[0], entry[1], ((u8 *)&tbl)[now_girl]);
                device_cmd_send(768);
            }
        }
        do {
            if (sound_seek_time >= 1024) goto do_cleanup_5;
            padret = get_CD_State();
            if (!(padret & 4)) {
                if (serifu_state == 0) {
                    sound_seek_wait = 1;
                    serifu_skip();
                }
            }
            if (sound_seek_wait == 1) {
                return 0;
            }
            break;
        do_cleanup_5:
                sound_seek_time = 0;
                cmd_que_clear();
                device_cmd_send(116);
                return 100;
        } while(0);

    case18_set_and_text:
        serifu_voice = 1;
        sound_seek_time = 0;
    case18_text:
        set_kanji_string(-86, ycoord, color, &serifu_cmd_exec_str36, 0);
        msg_str_disp(-72, last_x_y, color, savedptr, 0, &serifu_cmd_exec_tbl43, &serifu_cmd_exec_tbl44, 0);
        serifu_wait = 1;
        return 0;

    case 11:
        slen = strlen(savedptr);
        t0_base = (s32 *)((u8 *)a3_param + (idx1 << 2));
        t1_off = idx2 << 2;
        entry = (s32 *)(*((s32 *)((u8 *)(*t0_base) + t1_off)));
        if (entry[0] != 0) {
            goto case11_ee79c;
        }
        if (entry[1] == 0) {
            goto case11_text;
        }
    case11_ee79c:
        if (voice_level == 0) {
            goto case11_nodisp;
        }
        if (sound_seek_time++ == 0) {

            if (serifu_state == 0) {
                entry = (s32 *)(*((s32 *)((u8 *)(*t0_base) + t1_off)));
                set_XAsector(entry[0], entry[1], 0);
                device_cmd_send(768);
            }
        }
        do {
            if (sound_seek_time >= 1024) goto do_cleanup_6;
            padret = get_CD_State();
            if (!(padret & 4)) {
                if (serifu_state == 0) {
                    sound_seek_wait = 1;
                    serifu_skip();
                }
            }
            if (sound_seek_wait == 1) {
                return 0;
            }
            break;
        do_cleanup_6:
                sound_seek_time = 0;
                cmd_que_clear();
                device_cmd_send(116);
                return 100;
        } while(0);

    case11_set_and_text:
        serifu_voice = 1;
        sound_seek_time = 0;
    case11_text:
        if (savedptr[slen - 1] == 110 && savedptr[slen - 2] == 110) {
            if (serifu_no != 0) {
                serifu_voice = 1;
                sound_seek_time = 0;
                return 0;
            }
            savedptr[slen - 2] = 0;
            slen = strlen(&serifu_cmd_exec_str37);
            set_kanji_string(-104 - slen * 4, ycoord, color, &serifu_cmd_exec_str38, 0);
            set_kanji_string(-86, ycoord, color, &serifu_cmd_exec_str39, 0);
            msg_str_disp(-72, ycoord, color, savedptr, 0, &serifu_cmd_exec_tbl45, &serifu_cmd_exec_tbl46, 0);
        } else {
            slen = strlen(&serifu_cmd_exec_str40);
            set_kanji_string(-104 - slen * 4, ycoord, color, &serifu_cmd_exec_str41, 0);
            set_kanji_string(-86, ycoord, color, &serifu_cmd_exec_str42, 0);
            msg_str_disp(-72, ycoord, color, savedptr, 0, &serifu_cmd_exec_tbl47, &serifu_cmd_exec_tbl48, 0);
        }
        goto set_flags;

    case11_nodisp:
        goto set_flags;

    case 10:
        slen = strlen(savedptr);
        t0_base = (s32 *)((u8 *)a3_param + (idx1 << 2));
        t1_off = idx2 << 2;
        entry = (s32 *)(*((s32 *)((u8 *)(*t0_base) + t1_off)));
        if (entry[0] != 0) {
            goto case10_ee79c;
        }
        if (entry[1] == 0) {
            goto case10_text;
        }
    case10_ee79c:
        if (voice_level == 0) {
            goto case10_text;
        }
        if (sound_seek_time++ == 0) {

            if (serifu_state == 0) {
                entry = (s32 *)(*((s32 *)((u8 *)(*t0_base) + t1_off)));
                set_XAsector(entry[0], entry[1], 13);
                device_cmd_send(768);
            }
        }
        do {
            if (sound_seek_time >= 1024) goto do_cleanup_7;
            padret = get_CD_State();
            if (!(padret & 4)) {
                if (serifu_state == 0) {
                    sound_seek_wait = 1;
                    serifu_skip();
                }
            }
            if (sound_seek_wait == 1) {
                return 0;
            }
            break;
        do_cleanup_7:
                sound_seek_time = 0;
                cmd_que_clear();
                device_cmd_send(116);
                return 100;
        } while(0);

    case10_set_and_text:
        serifu_voice = 1;
        sound_seek_time = 0;
    case10_text:
        if (savedptr[slen - 1] == 110 && savedptr[slen - 2] == 110) {
            if (serifu_no != 0) {
                return 0;
            }
            savedptr[slen - 2] = 0;
            slen = strlen(&serifu_cmd_exec_str43);
            set_kanji_string(-104 - slen * 4, ycoord, color, &serifu_cmd_exec_str44, 0);
            set_kanji_string(-86, ycoord, color, &serifu_cmd_exec_str45, 0);
            msg_str_disp(-72, ycoord, color, savedptr, 0, &serifu_cmd_exec_tbl49, &serifu_cmd_exec_tbl50, 0);
            goto set_flags;
        }
        slen = strlen(&serifu_cmd_exec_str46);
        set_kanji_string(-104 - slen * 4, ycoord, color, &serifu_cmd_exec_str47, 0);
        set_kanji_string(-86, ycoord, color, &serifu_cmd_exec_str48, 0);
        msg_str_disp(-72, ycoord, color, savedptr, 0, &serifu_cmd_exec_tbl51, &serifu_cmd_exec_tbl52, 0);
        goto set_flags;

    case 4:
    case 6:
        slen = strlen(&p_name);
        sprintf(fmtbuf, &serifu_cmd_exec_str49, &p_name);
        set_kanji_string(-104 - slen * 4, ycoord, color, fmtbuf, 0);
        set_kanji_string(-86, ycoord, color, &serifu_cmd_exec_str50, 0);
        msg_str_disp(-72, ycoord, color, savedptr, 0, &serifu_cmd_exec_str51, &serifu_cmd_exec_tbl53, 0);
        goto set_flags;

    case 3:
        if (*(u8 *)&a2_param != 0xFF) {
            msg_order_set();
            msg_part_reorder(savedptr);
        } else {
            junban = 0;
            menu3_select_tbl0 = 1;
            menu3_select_tbl1 = 2;
        }
        v0val = msg_str_disp(-114, ycoord, color, savedptr, 0, &serifu_cmd_exec_str52, &serifu_cmd_exec_str53, 1);
        k_disp_start(v0val);
        return 0;

    case 8:
        junban = 0;
        menu3_select_tbl0 = 1;
        v0val = msg_str_disp(-114, ycoord, color, savedptr, 0, &serifu_cmd_exec_str54, &serifu_cmd_exec_str55, 1);
        k_disp_start(v0val);
        return 0;

    case 19:
        junban = 0;
        menu3_select_tbl0 = 1;
        v0val = msg_str_disp(-114, ycoord, color, savedptr, 0, &serifu_cmd_exec_str52, &serifu_cmd_exec_str53, 1);
        k_disp_start(v0val);
        return 0;

    case 2:
    case 5:
        t0_base = (s32 *)((u8 *)a3_param + (idx1 << 2));
        t1_off = idx2 << 2;
        entry = (s32 *)(*(s32 *)((u8 *)(*t0_base) + t1_off) + (serifu_no << 3));
        if (entry[0] == 0) {
            goto case2_set_and_text;
        }
    case2_ee79c:
        if (voice_level == 0) {
            goto case2_set_and_text;
        }
        if (sound_seek_time++ == 0) {

            if (serifu_state == 0) {
                entry = (s32 *)(*(s32 *)((u8 *)(*(s32 *)((u8 *)a3_param + (idx1 << 2))) + (idx2 << 2)) + (serifu_no << 3));
                set_XAsector(entry[0], entry[1], ((u8 *)&tbl)[now_girl]);
                device_cmd_send(768);
            }
        }
        do {
            if (sound_seek_time >= 1024) goto do_cleanup_8;
            padret = get_CD_State();
            if (!(padret & 4)) {
                if (serifu_state == 0) {
                    sound_seek_wait = 1;
                    serifu_skip();
                }
            }
            if (sound_seek_wait == 1) {
                return 0;
            }
            break;
        do_cleanup_8:
                sound_seek_time = 0;
                cmd_que_clear();
                device_cmd_send(116);
                return 100;
        } while(0);

    case2_set_and_text:
        serifu_voice = 1;
        sound_seek_time = 0;
        slen = 0;
        v0val = 0;
        padret = 0;
        do {
            entry = (s32 *)(savedptr + padret);
            t1_off = *(u8 *)entry;
            if (t1_off == 0) {
                goto case2_end_scan;
            }
            if (t1_off == 113) {
                *(u8 *)entry = 0;
                v0val++;
                t0_base = (s32 *)(savedptr + (s16)slen);
                if ((u8)color < (s16)v0val) {
                    goto case2_nn_check;
                }
                slen = padret + 1;
                padret--;
            } else if (t1_off == 32 || t1_off == 109 || t1_off == 77 ||
                       t1_off == 65 || t1_off == 87 || t1_off == 79 ||
                       t1_off == 70 || t1_off == 89 || t1_off == 112 ||
                       t1_off == 71 || t1_off == 80 || t1_off == 83) {
                padret--;
            }
            padret += 2;
        } while (1);
    case2_nn_check:
        if (*(u8 *)t0_base == 110) {
            return 0;
        }
    case2_text:
        slen = strlen(&g_name);
        sprintf(fmtbuf, &serifu_cmd_exec_str58, &g_name);
        set_kanji_string(-104 - slen * 4, ycoord, color, fmtbuf, 0);
        set_kanji_string(-86, ycoord, color, &serifu_cmd_exec_str59, 0);
        msg_str_disp(-72, ycoord, color, savedptr, 0, &serifu_cmd_exec_tbl54, &serifu_cmd_exec_tbl55, 0);
        serifu_state = 1;
        return 0;
    case2_end_scan: ;

    }

default_other:
    game_reset();
    return 0;
}
