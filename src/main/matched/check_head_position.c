/* Function: check_head_position */
/* Address: 0x80046388 */
/* Size: 444 bytes (111 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: patch_stack_complete --old 112 --new 104 */
/* @PostProc: patch_insn_reg_field --patch 0xa8:rt:v0:v1 --patch 0xac:rt:v0:v1 --patch 0xac:rs:v0:v1 --patch 0xb8:rs:v0:v1 --patch 0xc0:rs:v0:v1 --patch 0xd0:rt:t8:v1 --patch 0xd8:rt:t8:v1 --patch 0x108:rt:t9:t8 --patch 0x10c:rt:t0:t9 --patch 0x110:rt:t1:t0 --patch 0x118:rt:t9:t8 --patch 0x11c:rt:t0:t9 --patch 0x124:rt:t1:t0 */
#include "types.h"

extern s32 CdSync(s32 a0, void *a1);
extern s32 CdControlB(s32 a0, s32 a1, void *a2);
extern s32 get_CD_l_com(void);
extern void printf(void *a0);
extern void cd_mode_set(void);
extern s32 CdPosToInt(u8 *a0);
extern s32 nibble_decimal_convert(u8 a0);

typedef struct {
    u8 byte0;
    u8 byte1;
    u8 byte2;
    u8 byte3;
} DataStruct;

extern DataStruct cdq_stat_buf;
extern u8 cdq_head;
extern s8 cd_head_pos;
extern char check_head_position_str0[];
extern char check_head_position_str1[];

s32 check_head_position(void) {
    s32 temp_s5;
    s32 s0;
    u8 sp68[36];
    s32 pad;
    DataStruct *s1;
    s32 result;

    (void)&pad;
    s1 = &cdq_stat_buf;
    temp_s5 = *(s32 *)s1;
    s0 = 0;
    s1->byte0 = s1->byte1 = s1->byte2 = s1->byte3 = 0;

    if (CdSync(1, sp68) != 0) {
        if (CdControlB(16, 0, s1) == 0) {
            do {
                s0++;
                if (s0 >= 16) {
                    if (get_CD_l_com() == 27) {
                        printf(check_head_position_str0);
                        cd_head_pos = -1;
                        if (cdq_head != 0) {
                            cdq_head = cdq_head - 1;
                        } else {
                            cdq_head = 47;
                        }
                        cd_mode_set();
                    }
                    printf(check_head_position_str1);
                    *(s32 *)s1 = temp_s5;
                }
            } while (CdControlB(16, 0, s1) == 0);
        }
    }

    sp68[16] = s1->byte0;
    sp68[17] = s1->byte1;
    sp68[18] = s1->byte2;
    result = CdPosToInt(&sp68[16]);

    if (nibble_decimal_convert(s1->byte0) >= 75) {
        return 0;
    }
    if (nibble_decimal_convert(s1->byte1) >= 60) {
        return 0;
    }
    if (nibble_decimal_convert(s1->byte2) >= 75) {
        return 0;
    }
    if (result > 0) {
        return result;
    }
    return 0;
}
