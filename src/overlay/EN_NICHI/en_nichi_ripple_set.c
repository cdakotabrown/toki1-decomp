/* @PostProc: remap_stack_slots --map '68:84' */
/* @PostProc: patch_stack_complete --old 136 --new 152 */
/* @PostProc: swap_instructions --offset1 0x14 --offset2 0x30 */
/* @PostProc: swap_instructions --offset1 0x64 --offset2 0x68 */
/* @PostProc: swap_instructions --offset1 0x8C --offset2 0x94 */
/* @PostProc: swap_instructions --offset1 0x1F8 --offset2 0x1FC */
/* @PostProc: swap_instructions --offset1 0x1FC --offset2 0x200 */
/* @PostProc: swap_instructions --offset1 0x200 --offset2 0x204 */
/* @PostProc: swap_instructions --offset1 0x204 --offset2 0x208 */
/* @PostProc: swap_instructions --offset1 0x208 --offset2 0x20C */
/* @PostProc: swap_instructions --offset1 0x20C --offset2 0x210 */
/* @PostProc: swap_instructions --offset1 0x210 --offset2 0x214 */
/* @PostProc: swap_instructions --offset1 0x214 --offset2 0x218 */
/* @PostProc: swap_instructions --offset1 0x218 --offset2 0x21C */
/* @PostProc: swap_instructions --offset1 0x21C --offset2 0x220 */
/* @PostProc: swap_instructions --offset1 0x220 --offset2 0x224 */
/* @PostProc: swap_instructions --offset1 0x224 --offset2 0x228 */
/* @PostProc: swap_instructions --offset1 0x228 --offset2 0x22C */
/* @PostProc: swap_instructions --offset1 0x2E8 --offset2 0x2EC */
/* @PostProc: swap_instructions --offset1 0x2EC --offset2 0x2F0 */
/* @PostProc: swap_instructions --offset1 0x2F0 --offset2 0x2F4 */
/* @PostProc: swap_instructions --offset1 0x2F4 --offset2 0x2F8 */
/* @PostProc: swap_instructions --offset1 0x2F8 --offset2 0x2FC */
/* @PostProc: swap_instructions --offset1 0x2FC --offset2 0x300 */
/* @PostProc: swap_instructions --offset1 0x300 --offset2 0x304 */
/* @PostProc: swap_instructions --offset1 0x304 --offset2 0x308 */
/* @PostProc: swap_instructions --offset1 0x308 --offset2 0x30C */
/* @PostProc: swap_instructions --offset1 0x30C --offset2 0x310 */
/* @PostProc: swap_instructions --offset1 0x310 --offset2 0x314 */
/* @PostProc: swap_instructions --offset1 0x314 --offset2 0x318 */
/* @PostProc: swap_instructions --offset1 0x318 --offset2 0x31C */
/* @PostProc: patch_insn_reg_field 0x40:rt:s2:s3 0x60:rs:s2:s3 0x60:rt:s2:s3 0x64:rd:s4:s2 0x68:rt:s3:s4 0x108:rs:s2:s3 0x10c:rt:s4:s2 0x114:rt:a0:t6 0x118:rt:t6:t5 0x140:rs:t6:t5 0x140:rt:a0:t6 0x14c:rt:t5:t1 0x188:rd:t1:t3 0x188:rs:t5:t1 0x190:rt:t1:t3 0x19c:rd:t3:t2 0x1a0:rs:t3:t2 0x1b4:rd:t2:t4 0x1b8:rs:t2:t4 0x1c4:rt:t5:t1 0x1c8:rd:t4:t5 0x1cc:rt:t5:t1 0x1d0:rt:s3:s4 0x1d4:rd:t4:t5 0x1d4:rs:t4:t5 0x1d8:rd:t4:t5 0x1d8:rt:t4:t5 0x1dc:rs:s2:s3 0x1dc:rt:t4:t5 0x1e0:rt:s4:s2 0x1e8:rt:a0:t7 0x1ec:rt:t7:t6 0x1f0:rt:t2:t4 0x1f4:rs:t7:t6 0x1f4:rt:a0:t7 0x1f8:rd:t1:t3 0x204:rs:t1:t3 0x220:rs:t1:t3 0x22c:rt:t6:t3 0x230:rd:t3:t2 0x234:rd:t4:t5 0x234:rs:t2:t4 0x234:rt:t3:t2 0x238:rt:t4:t5 0x23c:rt:s3:s4 0x240:rd:t5:t1 0x24c:rs:t5:t1 0x268:rs:t5:t1 0x278:rd:t1:t4 0x278:rs:t6:t3 0x280:rt:t1:t4 0x2a0:rt:t5:t1 0x2a4:rd:t3:t5 0x2a8:rs:t3:t5 0x2b4:rt:t6:t3 0x2b8:rd:t4:t6 0x2bc:rs:t5:t1 0x2bc:rt:t6:t3 0x2c0:rt:s3:s4 0x2c4:rd:t4:t6 0x2c4:rs:t4:t6 0x2c8:rd:t4:t6 0x2c8:rt:t4:t6 0x2cc:rs:s2:s3 0x2cc:rt:t4:t6 0x2d0:rt:s4:s2 0x2d8:rt:a0:t8 0x2dc:rt:t8:t7 0x2e0:rt:t3:t5 0x2e4:rd:t5:t1 0x2e4:rs:t8:t7 0x2e4:rt:a0:t8 0x2e8:rd:t1:t4 0x2f4:rs:t1:t4 0x310:rs:t1:t4 0x31c:rt:t7:t4 0x324:rd:t4:t6 0x324:rs:t3:t5 0x328:rt:t4:t6 0x32c:rs:t5:t1 0x32c:rt:s3:s4 0x330:rd:t6:t3 0x33c:rs:t6:t3 0x358:rs:t6:t3 0x368:rd:t1:t5 0x368:rs:t7:t4 0x370:rt:t1:t5 0x37c:rd:t3:t2 0x380:rs:t3:t2 0x390:rt:t6:t3 0x394:rd:t2:t6 0x398:rs:t2:t6 0x3a4:rt:t7:t4 0x3a8:rd:t4:t7 0x3ac:rs:t6:t3 0x3ac:rt:t7:t4 0x3b0:rd:t1:t5 0x3b4:rs:t1:t5 0x3b8:rd:t4:t7 0x3b8:rs:t4:t7 0x3bc:rd:t4:t7 0x3bc:rt:t4:t7 0x3c0:rs:s2:s3 0x3c0:rt:t4:t7 0x3c4:rd:t5:t1 0x3c4:rt:s4:s2 0x3c8:rt:t5:t1 0x3cc:rt:a0:t1 0x3d0:rt:t5:t8 0x3ec:rs:t1:t5 0x3f8:rd:t6:t3 0x3f8:rs:t5:t8 0x3f8:rt:a0:t1 0x3fc:rd:t7:t4 0x3fc:rt:t6:t3 0x400:rt:t2:t6 0x404:rt:t8:t5 0x408:rd:t3:t2 0x40c:rd:t4:t7 0x40c:rs:t2:t6 0x40c:rt:t3:t2 0x410:rt:t4:t7 0x414:rs:t7:t4 0x430:rs:t7:t4 0x440:rd:t1:t6 0x440:rs:t8:t5 0x444:rt:t1:t6 0x458:rs:s4:s2 0x458:rt:s4:s2 0x460:rs:s4:s2 */
/* en_nichi_ripple_set - EN_NICHI overlay, Address: 0x80135C48, Size: 1160 bytes (290 insns) */
#include "types.h"

