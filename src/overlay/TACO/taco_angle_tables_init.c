/* @PostProc: scheduler_normalizer */
/* Address: 0x8014C2B4 */
/* Size: 208 bytes (52 insns) */
#include "types.h"

extern s32 taco_line_ang_tbl[];
extern s32 taco_line_ang_tbl_5[];
extern s32 taco_line_ang_tbl_14[];
extern s32 taco_line_ang_tbl_24[];

extern void angle_set(s32, s32);

void taco_angle_tables_init(void)
{
    s32 i;

    i = 0;
    do {
        angle_set(i + 1, taco_line_ang_tbl[i]);
        i++;
    } while (i < 5);
    i = 0;
    do {
        angle_set(i + 19, taco_line_ang_tbl_5[i]);
        i++;
    } while (i < 9);
    i = 0;
    do {
        angle_set(i + 7, taco_line_ang_tbl_14[i]);
        i++;
    } while (i < 10);
    i = 0;
    do {
        angle_set(i + 29, taco_line_ang_tbl_24[i]);
        i++;
    } while (i != 2);
}
