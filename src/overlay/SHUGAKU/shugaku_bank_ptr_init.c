#include "types.h"

extern s32 shugaku_serifu_tbl[];
extern s32 shugaku_serifu_kind_tbl[];
extern s32 shugaku_sound_tbl[];

void shugaku_bank_ptr_init(void)
{
    shugaku_serifu_tbl[0] = (s32)0x80197EB8;
    shugaku_serifu_tbl[1] = (s32)0x801999E8;
    shugaku_serifu_tbl[2] = (s32)0x8019B628;
    shugaku_serifu_tbl[3] = (s32)0x8019D378;
    shugaku_serifu_tbl[4] = (s32)0x8019F178;
    shugaku_serifu_tbl[5] = (s32)0x801A0C30;
    shugaku_serifu_tbl[6] = (s32)0x801A286C;
    shugaku_serifu_tbl[7] = (s32)0x801A5D20;
    shugaku_serifu_tbl[8] = (s32)0x801A435C;
    shugaku_serifu_tbl[9] = (s32)0x801A77F0;
    shugaku_serifu_tbl[10] = (s32)0x801A94BC;
    shugaku_serifu_tbl[11] = (s32)0x801AAFE8;
    shugaku_serifu_tbl[12] = (s32)0x801AC980;
    shugaku_serifu_kind_tbl[0] = (s32)0x80198054;
    shugaku_serifu_kind_tbl[1] = (s32)0x80199B80;
    shugaku_serifu_kind_tbl[2] = (s32)0x8019B7CC;
    shugaku_serifu_kind_tbl[3] = (s32)0x8019D518;
    shugaku_serifu_kind_tbl[4] = (s32)0x8019F310;
    shugaku_serifu_kind_tbl[5] = (s32)0x801A0DCC;
    shugaku_serifu_kind_tbl[6] = (s32)0x801A2A08;
    shugaku_serifu_kind_tbl[7] = (s32)0x801A5EB0;
    shugaku_serifu_kind_tbl[8] = (s32)0x801A44EC;
    shugaku_serifu_kind_tbl[9] = (s32)0x801A7994;
    shugaku_serifu_kind_tbl[10] = (s32)0x801A9660;
    shugaku_serifu_kind_tbl[11] = (s32)0x801AB164;
    shugaku_serifu_kind_tbl[12] = (s32)0x801ACA6C;
    shugaku_sound_tbl[0] = (s32)0x80198C34;
    shugaku_sound_tbl[1] = (s32)0x8019A5BC;
    shugaku_sound_tbl[2] = (s32)0x8019C48C;
    shugaku_sound_tbl[3] = (s32)0x8019E0C0;
    shugaku_sound_tbl[4] = (s32)0x8019FEB8;
    shugaku_sound_tbl[5] = (s32)0x801A1904;
    shugaku_sound_tbl[6] = (s32)0x801A3594;
    shugaku_sound_tbl[7] = (s32)0x801A6924;
    shugaku_sound_tbl[8] = (s32)0x801A4F28;
    shugaku_sound_tbl[9] = (s32)0x801A8638;
    shugaku_sound_tbl[10] = (s32)0x801AA180;
    shugaku_sound_tbl[11] = (s32)0x801ABB24;
    shugaku_sound_tbl[12] = (s32)0x801AD370;
}
