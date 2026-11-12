/* @PostProc: patch_insn_reg_field 0x28:rt:v1:v0 0x30:rs:v1:v0 0x134:rs:v1:v0 */
/* Address: 0x80140BC8, Size: 516 bytes (129 instructions) */
#include "types.h"

extern u8 option_on;
extern u8 mascot_flag2;
extern u8 mascot_flag2_1;
extern u8 mascot_flag2_2;
extern u8 mascot_flag2_3;
extern u8 mascot_flag2_4;
extern u8 mascot_flag2_5;
extern u8 mascot_flag2_6;
extern u8 mascot_flag2_7;
extern u8 mascot_flag2_8;
extern u8 mascot_flag2_9;
extern u8 mascot_flag2_10;
extern u8 mascot_flag2_11;
extern u8 mascot_flag2_12;

s32 etc_flags_judge(void)
{
    u8 first;

    if (option_on == 0) {
        first = mascot_flag2;
        if (first >= 2U && mascot_flag2_1 >= 2U && mascot_flag2_2 >= 2U &&
            mascot_flag2_3 >= 2U && mascot_flag2_4 >= 2U && mascot_flag2_5 >= 2U &&
            mascot_flag2_6 >= 2U && mascot_flag2_7 >= 2U && mascot_flag2_8 >= 2U &&
            mascot_flag2_9 >= 2U && mascot_flag2_10 >= 2U && mascot_flag2_11 >= 2U &&
            mascot_flag2_12 >= 2U) {
            return 2;
        }
        if (first != 0 && mascot_flag2_1 != 0 && mascot_flag2_2 != 0 &&
            mascot_flag2_3 != 0 && mascot_flag2_4 != 0 && mascot_flag2_5 != 0 &&
            mascot_flag2_6 != 0 && mascot_flag2_7 != 0 && mascot_flag2_8 != 0 &&
            mascot_flag2_9 != 0 && mascot_flag2_10 != 0 && mascot_flag2_11 != 0 &&
            mascot_flag2_12 != 0) {
            return 1;
        }
        return 0;
    }
    return 0;
}
