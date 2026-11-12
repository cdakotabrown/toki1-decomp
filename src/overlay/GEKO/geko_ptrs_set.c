/* Function: geko_ptrs_set */
/* Overlay: GEKO */
/* Address: 0x80132000 */
/* Size: 580 bytes (145 instructions) */
#include "types.h"

extern s32 geko_serifu_tbl[];
extern s32 geko_serifu_kind_tbl[];
extern s32 geko_sound_tbl[];

void geko_ptrs_set(void) {
    geko_serifu_tbl[0] = (s32)0x80197BFC;
    geko_serifu_tbl[1] = (s32)0x801996B8;
    geko_serifu_tbl[2] = (s32)0x8019B050;
    geko_serifu_tbl[3] = (s32)0x8019C9A8;
    geko_serifu_tbl[4] = (s32)0x8019E3F0;
    geko_serifu_tbl[5] = (s32)0x8019FF58;
    geko_serifu_tbl[6] = (s32)0x801A18FC;
    geko_serifu_tbl[7] = (s32)0x801A3030;
    geko_serifu_tbl[8] = (s32)0x801A48F4;
    geko_serifu_tbl[9] = (s32)0x801A63EC;
    geko_serifu_tbl[10] = (s32)0x801A7324;
    geko_serifu_tbl[11] = (s32)0x801A7F90;
    geko_serifu_kind_tbl[0] = (s32)0x80197D80;
    geko_serifu_kind_tbl[1] = (s32)0x80199838;
    geko_serifu_kind_tbl[2] = (s32)0x8019B1C8;
    geko_serifu_kind_tbl[3] = (s32)0x8019CB2C;
    geko_serifu_kind_tbl[4] = (s32)0x8019E584;
    geko_serifu_kind_tbl[5] = (s32)0x801A00D8;
    geko_serifu_kind_tbl[6] = (s32)0x801A1A60;
    geko_serifu_kind_tbl[7] = (s32)0x801A31AC;
    geko_serifu_kind_tbl[8] = (s32)0x801A4A74;
    geko_serifu_kind_tbl[9] = (s32)0x801A657C;
    geko_serifu_kind_tbl[10] = (s32)0x801A7354;
    geko_serifu_kind_tbl[11] = (s32)0x801A8108;
    geko_sound_tbl[0] = (s32)0x80198980;
    geko_sound_tbl[1] = (s32)0x8019A3C8;
    geko_sound_tbl[2] = (s32)0x8019BD20;
    geko_sound_tbl[3] = (s32)0x8019D6BC;
    geko_sound_tbl[4] = (s32)0x8019F1C4;
    geko_sound_tbl[5] = (s32)0x801A0C84;
    geko_sound_tbl[6] = (s32)0x801A24F4;
    geko_sound_tbl[7] = (s32)0x801A3D04;
    geko_sound_tbl[8] = (s32)0x801A5674;
    geko_sound_tbl[9] = (s32)0x801A7134;
    geko_sound_tbl[10] = (s32)0x801A7414;
    geko_sound_tbl[11] = (s32)0x801A8C28;
}
