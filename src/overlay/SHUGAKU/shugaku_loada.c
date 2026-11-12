/* @SymbolStart: 0x80137698 */
/* @SymbolSize: 428 */
/* @RodataBase: 0x8013AD70 */
/* shugaku_loada - SHUGAKU overlay, Address: 0x801376A0, Size: 420 bytes */
#include "types.h"

extern void cd_read_start2(s32 a0, s32 a1, s32 a2);
extern void game_reset(void);
extern void shugaku_bank_ptr_init2(void);
extern void sub_sub_step_up(void);

extern u8 now_girl;

void shugaku_loada(void)
{
    switch ((u32)now_girl) {
    case 0:
        cd_read_start2(6, (s32)0x80198000, 0xAE25);
        break;
    case 1:
        cd_read_start2(3, (s32)0x80198000, 0xAE39);
        break;
    case 2:
        cd_read_start2(4, (s32)0x80198000, 0xAE2B);
        break;
    case 3:
        cd_read_start2(5, (s32)0x80198000, 0xAE2F);
        break;
    case 4:
        cd_read_start2(7, (s32)0x80198000, 0xAE4C);
        break;
    case 5:
        cd_read_start2(5, (s32)0x80198000, 0xAE3C);
        break;
    case 6:
        cd_read_start2(6, (s32)0x80198000, 0xAE41);
        break;
    case 7:
        cd_read_start2(5, (s32)0x80198000, 0xAE34);
        break;
    case 8:
        cd_read_start2(4, (s32)0x80198000, 0xAE21);
        break;
    case 9:
        cd_read_start2(5, (s32)0x80198000, 0xAE47);
        break;
    case 10:
        cd_read_start2(6, (s32)0x80198000, 0xAE5A);
        break;
    case 11:
    case 12:
    case 13:
        cd_read_start2(7, (s32)0x80198000, 0xAE53);
        break;
    default:
        game_reset();
        break;
    }
    shugaku_bank_ptr_init2();
    sub_sub_step_up();
}
