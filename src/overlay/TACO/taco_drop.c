/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: patch_stack_complete --old 112 --new 104 */
/* @PostProc: patch_insn_reg_field 0x114:rd:s3:s2 0x248:rt:s2:s3 0x36c:rs:s2:s3 0x36c:rt:s2:s3 0x37c:rs:s3:s2 0x3dc:rd:s3:s2 0x41c:rt:s2:s3 0x450:rd:s3:s2 0x524:rs:t3:v1 0x524:rt:v1:t3 0x540:rt:s2:s3 */
/* Address: 0x8013A760 */
/* Size: 1492 bytes (373 instructions) */
#include "types.h"

typedef struct {
    s32 x;
    s32 h;
} Drop;

extern s32 pad_data;
extern u8 *taco_actor_rec;
extern Drop *taco_drop_ptr;

extern s32 rand(void);
extern void taco_drop_disp(s32, s32, s32, s32);

void
taco_drop(s32 arg0, s32 arg1)
{
    s32 lim;
    s32 dx;
    s32 top;
    s32 mul;
    s32 cnt;
    s32 v;
    s32 h;
    s32 i;

    lim = 30000;
    dx = 0;
    if (arg1 == 0) {
        v = pad_data;
        if (v & 1) {
            lim = 10000;
        } else if (v & 2) {
            lim = 50000;
        }
        if (v & 4) {
            dx = 40;
        }
        if (v & 8) {
            dx = -40;
        }
    } else if (arg1 == 1) {
        v = taco_actor_rec[2];
        if ((v & 1) == 1U) {
            dx = 40;
        }
        if ((v & 2) == 2U) {
            dx = -40;
        }
    } else if (arg1 == 2) {
        dx = 40;
        lim = 20000;
    } else if (arg1 == 3) {
        dx = 40;
        lim = 10000;
    }

    cnt = 0;
    if (dx > 0) {
        for (i = 0; i != 100; i++) {
            taco_drop_ptr[i].x = taco_drop_ptr[i].x + dx;
            if (taco_drop_ptr[i].x > 4096) {
                taco_drop_ptr[i].x = taco_drop_ptr[i].x - 4096;
            }
        }
    }
    if (dx < 0) {
        for (i = 0; i != 100; i++) {
            taco_drop_ptr[i].x = taco_drop_ptr[i].x + dx;
            if (taco_drop_ptr[i].x < 0) {
                taco_drop_ptr[i].x = taco_drop_ptr[i].x + 4096;
            }
        }
    }

    top = 240000;
    mul = 1000;
    for (i = 0; i != 100; i++) {
        h = taco_drop_ptr[i].h;
        if (h == 0) {
            if (rand() % (cnt + 1) == 0) {
                cnt = cnt + 1;
                taco_drop_ptr[i].x = rand() % 4096;
                if (arg0 == 0) {
                    taco_drop_ptr[i].h = top;
                } else {
                    taco_drop_ptr[i].h = mul;
                }
            }
            h = taco_drop_ptr[i].h;
        } else {
            cnt = cnt + 1;
        }
        if (arg0 == 0) {
            taco_drop_ptr[i].h = h - h * mul / lim;
            h = taco_drop_ptr[i].h;
            if (h < 3001) {
                taco_drop_ptr[i].h = 0;
                h = taco_drop_ptr[i].h;
            }
        } else {
            taco_drop_ptr[i].h = h + h * mul / lim;
            h = taco_drop_ptr[i].h;
            if (h >= top) {
                taco_drop_ptr[i].h = 0;
                h = taco_drop_ptr[i].h;
            }
        }
        if (h != 0) {
            taco_drop_disp(taco_drop_ptr[i].x, h / mul, -1, arg1);
        }
    }
}
