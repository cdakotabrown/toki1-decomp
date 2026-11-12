/* Address: 0x80084358 */
/* Size: 980 bytes (245 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @SymbolSize: 980 */
/* @CompileFlags: -cckr -Wc,-pic1 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern u8 now_girl;
extern u8 now_month;

void girl_message_load()
{
    switch (now_girl) {
    case 0:
        if (now_month >= 6 && now_month < 10) {
            bustup_cd_read(0x541D);
        } else {
            bustup_cd_read(0x544A);
        }
        break;
    case 1:
        if (now_month >= 6 && now_month < 10) {
            bustup_cd_read(0x5D14);
        } else {
            bustup_cd_read(0x5D41);
        }
        break;
    case 2:
        if (now_month >= 6 && now_month < 10) {
            bustup_cd_read(0x5201);
        } else {
            bustup_cd_read(0x522E);
        }
        break;
    case 3:
        if (now_month >= 6 && now_month < 10) {
            bustup_cd_read(0x5882);
        } else {
            bustup_cd_read(0x58AF);
        }
        break;
    case 4:
        if (now_month >= 6 && now_month < 10) {
            bustup_cd_read(0x6692);
        } else {
            bustup_cd_read(0x66BF);
        }
        break;
    case 5:
        if (now_month >= 6 && now_month < 10) {
            bustup_cd_read(0x5F8A);
        } else {
            bustup_cd_read(0x5FB7);
        }
        break;
    case 6:
        if (now_month >= 6 && now_month < 10) {
            bustup_cd_read(0x6200);
        } else {
            bustup_cd_read(0x622D);
        }
        break;
    case 7:
        if (now_month >= 6 && now_month < 10) {
            bustup_cd_read(0x5AF8);
        } else {
            bustup_cd_read(0x5B25);
        }
        break;
    case 8:
        if (now_month >= 6 && now_month < 10) {
            bustup_cd_read(0x4FE5);
        } else {
            bustup_cd_read(0x5012);
        }
        break;
    case 9:
        if (now_month >= 6 && now_month < 10) {
            bustup_cd_read(0x64A3);
        } else {
            bustup_cd_read(0x64D0);
        }
        break;
    case 10:
        if (now_month >= 6 && now_month < 10) {
            bustup_cd_read(0x6908);
        } else {
            bustup_cd_read(0x6935);
        }
        break;
    case 12:
        if (now_month >= 6 && now_month < 10) {
            bustup_cd_read(0x57FB);
        } else {
            bustup_cd_read(0x5828);
        }
        break;
    case 11:
        if (now_month >= 6 && now_month < 10) {
            bustup_cd_read(0x6B51);
        } else {
            bustup_cd_read(0x6B7E);
        }
        break;
    case 13:
        if (now_month >= 6 && now_month < 10) {
            bustup_cd_read(0x6C32);
        } else {
            bustup_cd_read(0x6C5F);
        }
        break;
    case 14:
        bustup_cd_read(0x6B24);
        break;
    }
    sub_sub_step_up();
}
