/* Address: 0x8005931C */
/* Size: 172 bytes (43 instructions) */
/* Region: Game (0x80041000-0x80086000) */
/* Compiler: IDO 5.2 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: remap_stack_slots --map '40:44' */
#include "types.h"

extern u8 object[];
extern u8 coord_buf[];

extern void GsMapModelingData(s32 *hdr);
extern void GsLinkObject4(s32 *hdr, u8 *slot, u32 idx);

s32 model_slot_set(s32 *arg0, s32 arg1, u32 arg2)
{
    s32 *p;
    s32 lim;
    u8 *rec;

    p = arg0 + 1;
    GsMapModelingData(p);
    p = p + 1;
    lim = *p;
    if ((u32)lim >= 17) {
        lim = 15;
    }
    p = p + 1;
    if (arg2 < (u32)lim) {
        rec = &object[arg1 * 16];
        GsLinkObject4(p, rec, arg2);
        *(u8 **)(rec + 4) = &coord_buf[arg1 * 80];
        *(s32 *)rec = 0;
        return 1;
    }
    return -1;
}
