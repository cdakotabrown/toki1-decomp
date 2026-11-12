/* Address: 0x80081398 */
/* Size: 5588 bytes (1397 instructions) */
/* Region: Game (IDO 5.2) */
/* @CompileFlags: -Wc,-pic1 -cckr */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

typedef struct { u8 d[3]; } S3;

extern void strcpy();
extern s32 strlen();
extern void strcat();
extern void sprintf();
extern s32 set_kanji_string(s16, s16, u8, u8 *, s32);

extern S3 msg_str_disp_str0; extern S3 msg_str_disp_str1; extern S3 msg_str_disp_str2;
extern S3 msg_str_disp_str3; extern S3 msg_str_disp_str4;
extern u8 msg_str_disp_str5[]; extern u8 msg_str_disp_str6[]; extern u8 msg_str_disp_str7[];
extern u8 msg_str_disp_str8[]; extern u8 msg_str_disp_str9[];
extern u8 p_name[]; extern u8 pp_name[]; extern u8 p_name2[];
extern u8 g_name2[]; extern u8 shu_name[]; extern u8 oname[];
extern u8 wname[]; extern u8 aname[]; extern u8 mname[];
extern s16 last_x_y;
extern s32 mugon_flag;

s32 msg_str_disp(x, y, color, str, arg4, suffix, bullet, show)
    s16 x;
    s16 y;
    u8 color;
    u8 *str;
    s32 arg4;
    u8 *suffix;
    u8 *bullet;
    u8 show;
{
    s32 ret;
    s16 pos;
    s16 len;
    s16 start;
    s16 n;
    s16 k;
    s16 j;
    s16 dy;
    u8 c;
    u8 work[64];
    u8 tmpBuf[24];
    u8 buf[512];
    u8 prefix[8];
    S3 pair0, pair1, pair2, pair3, pair4;

    pair0 = msg_str_disp_str0;
    pair1 = msg_str_disp_str1;
    pair2 = msg_str_disp_str2;
    pair3 = msg_str_disp_str3;
    pair4 = msg_str_disp_str4;

    if (*str == 0) {
        return 0;
    }
    strcpy(prefix, msg_str_disp_str5);
    strcpy(buf, str);
    mugon_flag = 1;
    start = strlen(buf);
    for (pos = 0; pos < start; pos += 2) {
        if (buf[pos] == 'm' || buf[pos] == 'M' || buf[pos] == 'A' ||
            buf[pos] == 'W' || buf[pos] == 'O' || buf[pos] == 'F' ||
            buf[pos] == 'Y' || buf[pos] == 'S' || buf[pos] == 'P' ||
            buf[pos] == 'p' || buf[pos] == 'G' || buf[pos] == ' ' ||
            buf[pos] == 'q') {
            pos--;
        } else if (buf[pos] == pair0.d[0] && buf[pos + 1] == pair0.d[1]) {
        } else if (buf[pos] == pair1.d[0] && buf[pos + 1] == pair1.d[1]) {
        } else if (buf[pos] == pair2.d[0] && buf[pos + 1] == pair2.d[1]) {
        } else if (buf[pos] == pair3.d[0] && buf[pos + 1] == pair3.d[1]) {
        } else if (buf[pos] == pair4.d[0] && buf[pos + 1] == pair4.d[1]) {
        } else {
            mugon_flag = 0;
        }
    }

    start = 0;
    dy = 0;
    for (pos = 0, n = 0, k = 0; ; pos += 2) {
        c = buf[pos];
        if (c == 'm' || c == 'M' || c == 'A' || c == 'W' || c == 'O' ||
            c == 'F' || c == 'Y' || c == 'S' || c == 'P' || c == 'p' ||
            c == 'G') {
            pos--;
        } else if (c == ' ' || c == 'q') {
            buf[pos] = 0;
            if (show != 0) {
                set_kanji_string(x - 14, y + dy, color, bullet, arg4);
            }
            strncpy(work, &buf[start], 64);
            len = pos - start;
            if (len > 0) {
                do {
                    c = work[n];
                    if (c == 'P') {
                        for (j = 0; j < strlen(p_name2); j++) {
                            work[n + j] = p_name2[j];
                        }
                        strncpy(&work[n + j], &buf[start] + 1 + k,
                            64 - n - j);
                        len += strlen(p_name2) - 1;
                        n += strlen(p_name2);
                        n -= 2; k--;
                    } else if (c == 'S') {
                        for (j = 0; j < strlen(shu_name); j++) {
                            work[n + j] = shu_name[j];
                        }
                        strncpy(&work[n + j], &buf[start] + 1 + k,
                            64 - n - j);
                        len += strlen(shu_name) - 1;
                        n += strlen(shu_name);
                        n -= 2; k--;
                    } else if (c == 'G') {
                        for (j = 0; j < strlen(g_name2); j++) {
                            work[n + j] = g_name2[j];
                        }
                        strncpy(&work[n + j], &buf[start] + 1 + k,
                            64 - n - j);
                        len += strlen(g_name2) - 1;
                        n += strlen(g_name2);
                        n -= 2; k--;
                    } else if (c == 'p') {
                        for (j = 0; j < strlen(p_name); j++) {
                            work[n + j] = p_name[j];
                        }
                        strncpy(&work[n + j], &buf[start] + 1 + k,
                            64 - n - j);
                        len += strlen(p_name) - 1;
                        n += strlen(p_name);
                        n -= 2; k--;
                    } else if (c == 'Y') {
                        strcpy(tmpBuf, msg_str_disp_str6);
                        for (j = 0; j < strlen(tmpBuf); j++) {
                            work[n + j] = tmpBuf[j];
                        }
                        strncpy(&work[n + j], &buf[start] + 1 + k,
                            64 - n - j);
                        len += strlen(tmpBuf) - 1;
                        n += strlen(tmpBuf);
                        n -= 2; k--;
                    } else if (c == 'F') {
                        sprintf(tmpBuf, msg_str_disp_str7, p_name, pp_name);
                        for (j = 0; j < strlen(tmpBuf); j++) {
                            work[n + j] = tmpBuf[j];
                        }
                        strncpy(&work[n + j], &buf[start] + 1 + k,
                            64 - n - j);
                        len += strlen(tmpBuf) - 1;
                        n += strlen(tmpBuf);
                        n -= 2; k--;
                    } else if (c == 'm') {
                        strcpy(tmpBuf, pp_name);
                        for (j = 0; j < strlen(tmpBuf); j++) {
                            work[n + j] = tmpBuf[j];
                        }
                        strncpy(&work[n + j], &buf[start] + 1 + k,
                            64 - n - j);
                        len += strlen(tmpBuf) - 1;
                        n += strlen(tmpBuf);
                        n -= 2; k--;
                    } else if (c == 'O') {
                        for (j = 0; j < strlen(oname); j++) {
                            work[n + j] = oname[j];
                        }
                        strncpy(&work[n + j], &buf[start] + 1 + k,
                            64 - n - j);
                        len += strlen(oname) - 1;
                        n += strlen(oname);
                        n -= 2; k--;
                    } else if (c == 'W') {
                        for (j = 0; j < strlen(wname); j++) {
                            work[n + j] = wname[j];
                        }
                        strncpy(&work[n + j], &buf[start] + 1 + k,
                            64 - n - j);
                        len += strlen(wname) - 1;
                        n += strlen(wname);
                        n -= 2; k--;
                    } else if (c == 'A') {
                        for (j = 0; j < strlen(aname); j++) {
                            work[n + j] = aname[j];
                        }
                        strncpy(&work[n + j], &buf[start] + 1 + k,
                            64 - n - j);
                        len += strlen(aname) - 1;
                        n += strlen(aname);
                        n -= 2; k--;
                    } else if (c == 'M') {
                        for (j = 0; j < strlen(mname); j++) {
                            work[n + j] = mname[j];
                        }
                        strncpy(&work[n + j], &buf[start] + 1 + k,
                            64 - n - j);
                        len += strlen(mname) - 1;
                        n += strlen(mname);
                        n -= 2; k--;
                    }
                    n += 2;
                    k += 2;
                } while (n < len);
                n = 0;
                k = 0;
            }
            if (show != 0) {
                strcat(work, suffix);
            }
            if (work[0] == prefix[0] && work[1] == prefix[1]) {
                set_kanji_string(x - 14, y + dy, color, work, arg4);
            } else {
                set_kanji_string(x, y + dy, color, work, arg4);
            }
            dy += 16;
            start = pos + 1;
            pos--;
        } else if (c == 0) {
            if (show != 0) {
                set_kanji_string(x - 14, y + dy, color, bullet, arg4);
            }
            strncpy(work, &buf[start], 64);
            len = pos - start;
            if (len > 0) {
                do {
                    c = work[n];
                    if (c == 'P') {
                        for (j = 0; j < strlen(p_name2); j++) {
                            work[n + j] = p_name2[j];
                        }
                        strncpy(&work[n + j], &buf[start] + 1 + k,
                            64 - n - j);
                        len += strlen(p_name2) - 1;
                        n += strlen(p_name2);
                        n -= 2; k--;
                    } else if (c == 'S') {
                        for (j = 0; j < strlen(shu_name); j++) {
                            work[n + j] = shu_name[j];
                        }
                        strncpy(&work[n + j], &buf[start] + 1 + k,
                            64 - n - j);
                        len += strlen(shu_name) - 1;
                        n += strlen(shu_name);
                        n -= 2; k--;
                    } else if (c == 'G') {
                        for (j = 0; j < strlen(g_name2); j++) {
                            work[n + j] = g_name2[j];
                        }
                        strncpy(&work[n + j], &buf[start] + 1 + k,
                            64 - n - j);
                        len += strlen(g_name2) - 1;
                        n += strlen(g_name2);
                        n -= 2; k--;
                    } else if (c == 'p') {
                        for (j = 0; j < strlen(p_name); j++) {
                            work[n + j] = p_name[j];
                        }
                        strncpy(&work[n + j], &buf[start] + 1 + k,
                            64 - n - j);
                        len += strlen(p_name) - 1;
                        n += strlen(p_name);
                        n -= 2; k--;
                    } else if (c == 'Y') {
                        strcpy(tmpBuf, msg_str_disp_str8);
                        for (j = 0; j < strlen(tmpBuf); j++) {
                            work[n + j] = tmpBuf[j];
                        }
                        strncpy(&work[n + j], &buf[start] + 1 + k,
                            64 - n - j);
                        len += strlen(tmpBuf) - 1;
                        n += strlen(tmpBuf);
                        n -= 2; k--;
                    } else if (c == 'F') {
                        sprintf(tmpBuf, msg_str_disp_str9, p_name, pp_name);
                        for (j = 0; j < strlen(tmpBuf); j++) {
                            work[n + j] = tmpBuf[j];
                        }
                        strncpy(&work[n + j], &buf[start] + 1 + k,
                            64 - n - j);
                        len += strlen(tmpBuf) - 1;
                        n += strlen(tmpBuf);
                        n -= 2; k--;
                    } else if (c == 'm') {
                        strcpy(tmpBuf, pp_name);
                        for (j = 0; j < strlen(tmpBuf); j++) {
                            work[n + j] = tmpBuf[j];
                        }
                        strncpy(&work[n + j], &buf[start] + 1 + k,
                            64 - n - j);
                        len += strlen(tmpBuf) - 1;
                        n += strlen(tmpBuf);
                        n -= 2; k--;
                    } else if (c == 'O') {
                        for (j = 0; j < strlen(oname); j++) {
                            work[n + j] = oname[j];
                        }
                        strncpy(&work[n + j], &buf[start] + 1 + k,
                            64 - n - j);
                        len += strlen(oname) - 1;
                        n += strlen(oname);
                        n -= 2; k--;
                    } else if (c == 'W') {
                        for (j = 0; j < strlen(wname); j++) {
                            work[n + j] = wname[j];
                        }
                        strncpy(&work[n + j], &buf[start] + 1 + k,
                            64 - n - j);
                        len += strlen(wname) - 1;
                        n += strlen(wname);
                        n -= 2; k--;
                    } else if (c == 'A') {
                        for (j = 0; j < strlen(aname); j++) {
                            work[n + j] = aname[j];
                        }
                        strncpy(&work[n + j], &buf[start] + 1 + k,
                            64 - n - j);
                        len += strlen(aname) - 1;
                        n += strlen(aname);
                        n -= 2; k--;
                    } else if (c == 'M') {
                        for (j = 0; j < strlen(mname); j++) {
                            work[n + j] = mname[j];
                        }
                        strncpy(&work[n + j], &buf[start] + 1 + k,
                            64 - n - j);
                        len += strlen(mname) - 1;
                        n += strlen(mname);
                        n -= 2; k--;
                    }
                    n += 2;
                    k += 2;
                } while (n < len);
            }
            if (*suffix != 0) {
                strcat(work, suffix);
            }
            last_x_y = y + dy + 16;
            if (work[0] == prefix[0] && work[1] == prefix[1]) {
                ret = set_kanji_string(x - 14, y + dy, color, work, arg4);
            } else {
                ret = set_kanji_string(x, y + dy, color, work, arg4);
            }
            return ret;
        }
    }
}
