/* @SymbolStart: 0x80143A38 */
/* @RodataBase: 0x8015DAD0 */
/* Address: 0x80143A40 */
/* @SymbolSize: 804 */
#include "types.h"

extern u8 taco_load_no;

extern void cd_read_start(s32, s32, s32);
extern void cd_read_start2(s32, u32, s32);
extern void device_load_start(u32 a0, u32 a1, u32 a2, u32 a3, u32 a4, u32 a5);
extern s32 cdq_idle_check(void);

void taco_cd_load_step(void)
{
    u8 sel = taco_load_no;

    switch ((u32)sel) {
    case 16:
        cd_read_start(32, 0x80162000, 0xB69B);
        cd_read_start(32, 0x80172000, 0xB6BB);
        cd_read_start(8, 0x80182000, 0xB47E);
        taco_load_no = taco_load_no + 1;
        break;
    case 20:
        device_load_start(0, 0xBF51, 0xBF34, 0xD31E, 0xD2D0, 0xD2AB);
        taco_load_no = taco_load_no + 1;
        break;
    case 24:
        cd_read_start(18, 0x8018D000, 0xB438);
        cd_read_start2(12, 0x801E0000, 0xB44A);
        cd_read_start(32, 0x80162000, 0xB418);
        taco_load_no = taco_load_no + 1;
        break;
    case 32:
        cd_read_start(32, 0x80162000, 0xB647);
        cd_read_start(32, 0x80172000, 0xB5A0);
        taco_load_no = taco_load_no + 1;
        break;
    case 36:
        device_load_start(0, 0xBF6F, 0xBF53, 0xD31E, 0xD2D0, 0xD2AB);
        taco_load_no = taco_load_no + 1;
        break;
    case 40:
        cd_read_start(7, 0x80196000, 0xB45B);
        cd_read_start2(5, 0x801E0000, 0xB462);
        cd_read_start(32, 0x80162000, 0xB535);
        taco_load_no = taco_load_no + 1;
        break;
    case 48:
        cd_read_start(32, 0x80162000, 0xB67B);
        cd_read_start(32, 0x80172000, 0xB5C1);
        taco_load_no = taco_load_no + 1;
        break;
    case 52:
        device_load_start(0, 0xBF85, 0xBF71, 0xD31E, 0xD2D0, 0xD2AB);
        taco_load_no = taco_load_no + 1;
        break;
    case 56:
        cd_read_start(10, 0x801A7000, 0xB515);
        cd_read_start2(21, 0x801E0000, 0xB51F);
        cd_read_start(32, 0x80162000, 0xB5FC);
        cd_read_start(32, 0x80172000, 0xB61D);
        taco_load_no = taco_load_no + 1;
        break;
    case 68:
        device_load_start(0, 0xC57B, 0xC55B, 0xD31E, 0xD2D0, 0xD2AB);
        taco_load_no = taco_load_no + 1;
        break;
    case 0:
        break;
    default:
        if (cdq_idle_check() == 1) {
            taco_load_no = 0;
        }
        break;
    }
}
