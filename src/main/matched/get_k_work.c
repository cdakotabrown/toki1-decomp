/* @PostProc: strip_leading_instructions --count auto */
/* Address: 0x8004ED98 */
/* Size: 44 bytes (11 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
#include "types.h"

typedef struct {
    u8 b[8];
} TableEntry;

extern TableEntry disp_string_tbl0[];
extern TableEntry get_k_work_tbl0;

TableEntry *get_k_work(s32 idx)
{
    if (idx >= 53) {
        return &get_k_work_tbl0;
    }
    return &disp_string_tbl0[idx];
}
