/* @SymbolStart: 0x80133E14 */
/* @RodataBase: 0x80134228 */
/* @SymbolSize: 260 */
/* rensyu_bustup_read - RENSYU overlay, Address: 0x80133E1C, Size: 252 bytes (63 insns) */
#include "types.h"

extern u8 rensyu_girl_no;

extern u8 girl0_basyo;
extern void bustup_cd_read(u32 a0);
extern void game_reset(void);

void rensyu_bustup_read(void)
{
    u32 t6 = rensyu_girl_no;

    u8 t7;
    u32 t8;

    switch (t6) {
    case 0:
        t7 = girl0_basyo;
        t8 = ((u32)t7 >> 4) - 6;
        switch (t8) {
        case 0:
        case 1:
            bustup_cd_read(21758);
            return;
        case 2:
            bustup_cd_read(21803);
            return;
        case 3:
            bustup_cd_read(21848);
            return;
        case 4:
            bustup_cd_read(21893);
            return;
        }
        return;
    case 4:
        bustup_cd_read(26348);
        return;
    case 5:
        bustup_cd_read(24548);
        return;
    case 6:
        bustup_cd_read(25178);
        return;
    case 10:
        bustup_cd_read(26978);
        return;
    }

    game_reset();
}
