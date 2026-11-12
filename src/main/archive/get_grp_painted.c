/* Function: get_grp */
/* Address: 0x800537F0 */
/* Size: 276 bytes (69 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* Compiler: IDO 5.2 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: custom_script custom_func_800537F0.py */
#include "types.h"

extern u8 get_grp_tbl0[];

extern void get_h_yuukou_table(u8 *dest);
extern void get_h_tokimeki_table(u8 *dest);
extern void get_grp_point(u8 *arg0, s16 *arg1);
extern void func_80053364(u8 *arg0, s16 *arg1);

void get_grp(u8 *arg0, s16 *arg1) {
    u8 arr2[16];
    u8 arr1[16];
    s32 pad[3];
    u8 *p1;
    u8 *p2;
    u8 *dst;
    s16 *out;
    u32 col;
    u32 row;
    s32 i;

    get_h_yuukou_table(arr1);
    get_h_tokimeki_table(arr2);

    out = arg1;
    dst = arg0;
    col = 0;
    i = 0;
    p1 = arr1;
    p2 = arr2;

    do {
        s32 offset;

        *out = 0;
        *p1 = (u8)*p1 / 5u;
        i += 2;
        out++;
        col = (u8)*p1;
        if (col >= 20) {
            col = (u8)(*p1 = 19);
        }
        *p2 = (u8)*p2 / 5u;
        p1++;
        row = (u8)*p2;
        if (row >= 20) {
            row = (u8)(*p2 = 19);
        }
        offset = (row << 2) + row;
        offset <<= 2;
        *dst = get_grp_tbl0[offset + col];
        p2++;
        dst++;
    } while (i != 32);

    get_grp_point(arg0, arg1);
    func_80053364(arg0, arg1);
}
