/* @SymbolSize: 428 */
/* @PostProc: swap_instructions --offset1 0x20 --offset2 0x24 */
/* @PostProc: swap_instructions --offset1 0x24 --offset2 0x28 */
/* Address: 0x8013F74C */
/* Size: 428 bytes (107 instructions) */
#include "types.h"

extern u8 D_800EE761[];
extern u8 now_girl;
extern u8 save_buf[];

void func_8013F74C(void)
{
    s32 i;

    D_800EE761[1339] = 1;
    for (i = 0; i < 3; i++) {
        D_800EE761[1340 + i * 4] = 1;
        D_800EE761[1341 + i * 4] = 1;
        D_800EE761[1342 + i * 4] = 1;
        D_800EE761[1343 + i * 4] = 1;
    }
    D_800EE761[1350] = 5;
    now_girl = 4;
    save_buf[0] = 4;
    save_buf[2] = 2;
    save_buf[1] = 32;
    save_buf[3] = 0;
    save_buf[4] = 96;
    save_buf[5] = 2;
    save_buf[6] = 25;
    save_buf[7] = 0;
    save_buf[8] = 4;
    save_buf[10] = 0;
    save_buf[9] = 1;
    save_buf[11] = 0;
    save_buf[12] = 96;
    save_buf[13] = 2;
    save_buf[14] = 25;
    save_buf[15] = 0;
    save_buf[16] = 4;
    save_buf[18] = 2;
    save_buf[17] = 28;
    save_buf[19] = 0;
    save_buf[20] = 96;
    save_buf[21] = 2;
    save_buf[22] = 25;
    save_buf[23] = 0;
    save_buf[24] = 4;
    save_buf[26] = 0;
    save_buf[25] = 5;
    save_buf[27] = 0;
    save_buf[28] = 96;
    save_buf[29] = 2;
    save_buf[30] = 25;
    save_buf[31] = 0;
}
