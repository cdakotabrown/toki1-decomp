/* @RodataBase: 0x8015B100 */
/* @SymbolSize: 624 */
/* date_main3_serifu_ptr_set - DATE overlay, Address: 0x80145E34, Size: 624 bytes (156 insns) */
#include "types.h"

extern u8 now_girl;

extern u32 date_ss_serifu3;
extern u32 date_ss_serifu3_kind;
extern u32 date_sss_sound3;

extern s32 date_sound3_tbl[];
extern s32 date_serifu3_kind_tbl[];
extern s32 date_serifu3_tbl[];

void date_main3_serifu_ptr_set(void) {
    switch (now_girl) {
    case 0:
        date_ss_serifu3 = (*(u32 *)&date_serifu3_tbl[1]);
        date_ss_serifu3_kind = (*(u32 *)&date_serifu3_kind_tbl[1]);
        date_sss_sound3 = (*(u32 *)&date_sound3_tbl[1]);
        break;
    case 1:
        date_ss_serifu3 = (*(u32 *)&date_serifu3_tbl[6]);
        date_ss_serifu3_kind = (*(u32 *)&date_serifu3_kind_tbl[6]);
        date_sss_sound3 = (*(u32 *)&date_sound3_tbl[6]);
        break;
    case 2:
        date_ss_serifu3 = (*(u32 *)&date_serifu3_tbl[3]);
        date_ss_serifu3_kind = (*(u32 *)&date_serifu3_kind_tbl[3]);
        date_sss_sound3 = (*(u32 *)&date_sound3_tbl[3]);
        break;
    case 3:
        date_ss_serifu3 = (*(u32 *)&date_serifu3_tbl[4]);
        date_ss_serifu3_kind = (*(u32 *)&date_serifu3_kind_tbl[4]);
        date_sss_sound3 = (*(u32 *)&date_sound3_tbl[4]);
        break;
    case 4:
        date_ss_serifu3 = (*(u32 *)&date_serifu3_tbl[10]);
        date_ss_serifu3_kind = (*(u32 *)&date_serifu3_kind_tbl[10]);
        date_sss_sound3 = (*(u32 *)&date_sound3_tbl[10]);
        break;
    case 5:
        date_ss_serifu3 = (*(u32 *)&date_serifu3_tbl[7]);
        date_ss_serifu3_kind = (*(u32 *)&date_serifu3_kind_tbl[7]);
        date_sss_sound3 = (*(u32 *)&date_sound3_tbl[7]);
        break;
    case 6:
        date_ss_serifu3 = (*(u32 *)&date_serifu3_tbl[8]);
        date_ss_serifu3_kind = (*(u32 *)&date_serifu3_kind_tbl[8]);
        date_sss_sound3 = (*(u32 *)&date_sound3_tbl[8]);
        break;
    case 7:
        date_ss_serifu3 = (*(u32 *)&date_serifu3_tbl[5]);
        date_ss_serifu3_kind = (*(u32 *)&date_serifu3_kind_tbl[5]);
        date_sss_sound3 = (*(u32 *)&date_sound3_tbl[5]);
        break;
    case 8:
        date_ss_serifu3 = (*(u32 *)&date_serifu3_tbl[0]);
        date_ss_serifu3_kind = (*(u32 *)&date_serifu3_kind_tbl[0]);
        date_sss_sound3 = (*(u32 *)&date_sound3_tbl[0]);
        break;
    case 9:
        date_ss_serifu3 = (*(u32 *)&date_serifu3_tbl[9]);
        date_ss_serifu3_kind = (*(u32 *)&date_serifu3_kind_tbl[9]);
        date_sss_sound3 = (*(u32 *)&date_sound3_tbl[9]);
        break;
    case 10:
        date_ss_serifu3 = (*(u32 *)&date_serifu3_tbl[12]);
        date_ss_serifu3_kind = (*(u32 *)&date_serifu3_kind_tbl[12]);
        date_sss_sound3 = (*(u32 *)&date_sound3_tbl[12]);
        break;
    }
}
