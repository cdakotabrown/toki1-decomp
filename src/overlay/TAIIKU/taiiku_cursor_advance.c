/* Address: 0x8013FB20, Size: 460 bytes (115 instructions) */
#include "types.h"

extern u8  taiiku_tama_ctick;
extern s32 taiiku_tama_fall;
extern u8  taiiku_tama_cpu_stop[];
extern u8  class_w[];

void taiiku_cursor_advance(void)
{
    s32 i;

    if ((u32)taiiku_tama_ctick < 31) {
        return;
    }
    taiiku_tama_ctick = 0;
    for (i = 0; i != 4; i++) {
        if (i == 0) {
            if (taiiku_tama_fall == 0) {
                if (*(s16 *)(class_w + 7096) >= 144) {
                    *(s16 *)(class_w + 7096) =
                        (s16)(*(s16 *)(class_w + 7096) ^ 1);
                } else if (*(s16 *)(class_w + 7096) == 141) {
                    *(s16 *)(class_w + 7096) = 138;
                } else {
                    *(s16 *)(class_w + 7096) =
                        (s16)(*(s16 *)(class_w + 7096) + 1);
                }
            }
        } else {
            if (taiiku_tama_cpu_stop[i - 1] == 0) {
                if (*(s16 *)(class_w + i * 68 + 7096) >= 142) {
                    *(s16 *)(class_w + i * 68 + 7096) =
                        (s16)(*(s16 *)(class_w + i * 68 + 7096) ^ 1);
                } else if (*(s16 *)(class_w + i * 68 + 7096) == 137) {
                    *(s16 *)(class_w + i * 68 + 7096) = 134;
                } else {
                    *(s16 *)(class_w + i * 68 + 7096) =
                        (s16)(*(s16 *)(class_w + i * 68 + 7096) + 1);
                }
            }
        }
    }
}
