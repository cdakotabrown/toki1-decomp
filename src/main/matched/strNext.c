/* Function: strNext */
/* Address: 0x80056C28 */
/* Size: 496 bytes (124 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: patch_stack_complete --old 72 --new 88 */
/* @PostProc: remap_stack_slots --map '56:68,58:70,60:72,62:74,64:80,68:84' */
/* @PostProc: swap_registers v1 a0 --only 0xdc,0xe4,0x158,0x168,0x178,0x17c,0x180,0x184,0x190,0x198,0x19c */
/* @PostProc: swap_registers v0 v1 --only 0x168,0x178,0x17c,0x180,0x184 */
/* @PostProc: swap_registers t5 t0 --only 0x174,0x1a0,0x1a8 */
/* @PostProc: swap_registers t6 t7 --only 0x1a4,0x1b8,0x1c0 */
/* @PostProc: swap_registers t8 t9 --only 0x1c0,0x1c8 */
/* @PostProc: swap_branch_operands --offsets 0xe4 */
#include "types.h"

extern s32 StGetNext(u8 **, u8 **);
extern void StFreeRing(u8 *);
extern void ClearImage(s16 *, s32, s32, s32);

extern u32 movie_end_frame;
extern u32 movie_main_tbl0;
extern u32 movie_main_tbl1;
extern u8 *movie_state;

u8 *strNext(s16 *arg0) {
    u8 *sp54;
    u8 *sp50;
    s16 rect[4];
    s32 var_s0;
    s32 resW;
    s32 v1;

    var_s0 = 0x800000;
    while (StGetNext(&sp54, &sp50) != 0) {
        var_s0--;
        if (var_s0 == 0) {
            return (u8 *)0;
        }
    }

    if (*(u32 *)(sp50 + 8) >= movie_end_frame) {
        *(s32 *)(movie_state + 52) = 1;
    }

    if (*(s32 *)(sp50 + 20) != *(s32 *)(sp54 + 0) ||
        *(s32 *)(sp50 + 24) != *(s32 *)(sp54 + 4)) {
        StFreeRing(sp54);
        return (u8 *)0;
    }

    movie_main_tbl0 = *(u32 *)(sp50 + 8);

    resW = *(u32 *)(movie_state + 60);
    if (*(u16 *)(sp50 + 16) != (u32)resW ||
        *(u16 *)(sp50 + 18) != *(u32 *)(movie_state + 64)) {
        rect[0] = 0;
        rect[1] = 0;
        rect[2] = 640;
        rect[3] = 480;
        ClearImage(rect, 0, 0, 0);

        *(u32 *)(movie_state + 60) = *(u16 *)(sp50 + 16);
        *(u32 *)(movie_state + 64) = *(u16 *)(sp50 + 18);

        resW = *(u32 *)(movie_state + 60);
    }

    if (movie_main_tbl1 & 1) {
        v1 = (resW * 3) / 2;
        arg0[14] = (s16)v1;
        arg0[10] = (s16)v1;
    } else {
        arg0[14] = (s16)resW;
        arg0[10] = (s16)resW;
    }

    v1 = *(s16 *)(movie_state + 64);
    arg0[15] = (s16)v1;
    arg0[11] = (s16)v1;

    arg0[21] = (s16)*(s32 *)(movie_state + 64);

    return sp54;
}
