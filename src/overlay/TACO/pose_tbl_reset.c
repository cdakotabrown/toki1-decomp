/* @SymbolSize: 220 */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* @PostProc: scheduler_normalizer */
/* @PostProc: swap_instructions --offset1 0x24 --offset2 0x48 */
/* @PostProc: patch_insn_reg_field 0x20:rd:s3:s1 0x24:rs:s1:s2 0x24:rt:s1:s2 0x34:rs:s1:s2 0x44:rt:s1:s3 0x48:rs:s2:s3 0x48:rt:s2:s3 0x50:rs:s1:s2 0x60:rt:s2:s3 0x6c:rt:s3:s1 0x78:rt:v0:v1 0x84:rs:v0:v1 0x84:rt:v0:v1 0x88:rs:v0:v1 0x8c:rs:v0:v1 0x8c:rt:v1:v0 0x90:rs:v0:v1 0x94:rs:v0:v1 0x94:rt:v1:v0 0x98:rs:v0:v1 0x98:rt:v1:v0 0x9c:rs:v0:v1 0x9c:rt:v1:v0 0xa0:rs:v0:v1 0xa4:rs:v0:v1 0xa4:rt:v1:v0 0xa8:rs:v0:v1 0xa8:rt:v1:v0 0xac:rs:v0:v1 0xac:rt:v1:v0 0xb0:rs:v0:v1 0xb4:rs:v0:v1 0xb4:rt:v1:v0 0xb8:rs:v0:v1 0xb8:rt:v1:v0 0xbc:rs:v0:v1 0xbc:rt:v1:v0 0xc0:rs:v0:v1 0xc0:rt:v0:v1 0xc4:rs:v0:v1 0xc4:rt:v1:v0 0xc8:rs:v0:v1 0xcc:rs:v0:v1 0xcc:rt:v1:v0 */
/* Address: 0x80146D10 */
/* Size: 220 bytes (55 insns) */
#include "types.h"

extern u8 taco_coord[];
extern u8 taco_coord_1_flg[];
extern u8 taco_angle_tbl[];
extern u8 taco_angle_tbl_end[];

extern void GsInitCoordinate2(s32 a0, s32 a1);

void pose_tbl_reset(s32 arg0)
{
    u8 *rec;
    volatile s16 *p;
    s16 t;

    GsInitCoordinate2(0, (s32)taco_coord);

    rec = taco_coord_1_flg;
    do {
        GsInitCoordinate2((s32)taco_coord, (s32)rec);
        *(s32 *)(rec + 28) = 0;
        *(s32 *)(rec + 24) = 0;
        *(s32 *)(rec + 32) = arg0;
        rec += 80;
        *(s32 *)(rec - 80) = 0;
    } while ((u32)rec < (u32)taco_angle_tbl);

    p = (volatile s16 *)taco_angle_tbl;
    do {
        p[2] = 0;
        t = p[2];
        p[6] = 0;
        p[1] = t;
        p[0] = t;
        t = p[6];
        p[10] = 0;
        p[5] = t;
        p[4] = t;
        t = p[10];
        p[14] = 0;
        p[9] = t;
        p[8] = t;
        t = p[14];
        p += 16;
        p[-3] = t;
        p[-4] = t;
    } while (p != (volatile s16 *)taco_angle_tbl_end);
}
