#include "types.h"

extern s32 shugaku_pbirth_serifu_tbl[];
extern s32 shugaku_pbirth_serifu_kind_tbl[];
extern s32 shugaku_pbirth_sound_tbl[];

void shugaku_tbl_init(void)
{
    shugaku_pbirth_serifu_tbl[0] = (s32)0x801B00DC;
    shugaku_pbirth_serifu_tbl[1] = (s32)0x801B02AC;
    shugaku_pbirth_serifu_tbl[2] = (s32)0x801B03C8;
    shugaku_pbirth_serifu_tbl[3] = (s32)0x801B04C4;
    shugaku_pbirth_serifu_tbl[4] = (s32)0x801B0668;
    shugaku_pbirth_serifu_tbl[5] = (s32)0x801B0808;
    shugaku_pbirth_serifu_tbl[6] = (s32)0x801B09DC;
    shugaku_pbirth_serifu_tbl[7] = (s32)0x801B0BE8;
    shugaku_pbirth_serifu_tbl[8] = (s32)0x801B0D8C;
    shugaku_pbirth_serifu_tbl[9] = (s32)0x801B0F5C;
    shugaku_pbirth_serifu_tbl[10] = (s32)0x801B1164;
    shugaku_pbirth_serifu_tbl[11] = (s32)0x801B12D8;
    shugaku_pbirth_serifu_tbl[12] = (s32)0x801B1454;
    shugaku_pbirth_serifu_kind_tbl[0] = (s32)0x801B00F4;
    shugaku_pbirth_serifu_kind_tbl[1] = (s32)0x801B02C8;
    shugaku_pbirth_serifu_kind_tbl[2] = (s32)0x801B03D0;
    shugaku_pbirth_serifu_kind_tbl[3] = (s32)0x801B04DC;
    shugaku_pbirth_serifu_kind_tbl[4] = (s32)0x801B0680;
    shugaku_pbirth_serifu_kind_tbl[5] = (s32)0x801B0820;
    shugaku_pbirth_serifu_kind_tbl[6] = (s32)0x801B09F8;
    shugaku_pbirth_serifu_kind_tbl[7] = (s32)0x801B0C00;
    shugaku_pbirth_serifu_kind_tbl[8] = (s32)0x801B0DA4;
    shugaku_pbirth_serifu_kind_tbl[9] = (s32)0x801B0F78;
    shugaku_pbirth_serifu_kind_tbl[10] = (s32)0x801B1180;
    shugaku_pbirth_serifu_kind_tbl[11] = (s32)0x801B12E0;
    shugaku_pbirth_serifu_kind_tbl[12] = (s32)0x801B1470;
    shugaku_pbirth_sound_tbl[0] = (s32)0x801B01A8;
    shugaku_pbirth_sound_tbl[1] = (s32)0x801B0398;
    shugaku_pbirth_sound_tbl[2] = (s32)0x801B03F0;
    shugaku_pbirth_sound_tbl[3] = (s32)0x801B0590;
    shugaku_pbirth_sound_tbl[4] = (s32)0x801B0734;
    shugaku_pbirth_sound_tbl[5] = (s32)0x801B08D4;
    shugaku_pbirth_sound_tbl[6] = (s32)0x801B0AE4;
    shugaku_pbirth_sound_tbl[7] = (s32)0x801B0CB4;
    shugaku_pbirth_sound_tbl[8] = (s32)0x801B0E58;
    shugaku_pbirth_sound_tbl[9] = (s32)0x801B1064;
    shugaku_pbirth_sound_tbl[10] = (s32)0x801B126C;
    shugaku_pbirth_sound_tbl[11] = (s32)0x801B1338;
    shugaku_pbirth_sound_tbl[12] = (s32)0x801B155C;
}
