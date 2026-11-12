/* @PostProc: swap_instructions --offset1 0x1c --offset2 0x28 */
/* @PostProc: swap_instructions --offset1 0x1c --offset2 0x24 */
/* @PostProc: swap_instructions --offset1 0x20 --offset2 0x24 */
/* Address: 0x80137428, Size: 276 bytes (69 instructions) */
#include "types.h"

extern u8  taiiku_kari_run[];
extern u8  class_w[];
extern s16 taiiku_kari_pfrm_tbl[];

void taiiku_racer_frame_set(void)
{
    u8 *ent;
    u8 *rec;
    s32 i;
    u32 sel;
    s32 v;

    i = 0;
    ent = taiiku_kari_run;
    do {
        if (ent[37] != 0) {
            *(s16 *)(class_w + i * 68 + 6620) = (s16)(i + 136);
        } else {
            sel = *(u16 *)(ent + 48);
            if ((sel >= 6 && *(s32 *)(ent + 60) >= 53248) ||
                (sel >= 8 && *(s32 *)(ent + 60) >= 26625) ||
                sel >= 11) {
                v = ent[39];
                rec = class_w + i * 68;
                if (v >= 5) {
                    ent[39] = 0;
                    *(u16 *)(ent + 48) = 0;
                } else {
                    ent[39] = (u8)(v + 1);
                    *(u16 *)(ent + 48) = 0;
                }
                *(s16 *)(rec + 6620) = (s16)(taiiku_kari_pfrm_tbl[ent[39]] + i * 6 + 6);
            }
        }
        i++;
        ent += 36;
    } while (i != 3);
}
