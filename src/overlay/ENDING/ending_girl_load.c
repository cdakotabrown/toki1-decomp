/* @SymbolStart: 0x80136F04 */
/* @SymbolSize: 580 */
/* @RodataBase: 0x8013C070 */
/* ending_girl_load - ENDING overlay, Address: 0x80136F0C, Size: 572 bytes (143 instructions) */
#include "types.h"

extern void cd_read_start2();
extern s32 girl1_flag;
extern u8 now_girl;
extern u8 mascot_flag2_6;
extern void ending_girl_init0();
extern void ending_girl_init1a();
extern void ending_girl_ptr_set1();
extern void ending_girl_init2();
extern void ending_girl3_set();
extern void ending_girl4_set();
extern void ending_girl5_set();
extern void ending_girl6_set();
extern void ending_girl_ptr_set6();
extern void ending_girl7_set();
extern void ending_girl8_set();
extern void ending_girl9_set();
extern void ending_girl10_set();
extern void ending_girl11_set();
extern void ending_girl_def_set();
extern void sub_sub_step_up();

void ending_girl_load(void)
{
    u8 t6_hidden = now_girl;
    u8 t0;
    s32 t1;

    switch (t6_hidden) {
    case 0:
        cd_read_start2(141, 0x801A0000, 29516);
        ending_girl_init0();
        break;
    case 1:
        if (((u32)girl1_flag << 17) >> 31 == 0) {
            cd_read_start2(133, 0x801A0000, 30213);
            ending_girl_init1a();
        } else {
            cd_read_start2(141, 0x801A0000, 30346);
            ending_girl_ptr_set1();
        }
        break;
    case 2:
        cd_read_start2(125, 0x801A0000, 29657);
        ending_girl_init2();
        break;
    case 3:
        cd_read_start2(141, 0x801A0000, 29923);
        ending_girl3_set();
        break;
    case 4:
        cd_read_start2(141, 0x801A0000, 31092);
        ending_girl4_set();
        break;
    case 5:
        cd_read_start2(150, 0x801A0000, 30487);
        ending_girl5_set();
        break;
    case 6:
        t0 = mascot_flag2_6;
        t1 = t0 & 0x01;
        if (t1 == 0) {
            cd_read_start2(157, 0x801A0000, 30637);
            ending_girl6_set();
        } else {
            cd_read_start2(157, 0x801A0000, 30794);
            ending_girl_ptr_set6();
        }
        break;
    case 7:
        cd_read_start2(149, 0x801A0000, 30064);
        ending_girl7_set();
        break;
    case 8:
        cd_read_start2(141, 0x801A0000, 29375);
        ending_girl8_set();
        break;
    case 9:
        cd_read_start2(141, 0x801A0000, 30951);
        ending_girl9_set();
        break;
    case 10:
        cd_read_start2(141, 0x801A0000, 31233);
        ending_girl10_set();
        break;
    case 11:
        cd_read_start2(141, 0x801A0000, 31374);
        ending_girl11_set();
        break;
    default:
        cd_read_start2(141, 0x801A0000, 29782);
        ending_girl_def_set();
        break;
    }

    sub_sub_step_up();
}
