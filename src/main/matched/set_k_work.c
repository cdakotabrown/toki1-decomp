/* Function: set_k_work */
/* Address: 0x8004EDD0 */
/* Size: 76 bytes (19 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

typedef struct {
    s16 x;
    s16 y;
    u8  a;
    u8  b;
    u8  c;
    s8  d;
} Ent8;

extern Ent8 disp_string_tbl0[];

void set_k_work(s32 idx, Ent8 e)
{
    disp_string_tbl0[idx].x = e.x;
    disp_string_tbl0[idx].y = e.y;
    disp_string_tbl0[idx].a = e.a;
    disp_string_tbl0[idx].b = e.b;
    disp_string_tbl0[idx].c = e.c;
    disp_string_tbl0[idx].d = e.d;
}
