/* @CompileEnv: IDO52_FORCE_WEB_COLOR=0:v1 IDO52_COPYPROP_VAR=-4 */
/* @SymbolStart: 0x80139DB4 */
/* @SymbolSize: 144 */
/* name_phase_disp - NAME_ENT overlay, Address: 0x80139DBC, Size: 136 bytes (34 insns), Semi-matched */
#include "types.h"

extern void name_kbd_disp();
extern void name_slots_disp();
extern void name_attr_set();
extern void name_pad_advance();

extern volatile s32 name_ent_view_page;

void name_phase_disp(void)
{
    s32 v;
    s32 idx;

    v = name_ent_view_page;
    if (v < 0) {
        v = v + 3;
    }
    idx = v % 3;
    name_ent_view_page = v;
    v = 0;

    switch (idx) {
    case 0:
        name_kbd_disp();
        break;
    case 1:
        name_slots_disp();
        break;
    case 2:
        name_attr_set();
        break;
    }

    name_pad_advance();
}
