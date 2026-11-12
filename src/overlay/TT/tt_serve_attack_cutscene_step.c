/* tt_serve_attack_cutscene_step - TT overlay, Address: 0x80138E28, Size: 1044 bytes (261 insns) */
/* @SymbolSize: 1044 */
/* @PostProc: swap_instructions --offset1 0x78 --offset2 0x7c */
/* @PostProc: swap_instructions --offset1 0x74 --offset2 0x78 */
/* @PostProc: swap_instructions --offset1 0x70 --offset2 0x74 */
/* @PostProc: swap_instructions --offset1 0x84 --offset2 0x88 */
/* @PostProc: swap_instructions --offset1 0x80 --offset2 0x84 */
/* @PostProc: swap_instructions --offset1 0x208 --offset2 0x20c */
/* @PostProc: swap_instructions --offset1 0x318 --offset2 0x31c */
/* @PostProc: patch_insn_reg_field 0x64:rt:t0:a2 0x70:rs:t0:a2 0x70:rt:t0:a2 0x80:rt:a1:t0 0x88:rt:a2:a1 0x8c:rt:t0:a2 0xb4:rt:a2:a1 0xbc:rt:a1:t0 0x114:rt:s3:s2 0x120:rt:v0:v1 0x128:rt:v0:v1 0x12c:rt:v0:v1 0x134:rt:v0:v1 0x138:rt:s3:s2 0x148:rt:v0:v1 0x158:rd:v1:a0 0x15c:rs:v1:a0 0x178:rs:v1:a0 0x194:rs:v1:a0 0x1ac:rt:s3:s2 0x1f0:rt:s3:s2 0x200:rt:s2:s3 0x204:rt:a2:a1 0x208:rt:v1:a0 0x20c:rt:a1:v1 0x218:rt:s3:s2 0x230:rt:s3:s2 0x25c:rt:s3:s2 0x274:rt:s3:s2 0x288:rt:s2:s3 0x28c:rt:s2:s3 0x290:rt:a1:v1 0x294:rt:v1:a0 0x29c:rt:a2:a1 0x2d0:rt:a2:a1 0x2d8:rt:v1:a0 0x2dc:rt:a1:v1 0x2e0:rt:s2:s3 0x2e4:rt:s2:s3 0x2f0:rt:s2:s3 0x308:rt:s2:s3 0x314:rt:s3:s1 0x318:rs:s3:s1 0x318:rt:s3:s1 0x31c:rd:s1:s2 0x32c:rs:s1:s2 0x330:rt:t5:t4 0x334:rt:t4:t7 0x338:rs:t5:t4 0x33c:rs:s1:s2 0x340:rd:t6:t5 0x344:rd:t7:t6 0x344:rt:t6:t5 0x348:rs:t4:t6 0x358:rt:t8:t2 0x35c:rt:t2:t4 0x360:rs:t8:t2 0x364:rs:s1:s2 0x364:rt:s1:s2 0x36c:rd:t3:t8 0x370:rd:t5:t3 0x370:rt:t3:t8 0x374:rd:t6:t5 0x374:rs:t2:t3 0x374:rt:t5:t4 0x378:rs:t6:t5 0x378:rt:t4:t6 0x37c:rt:t4:t6 0x390:rd:t8:t2 0x390:rs:s3:s1 0x394:rs:t8:t2 0x394:rt:t3:t8 0x398:rs:s1:s2 0x39c:rt:t3:t8 0x3a0:rt:t2:t3 0x3a8:rs:t2:t3 0x3a8:rt:t5:t4 0x3ac:rt:t5:t4 0x3dc:rt:s2:s3 0x3e4:rt:t6:t5 0x3ec:rt:t6:t5 0x3f4:rt:t4:t6 0x3f8:rt:t4:t6 */
#include "types.h"

extern u8 tt_puff_seq_tbl[];
extern u8 tt_anim_tbl[];
extern s16 tt_main51_scr[];
extern u8 tt_main51_pose_tbl[];

extern u8 *tt_boss_wk;
extern u8 *tt_anim_buf;
extern u8 *tt_scale_buf;

extern s32 rsin(s32 angle);
extern s32 rcos(s32 angle);
extern void tt_cmd_once(s32 id);
extern void tt_unit_init(void);
extern void tt_seq_step(u8 *tbl, s32 idx);

