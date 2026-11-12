/* Function: MoveImage (likely: StoreImage) */
/* Address:  0x800A0180 */
/* Size:     144 bytes (36 instructions) */
/* Region:   Library (0x80086000+) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: custom_script custom_func_800A0180.py */
#include "types.h"

extern s32 libgpu_moveimage_name_str;
extern s32 libgpu_moveimage_pkt_src_xy;
extern s32 libgpu_moveimage_pkt_dst;
extern s32 D_800E3B84;
extern s32 libgpu_moveimage_pkt_head;
extern s32 *graph_jtb;

extern void checkRECT(s32*, s32*);

void MoveImage(s32 *arg0, s32 arg1, s32 arg2) {
    void (*callback)();

    checkRECT(&libgpu_moveimage_name_str, arg0);

    libgpu_moveimage_pkt_src_xy = arg0[0];
    libgpu_moveimage_pkt_dst = (arg2 << 16) | (arg1 & 0xFFFF);
    D_800E3B84 = arg0[1];

    callback = (void (*)())*(s32*)((s32)graph_jtb + 8);
    callback(*(s32*)((s32)graph_jtb + 24), &libgpu_moveimage_pkt_head, 20, 0);
}
