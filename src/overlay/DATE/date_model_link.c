/* Address: 0x8014AEF8, Size: 248 bytes (62 insns) */
/* @SymbolSize: 248 */
/* @CompileEnv: IDO52_NO_LOOP_TEST_REPLACEMENT=1 */
/* @PostProc: swap_instructions --offset1 0x3c --offset2 0x44 */
/* @PostProc: swap_instructions --offset1 0x40 --offset2 0x44 */
/* @PostProc: swap_instructions --offset1 0x48 --offset2 0x54 */
/* @PostProc: swap_instructions --offset1 0x4c --offset2 0x50 */
/* @PostProc: swap_instructions --offset1 0x50 --offset2 0x54 */
/* @PostProc: swap_instructions --offset1 0x70 --offset2 0x74 */
/* @PostProc: swap_instructions --offset1 0x94 --offset2 0xa4 */
/* @PostProc: swap_instructions --offset1 0x98 --offset2 0xa4 */
/* @PostProc: swap_instructions --offset1 0x9c --offset2 0xa4 */
/* @PostProc: swap_instructions --offset1 0xa0 --offset2 0xa4 */
/* @PostProc: swap_instructions --offset1 0xa8 --offset2 0xb4 */
/* @PostProc: swap_instructions --offset1 0xac --offset2 0xb8 */
/* @PostProc: swap_instructions --offset1 0xb4 --offset2 0xb8 */
/* @PostProc: patch_insn_reg_field 0x44:rt:s1:s2 0x4c:rs:s1:s2 0x4c:rt:s1:s2 0x54:rd:s2:s1 0x58:rt:s3:s5 0x68:rs:s2:s1 0x6c:rs:s2:s1 0x6c:rt:s2:s1 0x70:rs:s2:s1 0x74:rt:s1:s2 0x78:rs:s1:s2 0x78:rt:s1:s2 0x84:rt:s3:s5 0x9c:rt:s2:s3 0xa0:rt:s1:s2 0xa4:rt:s5:s1 0xac:rs:s5:s1 0xac:rt:s5:s1 0xb0:rs:s1:s2 0xb0:rt:s1:s2 0xb4:rs:s2:s3 0xb4:rt:s2:s3 0xc0:rs:s2:s3 0xcc:rt:s1:s2 0xd0:rs:s1:s2 0xd0:rt:s1:s2 0xd8:rs:s2:s3 0xd8:rt:s2:s3 0xdc:rs:s1:s2 0xdc:rt:s5:s1 0xe0:rt:s3:s5 */
#include "types.h"

extern u8 object[];
extern u8 object_2[];
extern u8 coord_buf[];
extern u8 coord_buf_2[];
extern u8 coord_buf_11[];

extern void GsLinkObject4(u32 a0, u32 a1, s32 a2);
extern void GsMapModelingData(u32 a0);

void date_model_link(void)
{
    u8 *node;
    s32 i;
    u8 *payload;
    u8 *node2;
    u32 attr;

    GsMapModelingData(0x801F8004U);

    node = object;
    i = 0;
    payload = coord_buf;
    attr = 0x80000000U;

    do {
        GsLinkObject4(0x801F800CU, (u32)node, i);
        i++;
        *(u32 *)(node + 4) = (u32)payload;
        payload += 80;
        node += 16;
        *(u32 *)(node - 16) = attr;
    } while (i < 2);

    GsMapModelingData(0x801F9004U);

    node = object_2 - 32;
    node2 = object_2;
    payload = coord_buf_2;

    do {
        GsLinkObject4(0x801F900CU, (u32)node2, 0);
        *(u32 *)(node + 36) = (u32)payload;
        payload += 80;
        node += 16;
        node2 += 16;
        *(u32 *)(node + 16) = attr;
    } while (payload != coord_buf_11);
}