void tt_serve_attack_cutscene_step(void)
{
    u8 *s;
    u8 *cam;
    u8 *r;
    u8 *p;
    u8 *b;
    u8 *h;
    s32 i;
    u32 n;
    s32 id;
    s32 ang;
    s32 v;

    s = tt_boss_wk;
    cam = tt_scale_buf;
    switch ((u32)*(u16 *)(s + 6)) {
    case 0:
        i = 16;
        *(s16 *)(s + 868) = 0;
        *(s16 *)(s + 980) = 0;
        p = s + 320;
        b = s + 16;
        h = s + 32;
        do {
            *(s32 *)(p + 56) = (s32)tt_anim_tbl;
            p[60] = 0;
            p[61] = 4;
            p[66] = 0;
            p[72] = 0;
            b[784] = 0;
            i += 1;
            p += 20;
            b += 1;
            h += 2;
            *(s16 *)(h + 678) = 8;
            *(s16 *)(h + 582) = 0;
            *(s16 *)(h + 630) = 0;
        } while (i != 18);
        *(s16 *)(s + 6) = 64;
        break;
    case 64:
        if (tt_main51_scr[*(u16 *)(tt_boss_wk + 980)] != -1) {
            if (tt_main51_scr[*(u16 *)(tt_boss_wk + 980)] ==
                    *(u16 *)(tt_boss_wk + 868)) {
                tt_cmd_once(1281);
                id = tt_main51_scr[*(u16 *)(s + 980) + 4];
                s[id + 784] = 1;
                s[id * 20 + 64] = 128;
                *(s16 *)(s + id * 2 + 584) =
                    tt_main51_scr[*(u16 *)(s + 980) + 1];
                *(s16 *)(s + id * 2 + 632) =
                    tt_main51_scr[*(u16 *)(s + 980) + 2];
                *(s16 *)(s + id * 2 + 680) =
                    tt_main51_scr[*(u16 *)(s + 980) + 3];
                *(s16 *)(s + 980) = (s16)(*(u16 *)(s + 980) + 5);
            }
        }
        for (i = 16; i < 18; i++) {
            tt_seq_step(tt_puff_seq_tbl, i);
        }
        n = *(u16 *)(s + 868);
        if (n == 75) {
            s[784] = 0;
            tt_unit_init();
            s[82] = 1;
            r = tt_anim_buf + 6400;
            for (i = 0; i < 16; i++) {
                r[0] = 1;
                r[1] = 0;
                *(s16 *)(r + 68) = 0;
                r[59] = 128;
                *(s32 *)(r + 72) = (s32)cam;
                r[77] = 0;
                r[78] = 1;
                r[80] = 0;
                *(s16 *)(r + 20) = (s16)(*(s16 *)(s + 20) - 8);
                *(s16 *)(r + 22) = (s16)(*(s16 *)(s + 22) - 8);
                r[71] = (u8)(((i & 1) << 4) + 64);
                r[70] = 240;
                r[79] = 0;
                *(s16 *)(r + 60) = 16;
                *(s16 *)(r + 62) = 16;
                *(s16 *)(r + 64) = 18;
                *(s16 *)(r + 66) = 15570;
                r[76] = 0;
                *(s16 *)(r + 16) = 8;
                r += 100;
            }
            *(s16 *)(s + 980) = 0;
            n = *(u16 *)(s + 868);
        }
        if (n >= 76) {
            r = tt_anim_buf + 6400;
            ang = 0;
            do {
                v = rcos(ang);
                *(s16 *)(r + 20) = (s16)(
                    (s32)(((u32)*(u16 *)(s + 980) * (u32)v) >> 12) +
                    *(s16 *)(s + 20) - 8);
                v = rsin(ang);
                ang += 256;
                r += 100;
                *(s16 *)(r - 78) = (s16)(
                    (s32)(((u32)*(u16 *)(s + 980) * (u32)v) >> 12) +
                    *(s16 *)(s + 22) - 8);
                r[-24] = tt_main51_pose_tbl[((u32)*(u16 *)(s + 868) >> 2) & 3];
            } while (ang != 4096);
            *(s16 *)(s + 980) = (s16)(*(u16 *)(s + 980) + 4);
            n = *(u16 *)(s + 868);
        }
        if (n >= 136) {
            r = tt_anim_buf + 6400;
            for (i = 0; i < 16; i++) {
                r[0] = 0;
                r += 100;
            }
            *(s16 *)(s + 4) = 82;
            *(s16 *)(s + 6) = 0;
            n = *(u16 *)(s + 868);
        }
        *(s16 *)(s + 868) = (s16)(n + 1);
        break;
    }
}
