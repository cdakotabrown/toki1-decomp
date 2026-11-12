/* Address: 0x8007F08C */
/* Size: 8868 bytes (2217 instructions) */
/* @CompileEnv: IDO52_AS1_TIE_TAKE=58 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: patch_stack_complete --old 664 --new 656 */
/* @PostProc: remap_stack_slots --keep-frame --map '96:88,112:104,124:116,126:118,128:120,130:122,132:124,133:125,135:127,137:129,139:131,644:636,654:646,668:660,672:664,676:668,680:672,684:676,688:680' */
/* @PostProc: swap_branch_operands --offsets 0x58 0x1e0c 0x20c8 0x20d0 0x20d8 0x20e0 0x20e8 0x20f0 0x20f8 0x2100 */
/* @PostProc: swap_instructions --offset1 0x48 --offset2 0x54 */
/* @PostProc: swap_instructions --offset1 0x6c --offset2 0x70 */
/* @PostProc: swap_instructions --offset1 0x74 --offset2 0x7c */
/* @PostProc: swap_instructions --offset1 0x78 --offset2 0x80 */
/* @PostProc: swap_instructions --offset1 0x108 --offset2 0x110 */
/* @PostProc: swap_instructions --offset1 0xb14 --offset2 0xb1c */
/* @PostProc: swap_instructions --offset1 0xe48 --offset2 0xe50 */
/* @PostProc: swap_instructions --offset1 0x1078 --offset2 0x1080 */
/* @PostProc: swap_instructions --offset1 0x1398 --offset2 0x13a0 */
/* @PostProc: swap_instructions --offset1 0x15dc --offset2 0x15e4 */
/* @PostProc: swap_instructions --offset1 0x17a4 --offset2 0x17ac */
/* @PostProc: swap_instructions --offset1 0x1a88 --offset2 0x1a90 */
/* @PostProc: swap_instructions --offset1 0x550 --offset2 0x554 */
/* @PostProc: swap_instructions --offset1 0x54c --offset2 0x550 */
/* @PostProc: swap_instructions --offset1 0x548 --offset2 0x54c */
/* @PostProc: swap_instructions --offset1 0x694 --offset2 0x698 */
/* @PostProc: swap_instructions --offset1 0x690 --offset2 0x694 */
/* @PostProc: swap_instructions --offset1 0x68c --offset2 0x690 */
/* @PostProc: patch_insn_reg_field 0x794:rt:t7:v0 0x79c:rs:t7:v0 0x1f78:rd:v0:t7 0x1f7c:rs:v0:t7 */
/* @CompileEnv: IDO52_UGEN_TEMP_REG=79:t7,338:t8,339:t7,348:t7,349:t8,381:t9,382:t8,391:t8,392:t9,437:t9,438:t6,439:t8,455:t7,456:t9,459:t8 */
#include "types.h"

extern void strcpy(u8 *dst, u8 *src);
extern s32  msg_str_disp(s32 code, s16 v, s32 a6, u8 *buf,
                           s32 x, u8 *msg_a, u8 *msg_b, s32 y);
extern s32  k_disp_start(s32 a0);
extern s32  strlen(u8 *s);
extern void sprintf(u8 *dst, u8 *fmt, u8 *arg);
extern void strncpy(u8 *dst, u8 *src, s32 n);
extern s32  strcmp(u8 *a, u8 *b);
extern void set_kanji_string(s32 x, s16 y, s32 a6, void *p, s32 z);
extern void msg_order_set(void);
extern void msg_part_reorder(u8 *buf);
extern void sub_sub_step_up(void);
extern void game_reset(void);
extern void set_XAsector(u32 a0, u32 a1, u8 a2);
extern void device_cmd_send(s32 a0);
extern s32  get_CD_State(void);
extern void serifu_skip(void);
extern void cmd_que_clear(void);

extern s32 serifu_voice;
extern s32 serifu_wait;
extern s32 serifu_line;

#ifdef PC_PORT
extern u8 junban[3];
#else
u8 junban[3] = { 0, 1, 2 };
#endif
extern u8  sound_seek_wait;
extern s16 last_x_y;
extern s32 serifu_state;
extern s32 serifu_no;
extern s32 msg_wait;
extern u8  p_name[];
extern u8  g_name[];
extern u8  xcut;
extern s16 sound_seek_time;
extern u8  voice_level;
extern u8  now_girl;
typedef struct { u32 w[4]; } Hdr16;
extern Hdr16 serifu_cmd_exec_tbl56;

