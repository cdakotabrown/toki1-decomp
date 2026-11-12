/* @SymbolStart: 0x8013A2F4 */
/* @SymbolSize: 368 */
/* @CompileEnv: IDO52_AS1_XBB_VOLATILE_OK=all IDO52_FORCE_WEB_COLOR=0:v1,9:v0 */
#include "types.h"

extern volatile s32 rpg_bat_unit3_cnt;
extern volatile u8 cw144_clut;
extern volatile u8 cw145_clut;
extern volatile u8 cw144_bright;

extern s32 rpg_bat_blink_set();
extern s32 rpg_bat_seq_next();

void rpg_bat_blink(void)
{
    s32 counter;

    counter = rpg_bat_unit3_cnt;
    counter++;

    switch (counter % 4) {
    case 0:
        rpg_bat_unit3_cnt = counter;
        if (counter < 120) {
            rpg_bat_blink_set(30, 255);
        } else {
            rpg_bat_blink_set(31, 255);
        }
        break;
    case 1:
        rpg_bat_unit3_cnt = counter;
        if (counter < 120) {
            rpg_bat_blink_set(30, 128);
        } else {
            rpg_bat_blink_set(31, 128);
        }
        break;
    case 2:
        rpg_bat_unit3_cnt = counter;
        if (counter < 60) {
            rpg_bat_blink_set(30, 255);
        } else {
            rpg_bat_blink_set(31, 255);
        }
        break;
    case 3:
        rpg_bat_unit3_cnt = counter;
        if (counter < 60) {
            rpg_bat_blink_set(30, 128);
        } else {
            rpg_bat_blink_set(31, 128);
        }
        break;
    default:
        rpg_bat_unit3_cnt = counter;
        break;
    }

    counter = rpg_bat_unit3_cnt;
    if (counter >= 181) {
        cw144_clut = 31;
        cw145_clut = 31;
        cw144_bright = 128;
        rpg_bat_seq_next();
    }
}