typedef union {
    s32 w;
    struct {
        s16 lo;
        s16 hi;
    } h;
} Fixed;

typedef struct {
    Fixed x;
    Fixed y;
} Marker;

extern Marker en_nichi_marker_tbl[];
extern s32 en_nichi_marker_timer[];
extern Marker en_nichi_grid_buf[15][12];

extern s32 en_nichi_sq_dist(s16 x0, s16 y0, s16 x1, s16 y1);

void en_nichi_ripple_set(s32 idx)
{
    Marker *m;
    Marker *p;
    s32 *rp;
    s32 row;
    s32 col;
    s32 gx;
    s32 gy;
    Fixed px;
    Fixed py;
    Fixed *qx;
    Fixed *qy;
    s32 d2;
    s32 r;
    s32 b;
    s32 my;

    m = &en_nichi_marker_tbl[idx];
    rp = &en_nichi_marker_timer[idx];
    qx = &px;
    qy = &py;
    for (col = 0, gy = -25; col != 12; col++, gy += 25) {
        for (row = 0, gx = -25; row != 15; row++, gx += 25) {
            qx->h.hi = gx;
            qy->h.hi = gy;
            d2 = en_nichi_sq_dist(m->x.h.hi, m->y.h.hi, qx->h.hi, qy->h.hi);
            r = *rp * 5;
            if (d2 >= r * r) {
                if (d2 <= (r + 5) * (r + 5)) {
                    p = &en_nichi_grid_buf[row][col];
                    my = m->y.w;
                    p->x.w += (px.w - m->x.w) * 2 / r;
                    p->y.w += (py.w - my) * 2 / r;
                } else {
                    b = r + 10;
                    if (d2 > b * b) {
                        if (d2 <= (r + 15) * (r + 15)) {
                            p = &en_nichi_grid_buf[row][col];
                            my = m->y.w;
                            p->x.w -= (px.w - m->x.w) * 3 / b;
                            p->y.w -= (py.w - my) * 3 / b;
                        } else {
                            b = r + 20;
                            if (d2 > b * b) {
                                if (d2 <= (r + 25) * (r + 25)) {
                                    p = &en_nichi_grid_buf[row][col];
                                    my = m->y.w;
                                    p->x.w += (px.w - m->x.w) * 3 / b;
                                    p->y.w += (py.w - my) * 3 / b;
                                } else {
                                    b = r + 30;
                                    if (d2 > b * b) {
                                        if (d2 <= (r + 35) * (r + 35)) {
                                            p = &en_nichi_grid_buf[row][col];
                                            my = m->y.w;
                                            p->x.w -= (px.w - m->x.w) * 2 / b;
                                            p->y.w -= (py.w - my) * 2 / b;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