extern u8 serifu_cmd_exec_tbl16[];
extern u8 serifu_cmd_exec_tbl17[];
extern u8 serifu_cmd_exec_tbl18[];
extern u8 serifu_cmd_exec_tbl19[];
extern u8 serifu_cmd_exec_str54[];
extern u8 serifu_cmd_exec_str55[];
extern u8 serifu_cmd_exec_str56[];
extern u8 serifu_cmd_exec_str57[];
extern u8 serifu_cmd_exec_str17[];
extern u8 serifu_cmd_exec_tbl20[];
extern u8 serifu_cmd_exec_tbl21[];
extern u8 serifu_cmd_exec_tbl22[];
extern u8 serifu_cmd_exec_tbl23[];
extern u8 serifu_cmd_exec_str22[];
extern u8 serifu_cmd_exec_str23[];
extern u8 serifu_cmd_exec_str24[];
extern u8 serifu_cmd_exec_tbl28[];
extern u8 serifu_cmd_exec_str49[];
extern u8 serifu_cmd_exec_str50[];
extern u8 serifu_cmd_exec_str51[];
extern u8 serifu_cmd_exec_tbl53[];
extern u8 serifu_cmd_exec_str52[];
extern u8 serifu_cmd_exec_str53[];
extern u8 serifu_cmd_exec_str18[];
extern u8 serifu_cmd_exec_str19[];
extern u8 serifu_cmd_exec_tbl24[];
extern u8 serifu_cmd_exec_tbl25[];
extern u8 serifu_cmd_exec_str20[];
extern u8 serifu_cmd_exec_str21[];
extern u8 serifu_cmd_exec_tbl26[];
extern u8 serifu_cmd_exec_tbl27[];
extern u8 serifu_cmd_exec_str29[];
extern u8 serifu_cmd_exec_str30[];
extern u8 serifu_cmd_exec_tbl33[];
extern u8 serifu_cmd_exec_tbl34[];
extern u8 serifu_cmd_exec_tbl39[];
extern u8 serifu_cmd_exec_tbl40[];
extern u8 serifu_cmd_exec_str35[];
extern u8 serifu_cmd_exec_tbl41[];
extern u8 serifu_cmd_exec_tbl42[];
extern u8 serifu_cmd_exec_str31[];
extern u8 serifu_cmd_exec_str32[];
extern u8 serifu_cmd_exec_tbl35[];
extern u8 serifu_cmd_exec_tbl36[];
extern u8 serifu_cmd_exec_str33[];
extern u8 serifu_cmd_exec_str34[];
extern u8 serifu_cmd_exec_tbl37[];
extern u8 serifu_cmd_exec_tbl38[];
extern u8 serifu_cmd_exec_str58[];
extern u8 serifu_cmd_exec_str59[];
extern u8 serifu_cmd_exec_tbl54[];
extern u8 serifu_cmd_exec_tbl55[];
extern u8 serifu_cmd_exec_str25[];
extern u8 serifu_cmd_exec_str26[];
extern u8 serifu_cmd_exec_tbl29[];
extern u8 serifu_cmd_exec_tbl30[];
extern u8 serifu_cmd_exec_str27[];
extern u8 serifu_cmd_exec_str28[];
extern u8 serifu_cmd_exec_tbl31[];
extern u8 serifu_cmd_exec_tbl32[];
extern u8 serifu_cmd_exec_str0[];
extern u8 serifu_cmd_exec_str1[];
extern u8 serifu_cmd_exec_tbl0[];
extern u8 serifu_cmd_exec_tbl1[];
extern u8 serifu_cmd_exec_str2[];
extern u8 serifu_cmd_exec_str3[];
extern u8 serifu_cmd_exec_tbl2[];
extern u8 serifu_cmd_exec_tbl3[];
extern u8 serifu_cmd_exec_str4[];
extern u8 serifu_cmd_exec_str5[];
extern u8 serifu_cmd_exec_tbl4[];
extern u8 serifu_cmd_exec_tbl5[];
extern u8 serifu_cmd_exec_str6[];
extern u8 serifu_cmd_exec_str7[];
extern u8 serifu_cmd_exec_str8[];
extern u8 serifu_cmd_exec_tbl6[];
extern u8 serifu_cmd_exec_str9[];
extern u8 serifu_cmd_exec_tbl7[];
extern u8 serifu_cmd_exec_tbl8[];
extern u8 serifu_cmd_exec_str10[];
extern u8 serifu_cmd_exec_str11[];
extern u8 serifu_cmd_exec_str12[];
extern u8 serifu_cmd_exec_str13[];
extern u8 serifu_cmd_exec_tbl9[];
extern u8 serifu_cmd_exec_str14[];
extern u8 serifu_cmd_exec_str15[];
extern u8 serifu_cmd_exec_tbl10[];
extern u8 serifu_cmd_exec_tbl11[];
extern u8 serifu_cmd_exec_str16[];
extern u8 serifu_cmd_exec_tbl12[];
extern u8 serifu_cmd_exec_tbl13[];
extern u8 serifu_cmd_exec_tbl14[];
extern u8 serifu_cmd_exec_tbl15[];
extern u8 serifu_cmd_exec_str36[];
extern u8 serifu_cmd_exec_str43[];
extern u8 serifu_cmd_exec_str44[];
extern u8 serifu_cmd_exec_str45[];
extern u8 serifu_cmd_exec_tbl49[];
extern u8 serifu_cmd_exec_tbl50[];
extern u8 serifu_cmd_exec_str46[];
extern u8 serifu_cmd_exec_str47[];
extern u8 serifu_cmd_exec_str48[];
extern u8 serifu_cmd_exec_tbl51[];
extern u8 serifu_cmd_exec_tbl52[];
extern u8 serifu_cmd_exec_str37[];
extern u8 serifu_cmd_exec_str38[];
extern u8 serifu_cmd_exec_str39[];
extern u8 serifu_cmd_exec_tbl45[];
extern u8 serifu_cmd_exec_tbl46[];
extern u8 serifu_cmd_exec_str40[];
extern u8 serifu_cmd_exec_str41[];
extern u8 serifu_cmd_exec_str42[];
extern u8 serifu_cmd_exec_tbl47[];
extern u8 serifu_cmd_exec_tbl48[];
extern u8 serifu_cmd_exec_tbl43[];
extern u8 serifu_cmd_exec_tbl44[];

