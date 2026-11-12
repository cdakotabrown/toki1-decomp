/* @RodataBase: 0x8015CE74 */
/* @SymbolSize: 600 */
/* @PostProc: scheduler_normalizer --block-aware */
/* suuzi_str - RPG_BAT overlay, Address: 0x8014D11C, Size: 600 bytes (150 insns) */
#include "types.h"

extern s32 strcpy();
extern s32 strcat();

extern u8 suuzi_str_str0[];
extern u8 suuzi_str_str1[];
extern u8 suuzi_str_str2[];
extern u8 suuzi_str_str3[];
extern u8 suuzi_str_str4[];
extern u8 suuzi_str_str5[];
extern u8 suuzi_str_str6[];
extern u8 suuzi_str_str7[];
extern u8 suuzi_str_str8[];
extern u8 suuzi_str_str9[];
extern u8 suuzi_str_str10[];

void suuzi_str(s32 a0, s32 a1, s32 a2, s32 a3, s32 arg5)
{
    s32 divisor;
    s32 quotient;
    s32 ten;
    u32 digit;
    s32 *pnum;

    pnum = &a3;
    strcpy(a0, a1, a2, a3);
    strcat(a0, a2);
    if (a3 == 0) {
        strcat(a0, suuzi_str_str0);
    }
    divisor = 1000;
    ten = 10;
    do {
        quotient = *pnum / divisor;
        if (quotient != 0) {
            digit = (u32)quotient % ten;
            switch (digit) {
            case 0:
                strcat(a0, suuzi_str_str1);
                break;
            case 1:
                strcat(a0, suuzi_str_str2);
                break;
            case 2:
                strcat(a0, suuzi_str_str3);
                break;
            case 3:
                strcat(a0, suuzi_str_str4);
                break;
            case 4:
                strcat(a0, suuzi_str_str5);
                break;
            case 5:
                strcat(a0, suuzi_str_str6);
                break;
            case 6:
                strcat(a0, suuzi_str_str7);
                break;
            case 7:
                strcat(a0, suuzi_str_str8);
                break;
            case 8:
                strcat(a0, suuzi_str_str9);
                break;
            case 9:
                strcat(a0, suuzi_str_str10);
                break;
            }
        }
        divisor = divisor / ten;
    } while (divisor > 0);
    strcat(a0, arg5);
}
