/* @PostProc: swap_instructions --offset1 48 --offset2 72 */
/* @PostProc: patch_insn_reg_field 0x38:rd:s5:s4 0x7c:rt:s5:s4 0x84:rd:s2:s5 0x88:rt:s2:s5 0x90:rd:s2:s5 0x94:rt:s5:s4 0x134:rs:t3:fp 0x134:rt:fp:t3 0x150:rt:s2:s5 0x160:rt:s3:s2 0x164:rt:s4:s3 0x16c:rs:s4:s3 0x18c:rs:s4:s3 0x20c:rt:s5:s4 0x214:rd:s2:s5 0x218:rt:s2:s5 0x220:rd:s2:s5 0x224:rt:s5:s4 0x274:rt:s3:s2 0x2bc:rs:t8:fp 0x2bc:rt:fp:t8 0x2d8:rt:s2:s5 0x2f4:rd:v0:v1 0x2f8:rs:v0:v1 0x300:rt:v0:v1 0x304:rd:v1:v0 0x304:rt:v0:v1 0x308:rs:v1:v0 0x30c:rt:v1:v0 0x310:rs:v1:v0 0x31c:rd:v1:v0 0x34c:rd:v1:v0 0x358:rt:v1:v0 0x364:rt:s5:s4 0x36c:rd:s2:s1 0x370:rt:s2:s1 0x378:rd:s2:s1 0x37c:rt:s5:s4 0x3cc:rt:s3:s2 0x418:rs:t6:fp 0x418:rt:fp:t6 0x434:rt:s2:s1 0x444:rs:s3:s2 0x444:rt:s3:s2 0x44c:rs:s3:s2 0x450:rs:s4:s3 0x450:rt:s4:s3 0x4cc:rt:s5:s4 0x4d4:rd:s2:s5 0x4d8:rt:s2:s5 0x4e0:rd:s2:s5 0x4e4:rt:s5:s4 0x534:rt:s3:s2 0x57c:rs:t2:fp 0x57c:rt:fp:t2 0x598:rt:s2:s5 */
/* Address: 0x80139F88 */
/* Size: 1480 bytes (370 instructions) */
#include "types.h"

extern u8 taco_wheel_chr_tbl[];
extern s32 taco_wheel_no;
extern s32 taco_wheel_ofs;

extern s32 rsin(s32);
extern s32 rcos(s32);
extern void taco_glyph_disp(s32, s32, s32, s32, s32, s32, s32, s32, s32);

void
taco_wheel_disp(s32 arg0, s32 arg1, s32 scale, s32 arg3)
{
    s32 ang;
    s32 n;
    s32 v;
    s32 sy;
    s32 c;
    s32 bright;
    s32 i;
    s32 acc;
    s32 forty;
    s32 three;
    s32 t;
    u8 *tbl;

    ang = ((-taco_wheel_ofs) << 10) / 512;
    sy = (rsin(ang) * scale) >> 12;
    c = (rcos(ang) * scale) >> 12;
    if (c < -256) {
        bright = 0;
    } else {
        bright = ((c << 12) + 0x100000) / 96;
    }
    forty = 40;
    tbl = taco_wheel_chr_tbl;
    taco_glyph_disp(tbl[taco_wheel_no % forty], 2, 5,
                  ((c << 5) + 10240) / 160, sy, (c >> 2) + 50,
                  bright, bright, 0);

    i = 1;
    acc = 4096;
    three = 3;
    for (; i != 20; i++, acc += 4096) {
        n = acc / forty;
        v = n - taco_wheel_ofs;
        if (v < 512) {
            ang = (v << 10) / 512;
        } else {
            ang = ((v << 1) + 2048) / three;
        }
        sy = (rsin(ang) * scale) >> 12;
        c = (rcos(ang) * scale) >> 12;
        if (c < -256) {
            bright = 0;
        } else {
            bright = ((c << 12) + 0x100000) / 96;
        }
        taco_glyph_disp(tbl[(taco_wheel_no + i) % forty], 0, 5,
                      ((c << 5) + 10240) / 160, sy, (c >> 2) + 50,
                      bright, bright, 0);

        v = n + taco_wheel_ofs;
        if (v < 512) {
            t = (v << 10) / 512;
        } else {
            t = ((v << 1) + 2048) / three;
        }
        ang = 4096 - t;
        sy = (rsin(ang) * scale) >> 12;
        c = (rcos(ang) * scale) >> 12;
        if (c < -256) {
            bright = 0;
        } else {
            bright = ((c << 12) + 0x100000) / 96;
        }
        taco_glyph_disp(tbl[(taco_wheel_no - i + 40) % forty], 0, 5,
                      ((c << 5) + 10240) / 160, sy, (c >> 2) + 50,
                      bright, bright, 0);
    }

    v = 2048 - taco_wheel_ofs;
    if (v < 512) {
        ang = (v << 10) / 512;
    } else {
        ang = ((v << 1) + 2048) / three;
    }
    sy = (rsin(ang) * scale) >> 12;
    c = (rcos(ang) * scale) >> 12;
    if (c < -256) {
        bright = 0;
    } else {
        bright = ((c << 12) + 0x100000) / 96;
    }
    taco_glyph_disp(tbl[(taco_wheel_no + i) % forty], 0, 5,
                  ((c << 5) + 10240) / 160, sy, (c >> 2) + 50,
                  bright, bright, 0);
}
