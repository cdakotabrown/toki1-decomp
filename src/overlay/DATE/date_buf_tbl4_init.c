/* @SymbolSize: 52 */
/* Address: 0x801324a4 */
/* Size: 52 bytes (13 insns) */
#include "types.h"

extern s32 date_scene_load_ptr[];

void date_buf_tbl4_init(void) {
    date_scene_load_ptr[0] = 0x801B0090;
    date_scene_load_ptr[1] = 0x801B00A4;
    date_scene_load_ptr[2] = 0x801B0170;
}
