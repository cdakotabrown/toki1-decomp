/* Function: get_p_name */
/* Address: 0x80051928 */
/* Size: 1376 bytes (344 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* used to carry a bare `@PostProc: scheduler_normalizer`, which shuffled */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern s32 get_g_zyotai_s(s32);
extern void strcpy(s32, char *);
extern void strcat(s32, char *);
extern void strncat(s32, char *, s32);

extern char p_adana[];
extern char p_mei[];
extern char p_sei[];
extern char get_p_name_str0[];
extern char get_p_name_str1[];

void get_p_name(s32 a0, s32 a1)
{
    s32 result;
    s32 save_a0;

    save_a0 = a0;

    switch (a1) {
    case 0:
        result = get_g_zyotai_s(a1);
        if (result & 0x80) {
            strcpy(save_a0, p_adana);
        } else {
            strcpy(save_a0, p_mei);
            strcat(save_a0, get_p_name_str0);
        }
        break;
    case 1:
        result = get_g_zyotai_s(a1);
        if (result & 0x80) {
            strcpy(save_a0, p_sei);
            strcat(save_a0, get_p_name_str0);
        } else {
            strcpy(save_a0, p_sei);
            strcat(save_a0, get_p_name_str1);
        }
        break;
    case 2:
        result = get_g_zyotai_s(a1);
        if (result & 0x80) {
            strcpy(save_a0, p_mei);
            strcat(save_a0, get_p_name_str0);
        } else {
            strcpy(save_a0, p_sei);
            strcat(save_a0, get_p_name_str0);
        }
        break;
    case 3:
        result = get_g_zyotai_s(a1);
        if (result & 0x80) {
            strcpy(save_a0, p_adana);
        } else {
            strcpy(save_a0, p_sei);
            strcat(save_a0, get_p_name_str0);
        }
        break;
    case 4:
        result = get_g_zyotai_s(a1);
        if (result & 0x80) {
            strcpy(save_a0, p_mei);
            strcat(save_a0, get_p_name_str0);
        } else {
            result = get_g_zyotai_s(a1);
            if ((result & 0x7f) == 4) {
                strcpy(save_a0, p_sei);
                strcat(save_a0, get_p_name_str0);
            } else {
                strcpy(save_a0, p_sei);
                strcat(save_a0, get_p_name_str0);
            }
        }
        break;
    case 5:
        result = get_g_zyotai_s(a1);
        if (result & 0x80) {
            strcpy(save_a0, p_mei);
            strcat(save_a0, get_p_name_str1);
        } else {
            result = get_g_zyotai_s(a1);
            if ((result & 0x7f) == 4) {
                strcpy(save_a0, p_sei);
                strcat(save_a0, get_p_name_str1);
            } else {
                strcpy(save_a0, p_sei);
                strcat(save_a0, get_p_name_str1);
            }
        }
        break;
    case 6:
        result = get_g_zyotai_s(a1);
        if (result & 0x80) {
            strcpy(save_a0, p_adana);
        } else {
            strcpy(save_a0, p_sei);
            strcat(save_a0, get_p_name_str0);
        }
        break;
    case 7:
        result = get_g_zyotai_s(a1);
        if (result & 0x80) {
            strcpy(save_a0, p_mei);
            strcat(save_a0, get_p_name_str0);
        } else {
            strcpy(save_a0, p_sei);
            strcat(save_a0, get_p_name_str0);
        }
        break;
    case 8:
        result = get_g_zyotai_s(a1);
        if (result & 0x80) {
            strcpy(save_a0, p_adana);
        } else {
            strcpy(save_a0, p_mei);
            strcat(save_a0, get_p_name_str0);
        }
        break;
    case 9:
        result = get_g_zyotai_s(a1);
        if (result & 0x80) {
            strcpy(save_a0, p_sei);
            strcat(save_a0, get_p_name_str0);
        } else {
            strcpy(save_a0, p_sei);
            strcat(save_a0, get_p_name_str1);
        }
        break;
    case 10:
        result = get_g_zyotai_s(a1);
        if (result & 0x80) {
            strcpy(save_a0, p_mei);
            strcat(save_a0, get_p_name_str1);
        } else {
            strcpy(save_a0, p_sei);
            strcat(save_a0, get_p_name_str1);
        }
        break;
    case 13:
        strcpy(save_a0, p_mei);
        break;
    case 11:
        strcpy(save_a0, p_mei);
        strcat(save_a0, get_p_name_str0);
        break;
    case 12:
        strcpy(save_a0, p_sei);
        strcat(save_a0, get_p_name_str0);
        break;
    case 15:
        strcpy(save_a0, p_sei);
        strncat(save_a0, p_mei, 6);
        break;
    case 14:
    default:
        strcpy(save_a0, p_sei);
        strcat(save_a0, get_p_name_str0);
        break;
    }
}
