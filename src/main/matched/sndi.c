/* Address: 0x80051430 */
/* Size: 1052 bytes (263 instructions) */
/* Region: Game (0x80041000-0x80085FFF), IDO 5.2 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern u8 now_girl;
extern s8 k_mode;
extern char sndi_str0[];
extern char sndi_str1[];
extern char sndi_str2[];

extern void strncpy();
extern void strcpy();
extern void strcat();
extern void get_p_name();
extern void get_g_name();
extern void set_kanji_string();

void sndi(src, mode, kind)
u8 *src;
s32 mode;
s32 kind;
{
    u8 *p;
    u8 buf[64];
    u8 line[40];
    u8 tmp[40];
    u8 out[40];
    u8 num[20];
    u8 *s;
    s32 i;
    s32 found;

    strncpy(buf, src, 64);
    k_mode = 0;
    found = 0;
    if ((kind & 0xf) == 15) {
        for (i = 0, s = buf; s[i] != 0; i++) {
            p = buf + i;
            if (*p == 'P' && (i & 1) == 0) {
                *p = 0;
                found = 1;
                strcpy(tmp, buf);
                get_p_name(num, kind & 0xf);
                strcat(tmp, num);
                strcat(tmp, p + 1);
                break;
            }
        }
        i = 0;
        if (found == 0) {
            strcpy(tmp, buf);
        }
        found = 0;
        s = tmp;
        for (; s[i] != 0; i++) {
            p = tmp + i;
            if (*p == 'G' && (i & 1) == 0) {
                *p = 0;
                found = 1;
                strcpy(out, tmp);
                get_g_name(num, now_girl);
                strcat(out, num);
                strcat(out, p + 1);
                break;
            }
        }
        if (found == 0) {
            strcpy(out, tmp);
        }
    } else {
        for (i = 0, s = buf; s[i] != 0; i++) {
            p = buf + i;
            if (*p == 'P' && (i & 1) == 0) {
                *p = 0;
                found = 1;
                strcpy(out, buf);
                get_p_name(num, now_girl);
                strcat(out, num);
                strcat(out, p + 1);
                break;
            }
        }
        if (found == 0) {
            strcpy(out, buf);
        }
    }
    switch (mode) {
    case 0:
        if (kind == 15) {
            strcpy(line, sndi_str0);
            strcat(line, out);
        } else if (kind == 31) {
            strcpy(line, sndi_str1);
            strcat(line, out);
        } else if (kind == 47) {
            strcpy(line, out);
        } else {
            strcpy(line, sndi_str2);
            strcat(line, out);
        }
        if (kind == 255) {
            set_kanji_string(-128, 50, 0, line, 0);
        } else {
            set_kanji_string(-86, 50, 0, line, 0);
        }
        break;
    case 1:
        if (kind == 255) {
            set_kanji_string(-128, 66, 0, out, 0);
        } else {
            set_kanji_string(-72, 66, 0, out, 0);
        }
        break;
    case 2:
    default:
        if (kind == 255) {
            set_kanji_string(-128, 82, 0, out, 0);
        } else {
            set_kanji_string(-72, 82, 0, out, 0);
        }
        break;
    }
}
