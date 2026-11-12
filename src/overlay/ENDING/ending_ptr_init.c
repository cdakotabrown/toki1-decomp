/* Function: ending_ptr_init */
/* Overlay: ENDING */
/* Address: 0x80132000 */
/* Size: 820 bytes (205 instructions) */
#include "types.h"

extern s32 ending_serifu_tbl[];
extern s32 ending_serifu_kind_tbl[];
extern s32 ending_sound_tbl[];

void ending_ptr_init(void) {
    ending_serifu_tbl[0] = (s32)0x801F00B8;
    ending_serifu_tbl[1] = (s32)0x801F05A8;
    ending_serifu_tbl[2] = (s32)0x801F0C70;
    ending_serifu_tbl[3] = (s32)0x801F0F34;
    ending_serifu_tbl[4] = (s32)0x801F14C4;
    ending_serifu_tbl[5] = (s32)0x801F1FF4;
    ending_serifu_tbl[6] = (s32)0x801F2A20;
    ending_serifu_tbl[7] = (s32)0x801F3364;
    ending_serifu_tbl[8] = (s32)0x801F416C;
    ending_serifu_tbl[9] = (s32)0x801F4D84;
    ending_serifu_tbl[10] = (s32)0x801F5C40;
    ending_serifu_tbl[11] = (s32)0x801F6704;
    ending_serifu_tbl[12] = (s32)0x801F6E08;
    ending_serifu_tbl[13] = (s32)0x801F7120;
    ending_serifu_tbl[14] = (s32)0x801F7608;
    ending_serifu_tbl[15] = (s32)0x801F7ADC;
    ending_serifu_tbl[16] = (s32)0x801F8004;
    ending_serifu_kind_tbl[0] = (s32)0x801F00D0;
    ending_serifu_kind_tbl[1] = (s32)0x801F05C4;
    ending_serifu_kind_tbl[2] = (s32)0x801F0C8C;
    ending_serifu_kind_tbl[3] = (s32)0x801F0F44;
    ending_serifu_kind_tbl[4] = (s32)0x801F14E4;
    ending_serifu_kind_tbl[5] = (s32)0x801F2028;
    ending_serifu_kind_tbl[6] = (s32)0x801F2A40;
    ending_serifu_kind_tbl[7] = (s32)0x801F3388;
    ending_serifu_kind_tbl[8] = (s32)0x801F41B0;
    ending_serifu_kind_tbl[9] = (s32)0x801F4D9C;
    ending_serifu_kind_tbl[10] = (s32)0x801F5C88;
    ending_serifu_kind_tbl[11] = (s32)0x801F6720;
    ending_serifu_kind_tbl[12] = (s32)0x801F6E24;
    ending_serifu_kind_tbl[13] = (s32)0x801F7128;
    ending_serifu_kind_tbl[14] = (s32)0x801F7624;
    ending_serifu_kind_tbl[15] = (s32)0x801F7B18;
    ending_serifu_kind_tbl[16] = (s32)0x801F801C;
    ending_sound_tbl[0] = (s32)0x801F01A0;
    ending_sound_tbl[1] = (s32)0x801F07F8;
    ending_sound_tbl[2] = (s32)0x801F0EC0;
    ending_sound_tbl[3] = (s32)0x801F0F84;
    ending_sound_tbl[4] = (s32)0x801F17DC;
    ending_sound_tbl[5] = (s32)0x801F24FC;
    ending_sound_tbl[6] = (s32)0x801F2D38;
    ending_sound_tbl[7] = (s32)0x801F36F0;
    ending_sound_tbl[8] = (s32)0x801F47D8;
    ending_sound_tbl[9] = (s32)0x801F4F98;
    ending_sound_tbl[10] = (s32)0x801F6358;
    ending_sound_tbl[11] = (s32)0x801F6954;
    ending_sound_tbl[12] = (s32)0x801F7074;
    ending_sound_tbl[13] = (s32)0x801F7180;
    ending_sound_tbl[14] = (s32)0x801F7874;
    ending_sound_tbl[15] = (s32)0x801F7D0C;
    ending_sound_tbl[16] = (s32)0x801F81E0;
}
