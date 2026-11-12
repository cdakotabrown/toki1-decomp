/* @PostProc: scheduler_normalizer */
/* @SymbolSize: 196 */
/* Address: 0x8013516C */
#include "types.h"

extern s32 D_800EE840[];
extern s32 basyo_flag0[];
extern s32 *clut_set_adr;
extern u8 cursor_work_bak[];

extern u8 rand(void);
extern void palette_upload_enqueue(s32, s32, s32, s32, s32);

s32 taco_slot_fill(void)
{
    s32 *slot;
    s32 cnt;
    s32 off;

    slot = D_800EE840;
    cnt = 0;
    off = 224;
    do {
        if (slot[1423] == 0) {
            if ((rand() & 0xFF) < 127 && cnt == 0) {
                slot[1423] = *(s32 *)((s32)clut_set_adr + off);
                goto next;
            }
            cnt++;
        }
    next:
        slot++;
        off += 4;
    } while (slot != basyo_flag0);

    palette_upload_enqueue((s32)cursor_work_bak, 31, 1, 1, 0);
    return cnt;
}
