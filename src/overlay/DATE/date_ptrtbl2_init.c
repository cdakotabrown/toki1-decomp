/* @SymbolSize: 196 */
/* Function: date_ptrtbl2_init */
/* Overlay: DATE */
/* Address: 0x80140EC0 */
/* Size: 196 bytes */
#include "types.h"

extern volatile s32 date_serifu0_tbl;
extern volatile s32 date_serifu0_tbl_1;
extern volatile s32 date_serifu0_tbl_2;
extern volatile s32 date_serifu0_tbl_3;
extern volatile s32 date_serifu0_kind_tbl;
extern volatile s32 date_serifu0_kind_tbl_1;
extern volatile s32 date_serifu0_kind_tbl_2;
extern volatile s32 date_serifu0_kind_tbl_3;
extern volatile s32 date_sound0_tbl;
extern volatile s32 date_sound0_tbl_1;
extern volatile s32 date_sound0_tbl_2;
extern volatile s32 date_sound0_tbl_3;

void date_ptrtbl2_init(void) {
    date_serifu0_tbl = (s32)0x801972E8;
    date_serifu0_tbl_1 = (s32)0x8019772C;
    date_serifu0_tbl_2 = (s32)0x80197B04;
    date_serifu0_tbl_3 = (s32)0x80197EA4;
    date_serifu0_kind_tbl = (s32)0x801972FC;
    date_serifu0_kind_tbl_1 = (s32)0x80197778;
    date_serifu0_kind_tbl_2 = (s32)0x80197B10;
    date_serifu0_kind_tbl_3 = (s32)0x80197EB4;
    date_sound0_tbl = (s32)0x80197450;
    date_sound0_tbl_1 = (s32)0x80197994;
    date_sound0_tbl_2 = (s32)0x80197BD8;
    date_sound0_tbl_3 = (s32)0x80197FEC;
}