s32 serifu_cmd_exec(u8 *a0, s32 sel, s32 a2,
                  void *a3, u16 a4, u16 a5, volatile u8 a6)
{
#define A3_HOME a3
    s32 unused0;
    s32 unused1;
    s16 n;
    s32 unused2;
    u8 *seg;
    u8  buf[512];
    u8  tmp[8];
    u8  line[12];
    Hdr16 hdr;
    s16 v;
    u32 *e;
    u32 *lastEntry;
    s32 k;
    s16 i;
    s16 cnt;
    u32 c;
    u8 *cur;

    hdr = serifu_cmd_exec_tbl56;

    if (*(u8 *)&sel != 255U) {
        strcpy(buf, a0);
    }

    serifu_voice = 0;
    sound_seek_wait = 0;
    v = (s16)(serifu_line * 16 + 48);

    switch ((u32)*(u8 *)&sel) {
    case 7:
        e = ((u32 ***)A3_HOME)[a4][a5];
        if ((e[0] != 0 || e[1] != 0) && voice_level != 0) {
            if (sound_seek_time++ == 0 && serifu_state == 0) {
                e = ((u32 ***)A3_HOME)[a4][a5];
                set_XAsector(e[0], e[1], ((u8 *)&hdr)[now_girl]);
                device_cmd_send(768);
            }
            if (sound_seek_time < 1024) {
                if ((get_CD_State() & 4) == 0 && serifu_state == 0) {
                    sound_seek_wait = 1;
                    serifu_skip();
                    if (sound_seek_wait == 1) {
                        return 0;
                    }
                }
            } else {
                sound_seek_time = 0;
                cmd_que_clear();
                device_cmd_send(116);
                return 100;
            }
            serifu_voice = 1;
            sound_seek_time = 0;
        }

        if (buf[0] == 62) {
            strncpy(tmp, &buf[1], 4);
            tmp[4] = 0;
            sprintf(line, serifu_cmd_exec_str0, tmp);
            set_kanji_string(-120, v, a6, line, 0);
            set_kanji_string(-86, v, a6, serifu_cmd_exec_str1, 0);
            msg_str_disp(-72, v, a6, &buf[5], 0,
                           serifu_cmd_exec_tbl0, serifu_cmd_exec_tbl1, 0);
        } else if (buf[0] == 45) {
            strncpy(tmp, &buf[1], 6);
            tmp[6] = 0;
            sprintf(line, serifu_cmd_exec_str2, tmp);
            set_kanji_string(-128, v, a6, line, 0);
            set_kanji_string(-86, v, a6, serifu_cmd_exec_str3, 0);
            msg_str_disp(-72, v, a6, &buf[7], 0,
                           serifu_cmd_exec_tbl2, serifu_cmd_exec_tbl3, 0);
        } else if (buf[0] == 43) {
            strncpy(tmp, &buf[1], 2);
            tmp[2] = 0;
            sprintf(line, serifu_cmd_exec_str4, tmp);
            set_kanji_string(-112, v, a6, line, 0);
            set_kanji_string(-86, v, a6, serifu_cmd_exec_str5, 0);
            msg_str_disp(-72, v, a6, &buf[3], 0,
                           serifu_cmd_exec_tbl4, serifu_cmd_exec_tbl5, 0);
        } else if (buf[0] == 60) {
            strncpy(tmp, &buf[1], 4);
            tmp[4] = 0;
            sprintf(line, serifu_cmd_exec_str6, tmp);
            set_kanji_string(-120, v, a6, line, 0);
            set_kanji_string(-86, v, a6, serifu_cmd_exec_str7, 0);
            msg_str_disp(-72, v, a6, &buf[5], 0,
                           serifu_cmd_exec_str8, serifu_cmd_exec_tbl6, 0);
        } else if (strcmp(buf, serifu_cmd_exec_str9) == 0) {
            v = (s16)(v + 16);
            msg_str_disp(-40, v, a6, buf, 0,
                           serifu_cmd_exec_tbl7, serifu_cmd_exec_tbl8, 0);
        } else if (strcmp(buf, serifu_cmd_exec_str10) == 0 ||
                   strcmp(buf, serifu_cmd_exec_str11) == 0 ||
                   strcmp(buf, serifu_cmd_exec_str12) == 0 ||
                   strcmp(buf, serifu_cmd_exec_str13) == 0) {
            msg_str_disp(-128, v, 2, buf, 0,
                           serifu_cmd_exec_tbl9, serifu_cmd_exec_str14, 0);
        } else if (strcmp(buf, serifu_cmd_exec_str15) == 0) {
            msg_str_disp(-128, v, 1, buf, 0,
                           serifu_cmd_exec_tbl10, serifu_cmd_exec_tbl11, 0);
        } else if (strcmp(buf, serifu_cmd_exec_str16) == 0) {
            v = (s16)(v + 16);
            msg_str_disp(-40, v, 2, buf, 0,
                           serifu_cmd_exec_tbl12, serifu_cmd_exec_tbl13, 0);
        } else {
            msg_str_disp(-128, v, a6, buf, 0,
                           serifu_cmd_exec_tbl14, serifu_cmd_exec_tbl15, 0);
        }
        serifu_state = 1;
        break;

    case 16:
        msg_str_disp(-128, v, a6, buf, 0, serifu_cmd_exec_tbl16, serifu_cmd_exec_tbl17, 0);
        serifu_wait = 1;
        break;

    case 17:
        msg_str_disp(-72, last_x_y, a6, buf, 0, serifu_cmd_exec_tbl18, serifu_cmd_exec_tbl19, 0);
        serifu_wait = 1;
        break;

    case 9:
        if (buf[0] == 0) {
            break;
        }
        if (strcmp(buf, serifu_cmd_exec_str17) == 0) {
            msg_str_disp(-128, v, 1, buf, 0, serifu_cmd_exec_tbl20, serifu_cmd_exec_tbl21, 0);
        }
        msg_str_disp(-128, v, a6, buf, 0, serifu_cmd_exec_tbl22, serifu_cmd_exec_tbl23, 0);
        return 0;

    case 0:
        k = strlen(buf);
        n = (s16)k;
        if (buf[n - 1] == 110 && buf[k - 2] == 110) {
            if (serifu_no != 0) {
                goto ret0;
            }
            buf[n - 2] = 0;
            n = (s16)strlen(p_name);
            sprintf(line, serifu_cmd_exec_str18, p_name);
            set_kanji_string((s16)(-104 - n * 4), v, a6, line, 0);
            set_kanji_string(-86, v, a6, serifu_cmd_exec_str19, 0);
            msg_str_disp(-72, v, a6, buf, 0,
                           serifu_cmd_exec_tbl24, serifu_cmd_exec_tbl25, 0);
            serifu_state = 1;
            goto ret0;
        }
        n = (s16)strlen(p_name);
        sprintf(line, serifu_cmd_exec_str20, p_name);
        set_kanji_string((s16)(-104 - n * 4), v, a6, line, 0);
        set_kanji_string(-86, v, a6, serifu_cmd_exec_str21, 0);
        msg_str_disp(-72, v, a6, buf, 0,
                       serifu_cmd_exec_tbl26, serifu_cmd_exec_tbl27, 0);
        serifu_state = 1;
        goto ret0;

    case 14:
        n = (s16)strlen(p_name);
        sprintf(line, serifu_cmd_exec_str22, p_name);
        set_kanji_string((s16)(-104 - n * 4), v, a6, line, 0);
        set_kanji_string(-86, v, a6, serifu_cmd_exec_str23, 0);
        msg_str_disp(-72, v, a6, buf, 0, serifu_cmd_exec_str24, serifu_cmd_exec_tbl28, 0);
        return 0;

    case 1:
        k = strlen(buf);
        n = (s16)k;
        e = ((u32 ***)A3_HOME)[a4][a5];
        if ((e[0] != 0 || e[1] != 0) && voice_level != 0 &&
            !(buf[k - 1] == 110 && buf[k - 2] == 110 && serifu_no != 0)) {
            if (sound_seek_time++ == 0 && serifu_state == 0) {
                e = ((u32 ***)A3_HOME)[a4][a5];
                set_XAsector(e[0], e[1], ((u8 *)&hdr)[now_girl]);
                device_cmd_send(768);
            }
            if (sound_seek_time < 1024) {
                if ((get_CD_State() & 4) == 0 && serifu_state == 0) {
                    sound_seek_wait = 1;
                    serifu_skip();
                    if (sound_seek_wait == 1) {
                        return 0;
                    }
                }
            } else {
                sound_seek_time = 0;
                cmd_que_clear();
                device_cmd_send(116);
                return 100;
            }
            serifu_voice = 1;
            sound_seek_time = 0;
        }
        if (buf[n - 1] == 110 && buf[n - 2] == 110) {
            if (serifu_no != 0) {
                goto ret0;
            }
            buf[n - 2] = 0;
            n = (s16)strlen(g_name);
            sprintf(line, serifu_cmd_exec_str25, g_name);
            set_kanji_string((s16)(-104 - n * 4), v, a6, line, 0);
            set_kanji_string(-86, v, a6, serifu_cmd_exec_str26, 0);
            msg_str_disp(-72, v, a6, buf, 0,
                           serifu_cmd_exec_tbl29, serifu_cmd_exec_tbl30, 0);
            serifu_state = 1;
            goto ret0;
        }
        n = (s16)strlen(g_name);
        sprintf(line, serifu_cmd_exec_str27, g_name);
        set_kanji_string((s16)(-104 - n * 4), v, a6, line, 0);
        set_kanji_string(-86, v, a6, serifu_cmd_exec_str28, 0);
        msg_str_disp(-72, v, a6, buf, 0,
                       serifu_cmd_exec_tbl31, serifu_cmd_exec_tbl32, 0);
        serifu_state = 1;
        goto ret0;

    case 15:
        e = ((u32 ***)A3_HOME)[a4][a5];
        if ((e[0] != 0 || e[1] != 0) && voice_level != 0) {
            if (sound_seek_time++ == 0 && serifu_state == 0) {
                e = ((u32 ***)A3_HOME)[a4][a5];
                set_XAsector(e[0], e[1], ((u8 *)&hdr)[now_girl]);
                device_cmd_send(768);
            }
            if (sound_seek_time < 1024) {
                if ((get_CD_State() & 4) == 0 && serifu_state == 0) {
                    sound_seek_wait = 1;
                    serifu_skip();
                    if (sound_seek_wait == 1) {
                        return 0;
                    }
                }
            } else {
                sound_seek_time = 0;
                cmd_que_clear();
                device_cmd_send(116);
                return 100;
            }
            serifu_voice = 1;
            sound_seek_time = 0;
        }
        n = (s16)strlen(g_name);
        sprintf(line, serifu_cmd_exec_str29, g_name);
        set_kanji_string((s16)(-104 - n * 4), v, a6, line, 0);
        set_kanji_string(-86, v, a6, serifu_cmd_exec_str30, 0);
        msg_str_disp(-72, v, a6, buf, 0,
                       serifu_cmd_exec_tbl33, serifu_cmd_exec_tbl34, 0);
        msg_wait = 1;
        goto ret0;

    case 12:
        xcut = 0;
        n = (s16)strlen(buf);
        e = ((u32 ***)A3_HOME)[a4][a5];
        if ((e[0] != 0 || e[1] != 0) && voice_level != 0) {
            if (sound_seek_time++ == 0 && serifu_state == 0) {
                e = ((u32 ***)A3_HOME)[a4][a5];
                set_XAsector(e[0], e[1], ((u8 *)&hdr)[now_girl]);
                device_cmd_send(768);
            }
            if (sound_seek_time < 1024) {
                if ((get_CD_State() & 4) == 0 && serifu_state == 0) {
                    sound_seek_wait = 1;
                    serifu_skip();
                    if (sound_seek_wait == 1) {
                        return 0;
                    }
                }
            } else {
                sound_seek_time = 0;
                cmd_que_clear();
                device_cmd_send(116);
                return 100;
            }
            serifu_voice = 1;
            sound_seek_time = 0;
        }
        if (buf[n - 1] == 110 && buf[n - 2] == 110) {
            if (serifu_no != 0) {
                goto ret0;
            }
            buf[n - 2] = 0;
            n = (s16)strlen(g_name);
            sprintf(line, serifu_cmd_exec_str31, g_name);
            set_kanji_string((s16)(-104 - n * 4), v, a6, line, 0);
            set_kanji_string(-86, v, a6, serifu_cmd_exec_str32, 0);
            msg_str_disp(-72, v, a6, buf, 0,
                           serifu_cmd_exec_tbl35, serifu_cmd_exec_tbl36, 0);
            serifu_wait = 1;
            goto ret0;
        }
        n = (s16)strlen(g_name);
        sprintf(line, serifu_cmd_exec_str33, g_name);
        set_kanji_string((s16)(-104 - n * 4), v, a6, line, 0);
        set_kanji_string(-86, v, a6, serifu_cmd_exec_str34, 0);
        msg_str_disp(-72, v, a6, buf, 0,
                       serifu_cmd_exec_tbl37, serifu_cmd_exec_tbl38, 0);
        serifu_wait = 1;
        goto ret0;

    case 13:
        n = (s16)strlen(buf);
        e = ((u32 ***)A3_HOME)[a4][a5];
        if ((e[0] != 0 || e[1] != 0) && voice_level != 0 && xcut == 0) {
            if (sound_seek_time++ == 0 && serifu_state == 0) {
                e = ((u32 ***)A3_HOME)[a4][a5];
                set_XAsector(e[0], e[1], ((u8 *)&hdr)[now_girl]);
                device_cmd_send(768);
            }
            if (sound_seek_time < 1024) {
                if ((get_CD_State() & 4) == 0 && serifu_state == 0) {
                    sound_seek_wait = 1;
                    serifu_skip();
                    if (sound_seek_wait == 1) {
                        return 0;
                    }
                }
            } else {
                sound_seek_time = 0;
                cmd_que_clear();
                device_cmd_send(116);
                return 100;
            }
            serifu_voice = 1;
            sound_seek_time = 0;
        }
        if (buf[n - 1] == 110 && buf[n - 2] == 110) {
            if (serifu_no != 0) {
                goto ret0;
            }
            buf[n - 2] = 0;
            msg_str_disp(-72, last_x_y, a6, buf, 0,
                           serifu_cmd_exec_tbl39, serifu_cmd_exec_tbl40, 0);
            serifu_state = 1;
            goto ret0;
        }
        set_kanji_string(-86, v, a6, serifu_cmd_exec_str35, 0);
        msg_str_disp(-72, last_x_y, a6, buf, 0,
                       serifu_cmd_exec_tbl41, serifu_cmd_exec_tbl42, 0);
        serifu_state = 1;
        goto ret0;

    case 18:
        strlen(buf);
        e = ((u32 ***)A3_HOME)[a4][a5];
        if ((e[0] != 0 || e[1] != 0) && voice_level != 0 && xcut == 0) {
            if (sound_seek_time++ == 0 && serifu_state == 0) {
                e = ((u32 ***)A3_HOME)[a4][a5];
                set_XAsector(e[0], e[1], ((u8 *)&hdr)[now_girl]);
                device_cmd_send(768);
            }
            if (sound_seek_time < 1024) {
                if ((get_CD_State() & 4) == 0 && serifu_state == 0) {
                    sound_seek_wait = 1;
                    serifu_skip();
                    if (sound_seek_wait == 1) {
                        return 0;
                    }
                }
            } else {
                sound_seek_time = 0;
                cmd_que_clear();
                device_cmd_send(116);
                return 100;
            }
            serifu_voice = 1;
            sound_seek_time = 0;
        }
        set_kanji_string(-86, v, a6, serifu_cmd_exec_str36, 0);
        msg_str_disp(-72, last_x_y, a6, buf, 0,
                       serifu_cmd_exec_tbl43, serifu_cmd_exec_tbl44, 0);
        serifu_wait = 1;
        break;

    case 11:
        n = (s16)strlen(buf);
        e = ((u32 ***)A3_HOME)[a4][a5];
        if ((e[0] != 0 || e[1] != 0) && voice_level != 0) {
            if (sound_seek_time++ == 0 && serifu_state == 0) {
                e = ((u32 ***)A3_HOME)[a4][a5];
                set_XAsector(e[0], e[1], 0);
                device_cmd_send(768);
            }
            if (sound_seek_time < 1024) {
                if ((get_CD_State() & 4) == 0 && serifu_state == 0) {
                    sound_seek_wait = 1;
                    serifu_skip();
                    if (sound_seek_wait == 1) {
                        return 0;
                    }
                }
            } else {
                sound_seek_time = 0;
                cmd_que_clear();
                device_cmd_send(116);
                return 100;
            }
            serifu_voice = 1;
            sound_seek_time = 0;
        }
        if (buf[n - 1] == 110 && buf[n - 2] == 110) {
            if (serifu_no != 0) {
                goto ret0;
            }
            buf[n - 2] = 0;
            k = strlen(serifu_cmd_exec_str37);
            set_kanji_string((s16)(-104 - k * 4), v, a6, serifu_cmd_exec_str38, 0);
            set_kanji_string(-86, v, a6, serifu_cmd_exec_str39, 0);
            msg_str_disp(-72, v, a6, buf, 0,
                           serifu_cmd_exec_tbl45, serifu_cmd_exec_tbl46, 0);
            serifu_state = 1;
            goto ret0;
        }
        k = strlen(serifu_cmd_exec_str40);
        set_kanji_string((s16)(-104 - k * 4), v, a6, serifu_cmd_exec_str41, 0);
        set_kanji_string(-86, v, a6, serifu_cmd_exec_str42, 0);
        msg_str_disp(-72, v, a6, buf, 0,
                       serifu_cmd_exec_tbl47, serifu_cmd_exec_tbl48, 0);
        serifu_state = 1;
        goto ret0;

    case 10:
        n = (s16)strlen(buf);
        e = ((u32 ***)A3_HOME)[a4][a5];
        if ((e[0] != 0 || e[1] != 0) && voice_level != 0) {
            if (sound_seek_time++ == 0 && serifu_state == 0) {
                e = ((u32 ***)A3_HOME)[a4][a5];
                set_XAsector(e[0], e[1], 13);
                device_cmd_send(768);
            }
            if (sound_seek_time < 1024) {
                if ((get_CD_State() & 4) == 0 && serifu_state == 0) {
                    sound_seek_wait = 1;
                    serifu_skip();
                    if (sound_seek_wait == 1) {
                        return 0;
                    }
                }
            } else {
                sound_seek_time = 0;
                cmd_que_clear();
                device_cmd_send(116);
                return 100;
            }
            serifu_voice = 1;
            sound_seek_time = 0;
        }
        if (buf[n - 1] == 110 && buf[n - 2] == 110) {
            if (serifu_no != 0) {
                goto ret0;
            }
            buf[n - 2] = 0;
            k = strlen(serifu_cmd_exec_str43);
            set_kanji_string((s16)(-104 - k * 4), v, a6, serifu_cmd_exec_str44, 0);
            set_kanji_string(-86, v, a6, serifu_cmd_exec_str45, 0);
            msg_str_disp(-72, v, a6, buf, 0,
                           serifu_cmd_exec_tbl49, serifu_cmd_exec_tbl50, 0);
            serifu_state = 1;
            goto ret0;
        }
        k = strlen(serifu_cmd_exec_str46);
        set_kanji_string((s16)(-104 - k * 4), v, a6, serifu_cmd_exec_str47, 0);
        set_kanji_string(-86, v, a6, serifu_cmd_exec_str48, 0);
        msg_str_disp(-72, v, a6, buf, 0,
                       serifu_cmd_exec_tbl51, serifu_cmd_exec_tbl52, 0);
        serifu_state = 1;
        goto ret0;

    case 4:
    case 6:
        n = (s16)strlen(p_name);
        sprintf(line, serifu_cmd_exec_str49, p_name);
        set_kanji_string((s16)(-104 - n * 4), v, a6, line, 0);
        set_kanji_string(-86, v, a6, serifu_cmd_exec_str50, 0);
        msg_str_disp(-72, v, a6, buf, 0, serifu_cmd_exec_str51, serifu_cmd_exec_tbl53, 0);
        serifu_state = 1;
        break;

    case 3:
        if (*(u8 *)&a2 != 255U) {
            msg_order_set();
            msg_part_reorder(buf);
        } else {
            junban[0] = 0;
            junban[1] = 1;
            junban[2] = 2;
        }
        k_disp_start(msg_str_disp(-114, v, a6, buf, 0,
                                     serifu_cmd_exec_str52, serifu_cmd_exec_str53, 1));
        return 0;

    case 8:
        junban[0] = 0;
        junban[1] = 1;
        k_disp_start(msg_str_disp(-114, v, a6, buf, 0,
                                     serifu_cmd_exec_str54, serifu_cmd_exec_str55, 1));
        return 0;

    case 19:
        junban[0] = 0;
        junban[1] = 1;
        k_disp_start(msg_str_disp(-114, v, a6, buf, 0,
                                     serifu_cmd_exec_str56, serifu_cmd_exec_str57, 1));
        return 0;

    case 2:
    case 5:
        lastEntry = ((u32 ***)A3_HOME)[a4][a5] + serifu_no * 2;
        if (lastEntry[0] != 0 && voice_level != 0) {
            if (sound_seek_time++ == 0 && serifu_state == 0) {
                lastEntry = ((u32 ***)A3_HOME)[a4][a5] + serifu_no * 2;
                set_XAsector(lastEntry[0], lastEntry[1], ((u8 *)&hdr)[now_girl]);
                device_cmd_send(768);
            }
            if (sound_seek_time < 1024) {
                if ((get_CD_State() & 4) == 0 && serifu_state == 0) {
                    sound_seek_wait = 1;
                    serifu_skip();
                    if (sound_seek_wait == 1) {
                        goto ret0;
                    }
                }
            } else {
                sound_seek_time = 0;
                cmd_que_clear();
                device_cmd_send(116);
                return 100;
            }
            serifu_voice = 1;
            sound_seek_time = 0;
        }

        n = 0;
        cnt = 0;
        i = 0;
        for (;;) {
            cur = &buf[i];
            c = *cur;
            if (c == 32 || c == 109 || c == 77 || c == 65 || c == 87 ||
                c == 79 || c == 70 || c == 89 || c == 112 || c == 71 ||
                c == 80 || c == 83) {
                i = (s16)(i - 1);
            } else if (c == 113 || c == 0) {
                *cur = 0;
                cnt = (s16)(cnt + 1);
                seg = &buf[n];
                if ((u32)*(u8 *)&a2 < (u32)cnt) {
                    break;
                }
                n = (s16)(i + 1);
                i = (s16)(i - 1);
            }
            i = (s16)(i + 2);
        }
        if (seg[0] == 110) {
            break;
        }
        n = (s16)strlen(g_name);
        sprintf(line, serifu_cmd_exec_str58, g_name);
        set_kanji_string((s16)(-104 - n * 4), v, a6, line, 0);
        set_kanji_string(-86, v, a6, serifu_cmd_exec_str59, 0);
        msg_str_disp(-72, v, a6, seg, 0,
                       serifu_cmd_exec_tbl54, serifu_cmd_exec_tbl55, 0);
        serifu_state = 1;
        break;

    case 255:
        sub_sub_step_up();
        return 0;

    default:
        game_reset();
        break;
    }

ret0:
    return 0;
}
