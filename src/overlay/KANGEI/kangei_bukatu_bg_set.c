/* @SymbolStart: 0x80134BD4 */
/* @SymbolSize: 492 */
/* @RodataBase: 0x80139878 */
/* kangei_bukatu_bg_set -- KANGEI overlay, Address: 0x80134BDC, Size: 492 bytes */
#include "types.h"

extern void strcpy(u8 *, u8 *);
extern void dec_bg_cache_step(s32);
extern void event_log_add(s32, s32);
extern void sub_sub_step_up(void);

extern u8  ba_string[];
extern u16 basyo_pack;
extern u8  kangei_bukatu_bg_set_str0[];
extern u8  kangei_bukatu_bg_set_str1[];
extern u8  kangei_bukatu_bg_set_str2[];
extern u8  kangei_bukatu_bg_set_str3[];
extern u8  kangei_bukatu_bg_set_str4[];
extern u8  kangei_bukatu_bg_set_str5[];
extern u8  kangei_bukatu_bg_set_str6[];
extern u8  kangei_bukatu_bg_set_str7[];
extern u8  kangei_bukatu_bg_set_str8[];
extern u8  kangei_bukatu_bg_set_str9[];
extern u8  kangei_bukatu_bg_set_str10[];

void kangei_bukatu_bg_set(void)
{
    switch ((u32)basyo_pack >> 12) {
    case 0:
        strcpy(ba_string, kangei_bukatu_bg_set_str0);
        dec_bg_cache_step(16572);
        break;
    case 1:
        strcpy(ba_string, kangei_bukatu_bg_set_str1);
        dec_bg_cache_step(16562);
        break;
    case 2:
        strcpy(ba_string, kangei_bukatu_bg_set_str2);
        dec_bg_cache_step(16582);
        break;
    case 3:
        strcpy(ba_string, kangei_bukatu_bg_set_str3);
        dec_bg_cache_step(16592);
        break;
    case 4:
        strcpy(ba_string, kangei_bukatu_bg_set_str4);
        dec_bg_cache_step(16601);
        break;
    case 5:
        strcpy(ba_string, kangei_bukatu_bg_set_str5);
        dec_bg_cache_step(16610);
        break;
    case 6:
        strcpy(ba_string, kangei_bukatu_bg_set_str6);
        dec_bg_cache_step(17046);
        break;
    case 7:
        strcpy(ba_string, kangei_bukatu_bg_set_str7);
        dec_bg_cache_step(17046);
        break;
    case 8:
        strcpy(ba_string, kangei_bukatu_bg_set_str8);
        dec_bg_cache_step(17046);
        break;
    case 9:
        strcpy(ba_string, kangei_bukatu_bg_set_str9);
        dec_bg_cache_step(16544);
        break;
    default:
        strcpy(ba_string, kangei_bukatu_bg_set_str10);
        dec_bg_cache_step(16553);
        break;
    }

    event_log_add(14, ((u32)basyo_pack >> 12) & 0xFF);
    sub_sub_step_up();
}
