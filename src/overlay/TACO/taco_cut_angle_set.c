/* @CompileEnv: IDO52_UGEN_NO_ARG_COALESCE=occ:1 */
/* @SymbolSize: 224 */
/* taco_cut_angle_set - TACO overlay, Address: 0x8014D1D0, Size: 232 bytes (58 insns) */
#include "types.h"

extern s32 taco_cut_ang_tbl[5];

extern void angle_set();

void taco_cut_angle_set(void)
{
    s32 i;
    s32 j;
    s32 *ptr;

    ptr = &taco_cut_ang_tbl[0];
    i = 0;
    do {
        j = i + 1;
        angle_set(j, *ptr);
        i = j;
        ptr++;
    } while (j < 5);

    ptr = &(&taco_cut_ang_tbl[5])[0];
    i = 0;
    do {
        angle_set(i + 25, *ptr);
        i++;
        ptr++;
    } while (i < 3);

    ptr = &(&taco_cut_ang_tbl[8])[0];
    i = 0;
    do {
        angle_set(i + 29, *ptr);
        i++;
        ptr++;
    } while (i < 2);

    angle_set(8, taco_cut_ang_tbl[10]);

    ptr = &(&taco_cut_ang_tbl[11])[0];
    i = 0;
    do {
        angle_set(i + 21, *ptr);
        i++;
        ptr++;
    } while (i != 2);
}
