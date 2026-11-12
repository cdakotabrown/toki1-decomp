/* @SymbolStart: 0x801353F0 */
/* @RodataBase: 0x801394C4 */
/* @SymbolSize: 620 */
/* @PostProc: patch_stack_complete --old 48 --new 56 */
/* @PostProc: remap_stack_slots --map '28:40,32:44,36:48,40:52' */
/* en_nichi_dir_get - EN_NICHI overlay, Address: 0x801353F0, Size: 620 bytes (155 insns) */
#include "types.h"

extern s32 en_nichi_obj_octant_get(s32);
extern u8 en_nichi_obj_wk[];
extern s16 cursor_cw_x;
extern s16 cursor_cw_y;

s32 en_nichi_dir_get(s32 arg0)
{
    u8 *e;
    s32 dx;
    s32 dy;
    s32 ret;

    e = &en_nichi_obj_wk[arg0 * 48];
    dx = cursor_cw_x - *(s16 *)(e + 22);
    dy = cursor_cw_y - *(s16 *)(e + 26);

    switch ((u32)en_nichi_obj_octant_get(arg0)) {
    case 0:
        if (dy < dx) {
            ret = 0;
        } else {
            ret = 1;
        }
        if (dy <= -dx) {
            ret += 2;
        }
        break;
    case 1:
        if (cursor_cw_x > *(s16 *)(e + 22)) {
            ret = 0;
        } else {
            ret = 1;
        }
        if (*(s16 *)(e + 26) >= cursor_cw_y) {
            ret += 2;
        }
        break;
    case 2:
        if (-dx < dy) {
            ret = 0;
        } else {
            ret = 1;
        }
        if (dx >= dy) {
            ret += 2;
        }
        break;
    case 3:
        if (cursor_cw_y > *(s16 *)(e + 26)) {
            ret = 0;
        } else {
            ret = 1;
        }
        if (cursor_cw_x >= *(s16 *)(e + 22)) {
            ret += 2;
        }
        break;
    case 4:
        if (dx < dy) {
            ret = 0;
        } else {
            ret = 1;
        }
        if (dy >= -dx) {
            ret += 2;
        }
        break;
    case 5:
        if (cursor_cw_x < *(s16 *)(e + 22)) {
            ret = 0;
        } else {
            ret = 1;
        }
        if (cursor_cw_y >= *(s16 *)(e + 26)) {
            ret += 2;
        }
        break;
    case 6:
        if (dy < -dx) {
            ret = 0;
        } else {
            ret = 1;
        }
        if (dy >= dx) {
            ret += 2;
        }
        break;
    case 7:
        if (cursor_cw_y < *(s16 *)(e + 26)) {
            ret = 0;
        } else {
            ret = 1;
        }
        if (*(s16 *)(e + 22) >= cursor_cw_x) {
            ret += 2;
        }
        break;
    }
    return ret;
}
