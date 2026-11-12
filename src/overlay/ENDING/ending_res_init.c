/* @SymbolSize: 724 */
/* ending_res_init - ENDING overlay, Address: 0x80139700, Size: 724 bytes (181 insns) */
#include "types.h"

extern s32 ending_result_serifu_tbl[];
extern s32 ending_result_kind_tbl[];
extern s32 ending_result_sound_tbl[];

void ending_res_init(void)
{
    ending_result_serifu_tbl[0] = 0x80180308;
    ending_result_serifu_tbl[1] = 0x80180728;
    ending_result_serifu_tbl[2] = 0x80180BCC;
    ending_result_serifu_tbl[3] = 0x80180ED4;
    ending_result_serifu_tbl[4] = 0x801812E4;
    ending_result_serifu_tbl[5] = 0x80181648;
    ending_result_serifu_tbl[6] = 0x80181A84;
    ending_result_serifu_tbl[7] = 0x80181ED0;
    ending_result_serifu_tbl[8] = 0x80182258;
    ending_result_serifu_tbl[9] = 0x80182684;
    ending_result_serifu_tbl[10] = 0x80182AB4;
    ending_result_serifu_tbl[11] = 0x80182C74;
    ending_result_serifu_tbl[12] = 0x801835E4;
    ending_result_serifu_tbl[13] = 0x80183CC4;
    ending_result_serifu_tbl[14] = 0x80184008;
    ending_result_kind_tbl[0] = 0x80180314;
    ending_result_kind_tbl[1] = 0x80180734;
    ending_result_kind_tbl[2] = 0x80180BD8;
    ending_result_kind_tbl[3] = 0x80180EE0;
    ending_result_kind_tbl[4] = 0x801812F0;
    ending_result_kind_tbl[5] = 0x80181654;
    ending_result_kind_tbl[6] = 0x80181A90;
    ending_result_kind_tbl[7] = 0x80181EDC;
    ending_result_kind_tbl[8] = 0x80182264;
    ending_result_kind_tbl[9] = 0x80182690;
    ending_result_kind_tbl[10] = 0x80182AC0;
    ending_result_kind_tbl[11] = 0x80182C80;
    ending_result_kind_tbl[12] = 0x8018360C;
    ending_result_kind_tbl[13] = 0x80183CD4;
    ending_result_kind_tbl[14] = 0x80184014;
    ending_result_sound_tbl[0] = 0x801803DC;
    ending_result_sound_tbl[1] = 0x801807FC;
    ending_result_sound_tbl[2] = 0x80180CA0;
    ending_result_sound_tbl[3] = 0x80180FA8;
    ending_result_sound_tbl[4] = 0x801813B8;
    ending_result_sound_tbl[5] = 0x8018171C;
    ending_result_sound_tbl[6] = 0x80181B58;
    ending_result_sound_tbl[7] = 0x80181FA4;
    ending_result_sound_tbl[8] = 0x8018232C;
    ending_result_sound_tbl[9] = 0x80182758;
    ending_result_sound_tbl[10] = 0x80182B88;
    ending_result_sound_tbl[11] = 0x80182D48;
    ending_result_sound_tbl[12] = 0x801839E4;
    ending_result_sound_tbl[13] = 0x80183DB8;
    ending_result_sound_tbl[14] = 0x801840DC;
}
