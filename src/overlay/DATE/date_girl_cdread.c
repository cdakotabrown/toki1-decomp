/* @SymbolStart: 0x8013E73C */
/* @SymbolSize: 464 */
/* @RodataBase: 0x8015AFD4 */
#include "types.h"

extern u8 girl_no;

extern void date_girl0_ptr_set(void);
extern void date_girl1_ptr_set(void);
extern void date_girl2_ptr_set(void);
extern void date_girl3_ptr_set(void);
extern void date_girl4_ptr_set(void);
extern void date_girl5_ptr_set(void);
extern void date_girl6_ptr_set(void);
extern void date_girl7_ptr_set(void);
extern void date_girl8_ptr_set(void);
extern void date_girl9_ptr_set(void);
extern void date_girl_ptr_set(void);
extern void cd_read_start2(s32, s32, s32);
extern void sub_sub_step_up(void);

void date_girl_cdread(void)
{
    switch (girl_no) {
    case 0:
        date_girl0_ptr_set();
        cd_read_start2(23, 0x80198000, 0xAD29);
        break;
    case 1:
        date_girl1_ptr_set();
        cd_read_start2(24, 0x80198000, 0xAD82);
        break;
    case 2:
        date_girl2_ptr_set();
        cd_read_start2(21, 0x80198000, 0xAD40);
        break;
    case 3:
        date_girl3_ptr_set();
        cd_read_start2(22, 0x80198000, 0xAD55);
        break;
    case 4:
        date_girl4_ptr_set();
        cd_read_start2(22, 0x80198000, 0xADDB);
        break;
    case 5:
        date_girl5_ptr_set();
        cd_read_start2(23, 0x80198000, 0xAD9A);
        break;
    case 6:
        date_girl6_ptr_set();
        cd_read_start2(21, 0x80198000, 0xADB1);
        break;
    case 7:
        date_girl7_ptr_set();
        cd_read_start2(23, 0x80198000, 0xAD6B);
        break;
    case 8:
        date_girl8_ptr_set();
        cd_read_start2(20, 0x80198000, 0xAD15);
        break;
    case 9:
        date_girl9_ptr_set();
        cd_read_start2(21, 0x80198000, 0xADC6);
        break;
    default:
        date_girl_ptr_set();
        cd_read_start2(22, 0x80198000, 0xADF1);
        break;
    }
    sub_sub_step_up();
}
