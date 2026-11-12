/* Address: 0x800563DC */
/* Size: 532 bytes (133 instructions) */
/* Region: Game (IDO 5.2 -O2, -Wc,-pic1) */
/* @SymbolStart: 0x800563DC */
/* @SymbolSize: 532 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: swap_instructions --offset1 0x14 --offset2 0x18 */
#include "types.h"

extern u8 save_name_set_tbl0[];
extern u8 save_name_set_str0[];
extern u8 save_name_set_str1[];
extern u8 save_name_set_str2[];
extern u8 save_name_set_str3[];
extern u8 save_name_set_str4[];
extern u8 save_name_set_str5[];
extern u8 save_name_set_str6[];
extern u8 save_name_set_str7[];
extern u8 save_name_set_str8[];
extern u8 save_name_set_str9[];
extern u8 save_name_set_str10[];
extern u8 save_name_set_str11[];
extern u8 save_name_set_str12[];
extern u8 save_name_set_str13[];
extern u8 save_name_set_str14[];
extern u8 save_name_set_str15[];

extern void strcpy(u8 *dst, u8 *src);
extern void strcat(u8 *dst, u8 *src);

s32 save_name_set(u8 *obj, u32 idx)
{
    s32 i;

    i = 0;
    while (i < 20) {
        obj[i++] = 0;
    }
    strcpy(obj, save_name_set_tbl0);
    switch (idx) {
    case 0:
        strcat(obj, save_name_set_str0);
        break;
    case 1:
        strcat(obj, save_name_set_str1);
        break;
    case 2:
        strcat(obj, save_name_set_str2);
        break;
    case 3:
        strcat(obj, save_name_set_str3);
        break;
    case 4:
        strcat(obj, save_name_set_str4);
        break;
    case 5:
        strcat(obj, save_name_set_str5);
        break;
    case 6:
        strcat(obj, save_name_set_str6);
        break;
    case 7:
        strcat(obj, save_name_set_str7);
        break;
    case 8:
        strcat(obj, save_name_set_str8);
        break;
    case 9:
        strcat(obj, save_name_set_str9);
        break;
    case 10:
        strcat(obj, save_name_set_str10);
        break;
    case 11:
        strcat(obj, save_name_set_str11);
        break;
    case 12:
        strcat(obj, save_name_set_str12);
        break;
    case 13:
        strcat(obj, save_name_set_str13);
        break;
    case 14:
        strcat(obj, save_name_set_str14);
        break;
    case 15:
        strcat(obj, save_name_set_str15);
        break;
    default:
        return 0;
    }
    return 1;
}
