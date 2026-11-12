/* @SymbolSize: 108 */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:6 */
/* Address: 0x801335C4, Size: 108 bytes (27 instructions); the .sym window */
#include "types.h"
#include "tokimeki/class_w.h"

extern s32 _debug_number_show();

typedef struct {
    u8 pad0[3];
    u8 flags;
    u8 pad4[20];
    s16 field_24;
    u8 pad26[12];
    s16 field_38;
    u8 pad40[2];
    s16 field_42;
    u8 pad44[24];
} Entry;

extern Entry srn_w;
extern CLASS_W class_w[];

s32 option_slot_num_show(void)
{
    Entry *p = &(*(Entry *)&class_w[96]), *end = &srn_w;
    s32 result = 96;

    do {
        if (p->flags & 0x80) {
            result = _debug_number_show(p->field_24, p->field_38, p->field_42, 15);
        }
        p++;
    } while (p != end);
    return result;
}
