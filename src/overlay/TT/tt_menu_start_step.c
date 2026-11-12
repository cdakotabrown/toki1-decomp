/* @SymbolStart: 0x80149E90 */
/* @SymbolSize: 352 */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=5:v1,26:a2 */
/* tt_menu_start_step - TT overlay, Address: 0x80149E98, Size: 344 bytes (86 insns) */
#include "types.h"

extern u8 *tt_player_wk;
extern u8 *tt_step_wk;
extern u8 *tt_scroll_wk;
extern u8 *tt_text_buf;

extern void tt_cmd_once(s32 soundId);

void tt_menu_start_step(void)
{
    u8 *g74;
    u8 *g7C;
    u8 *gB8;
    s32 pos;
    s32 dist;
    s32 a;
    s32 base;
    u8 *obj;

    obj = tt_player_wk;
    pos = *(s32 *)(obj + 36);
    g74 = tt_step_wk;
    g7C = tt_scroll_wk;
    gB8 = tt_text_buf;

    dist = pos + (s32)0xFF640000;
    if (dist < 0) {
        base = 0x009C0000;
        a = base - pos;
    } else {
        a = dist;
        base = 0x009C0000;
    }
    if (a < 0x8001) {
        *(s32 *)(obj + 36) = base;
    } else if ((pos ^ 0) < base) {
        *(s32 *)(obj + 36) = *(volatile s32 *)(obj + 36) + 0x8000;
    } else {
        *(s32 *)(obj + 36) = *(volatile s32 *)(obj + 36) - 0x8000;
    }

    *(s16 *)(obj + 22) = *(s16 *)(obj + 38);

    if (*(u16 *)(g7C + 16) >= 30U) {
        *(u16 *)(g7C + 16) = 0;
        *gB8 = 1;
        *(u16 *)(g74 + 2) = *(u16 *)(g74 + 2) + 1;
    }

    switch (*(u16 *)(g7C + 16)) {
    case 0:
        obj[62] = 5;
        break;
    case 1:
        tt_cmd_once(1537);
        break;
    case 6:
        *(s8 *)(obj + 62) = 6;
        break;
    case 30:
        obj[62] = 7;
        break;
    }
    *(u16 *)(g7C + 16) = *(u16 *)(g7C + 16) + 1;
}
