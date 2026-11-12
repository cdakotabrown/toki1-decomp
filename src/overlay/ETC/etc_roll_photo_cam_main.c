/* @CompileFlags: -Wc,-pic0 */
/* @PostProc: remap_stack_slots --map '52:48,28:40' */
/* @PostProc: swap_instructions --offset1 0x94 --offset2 0x98 */
/* @PostProc: swap_instructions --offset1 0x1d4 --offset2 0x1d8 */
/* @PostProc: swap_instructions --offset1 0x468 --offset2 0x470 */
/* @PostProc: swap_instructions --offset1 0x46c --offset2 0x474 */
/* @PostProc: swap_instructions --offset1 0x470 --offset2 0x478 */
/* @PostProc: swap_instructions --offset1 0x474 --offset2 0x478 */
/* @PostProc: patch_insn_reg_field 0x28:rt:a2:a1 0x2c:rt:a2:a1 0x34:rs:a2:a1 0x44:rd:v0:v1 0x4c:rs:v0:v1 0x9c:rt:a1:v0 0xa0:rs:a0:t9 0xa0:rt:t9:a0 0xa4:rt:v1:a3 0xa8:rs:a1:v0 0xb0:rt:v1:a3 0xf0:rt:a2:a1 0xf4:rt:v1:t0 0xfc:rs:v1:t0 0x108:rs:v1:t0 0x110:rs:v0:v1 0x114:rs:v0:v1 0x118:rt:t0:t1 0x120:rt:t0:t1 0x128:rt:t1:t2 0x12c:rt:t1:t2 0x148:rt:t2:t3 0x150:rt:t2:t3 0x15c:rt:t3:t4 0x160:rs:t3:t4 0x160:rt:t3:t4 0x168:rs:t3:t4 0x168:rt:t4:t5 0x16c:rt:t4:t5 0x188:rt:a2:a1 0x190:rs:a2:a1 0x19c:rd:t5:t6 0x1a0:rs:t5:t6 0x1b4:rd:t6:t9 0x1b8:rd:t6:t9 0x1b8:rs:t6:t9 0x1bc:rd:t6:t9 0x1bc:rt:t6:t9 0x1c0:rd:t6:t9 0x1c0:rs:t6:t9 0x1c4:rd:t6:t9 0x1c4:rt:t6:t9 0x1cc:rd:t6:t9 0x1cc:rs:t6:t9 0x1d8:rs:t6:t9 0x1e0:rt:t6:t9 0x1f4:rd:t9:t1 0x1fc:rd:t9:t1 0x204:rd:t1:t2 0x204:rs:t9:t0 0x204:rt:t0:t1 0x20c:rt:t1:t2 0x21c:rd:t2:t4 0x224:rd:t2:t4 0x230:rd:t4:t5 0x230:rs:t2:t3 0x230:rt:t3:t4 0x238:rt:t4:t5 0x250:rt:t5:t6 0x254:rs:t5:t6 0x2e4:rd:t6:t7 0x2e8:rt:t6:t7 0x304:rt:t8:t9 0x308:rt:t7:t8 0x30c:rs:t7:t8 0x310:rt:t0:t1 0x314:rs:t8:t9 0x314:rt:t8:t9 0x31c:rs:t8:t9 0x31c:rt:t9:t0 0x320:rt:t9:t0 0x324:rs:t0:t1 0x328:rt:t3:t4 0x32c:rt:t1:t2 0x330:rs:t1:t2 0x330:rt:t1:t2 0x338:rs:t1:t2 0x338:rt:t2:t3 0x33c:rt:t2:t3 0x340:rs:t3:t4 0x344:rt:t6:t7 0x348:rt:t4:t5 0x34c:rs:t4:t5 0x34c:rt:t4:t5 0x354:rs:t4:t5 0x354:rt:t5:t6 0x358:rt:t5:t6 0x35c:rs:t6:t7 0x360:rt:t9:t0 0x364:rt:t7:t8 0x368:rs:t7:t8 0x368:rt:t7:t8 0x370:rs:t7:t8 0x370:rt:t8:t9 0x374:rt:t8:t9 0x378:rs:t9:t0 0x37c:rt:t2:t3 0x380:rt:t0:t1 0x384:rs:t0:t1 0x384:rt:t0:t1 0x38c:rs:t0:t1 0x38c:rt:t1:t2 0x390:rt:t1:t2 0x394:rs:t2:t3 0x398:rt:t5:t6 0x39c:rt:t3:t4 0x3a0:rs:t3:t4 0x3a0:rt:t3:t4 0x3a8:rs:t3:t4 0x3a8:rt:t4:t5 0x3ac:rt:t4:t5 0x3b0:rs:t5:t6 0x3b4:rt:t4:t5 0x3b8:rt:t6:t7 0x3bc:rs:t6:t7 0x3c0:rt:t9:t0 0x3c4:rt:t7:t8 0x3c8:rs:t7:t8 0x3c8:rt:t7:t8 0x3d0:rs:t7:t8 0x3d0:rt:t8:t9 0x3d4:rt:t8:t9 0x3d8:rs:t9:t0 0x3dc:rt:t2:t3 0x3e0:rt:t0:t1 0x3e4:rs:t0:t1 0x3e4:rt:t0:t1 0x3ec:rs:t0:t1 0x3ec:rt:t1:t2 0x3f0:rt:t1:t2 0x3f4:rs:t2:t3 0x3f8:rt:t5:t6 0x3fc:rt:t3:t4 0x400:rs:t3:t4 0x400:rt:t3:t4 0x408:rs:t3:t4 0x408:rt:t4:t5 0x40c:rt:t4:t5 0x410:rs:t5:t6 0x414:rt:t8:t9 0x418:rt:t6:t7 0x41c:rs:t6:t7 0x41c:rt:t6:t7 0x424:rs:t6:t7 0x424:rt:t7:t8 0x428:rt:t7:t8 0x42c:rs:t8:t9 0x430:rt:t1:t2 0x434:rt:t9:t0 0x438:rs:t9:t0 0x438:rt:t9:t0 0x440:rs:t9:t0 0x440:rt:t0:t1 0x444:rt:t0:t1 0x448:rs:t1:t2 0x44c:rt:t2:t3 0x450:rs:t2:t3 0x450:rt:t2:t3 0x458:rs:t2:t3 0x458:rt:t3:t4 0x460:rt:t3:t4 0x464:rs:t4:t5 0x464:rt:t4:t5 0x46c:rs:t4:t5 0x46c:rt:t5:t6 0x470:rt:t5:t6 0x474:rt:t6:t7 0x478:rs:t6:t7 0x478:rt:t6:t7 0x480:rs:t6:t7 0x480:rt:t7:t8 0x484:rt:t7:t8 0x488:rt:t8:t9 0x48c:rs:t8:t9 0x48c:rt:t8:t9 0x494:rs:t8:t9 0x494:rt:t9:t0 0x498:rt:t9:t0 */
/* Address: 0x801439F0 */
/* Size: 1180 bytes (295 instructions) */
/* `@PostProc: pad_trailing_nop --count 287` -- the entire body was fake. */
#include "types.h"

extern void sub_sub_step_up(void);
extern s32 get_CD_State(void);
extern void debug_hex_show(s32, s32, s32);
extern s32 rsin(s32);
extern s32 rcos(s32);
extern void load_etc_tpage(s32);
extern void etc_roll_move(void);

extern u8 now_girl;
extern s32 pad_data;
extern s32 step_counter;
extern s16 etc_roll_tx;
extern s16 etc_roll_ty;
extern s16 etc_roll_tz;
extern s16 etc_roll_rot[];
typedef struct { s16 c; s16 e; } CamCE;
volatile CamCE D_8015015C_v = {0, 0};
CamCE D_8015015C_s = {0, 0};
extern volatile s16 etc_roll_rot_2_v;
extern s32 etc_roll_pic;
extern s32 etc_roll_allgirl[];
extern s32 etc_roll_girl_tbl[];

void etc_roll_photo_cam_main(void)
{
    volatile s32 q;
    s32 r;
    s32 r2;
    s32 i;
    s32 *p;
    s32 x;
    s32 t;
    s32 b;
    u32 n701;

    etc_roll_move();
    n701 = 701;
    q = (u32)step_counter / n701;
    r = (u32)step_counter % n701;
    if (r == 1) {
        i = etc_roll_pic;
        if (i == 0) {
            load_etc_tpage(now_girl);
        } else if (i < 15) {
            p = &etc_roll_girl_tbl[i];
            x = p[-1];
            p++;
            i = i + 1;
            if (now_girl == x) {
                x = p[-1];
                etc_roll_pic = i;
            }
            load_etc_tpage(x);
        } else {
            load_etc_tpage(now_girl);
        }
        etc_roll_pic = etc_roll_pic + 1;
    } else {
        i = q;
        if (i < 15) {
        if (i != 0) {
            if (r == 0) {
                etc_roll_tx = -96;
                etc_roll_ty = 32;
                etc_roll_rot[2] = 0;
                D_8015015C_s.c = 0;
                D_8015015C_s.e = 0;
                etc_roll_tz = -29400;
            } else if (r < 80U) {
                etc_roll_tz = etc_roll_tz + 370;
            }
        }
        }
    }

    if (etc_roll_pic < 15) {
        r2 = (u32)step_counter % n701;
        if (r2 >= 622U) {
            t = q * 573;
            etc_roll_tz = etc_roll_tz + 300;
            etc_roll_ty = etc_roll_ty + rsin(t) / 256;
            etc_roll_tx = etc_roll_tx + rcos(t) / 256;
        }
    }

    if ((u32)step_counter >= 601U) {
        if (get_CD_State() & 2) {
            sub_sub_step_up();
        }
    }

    debug_hex_show(etc_roll_tx, 20, -40);
    debug_hex_show(etc_roll_ty, 20, -30);
    debug_hex_show(etc_roll_tz, 20, -20);
    debug_hex_show(step_counter, 0, -10);
    debug_hex_show(q, 0, 0);
    debug_hex_show(etc_roll_pic, 0, 10);
    debug_hex_show(etc_roll_allgirl[etc_roll_pic], 0, 20);

    b = pad_data;
    if (b & 0x8000) {
        etc_roll_tx = etc_roll_tx + 20;
    }
    if (b & 0x2000) {
        etc_roll_tx = etc_roll_tx - 20;
    }
    if (b & 0x4000) {
        etc_roll_ty = etc_roll_ty + 20;
    }
    if (b & 0x1000) {
        etc_roll_ty = etc_roll_ty - 20;
    }
    if (b & 0x0004) {
        etc_roll_tz = etc_roll_tz - 20;
    }
    if (b & 0x0001) {
        etc_roll_tz = etc_roll_tz + 20;
    }
    if (b & 0x00FA) {
        if (b & 0x0008) {
            etc_roll_rot[2] = etc_roll_rot[2] + 10;
        }
        if (b & 0x0002) {
            etc_roll_rot[2] = etc_roll_rot[2] - 10;
        }
        if (b & 0x0080) {
            etc_roll_rot[0] = etc_roll_rot[0] + 10;
        }
        if (b & 0x0020) {
            etc_roll_rot[0] = etc_roll_rot[0] - 10;
        }
        if (b & 0x0010) {
            etc_roll_rot[1] = etc_roll_rot[1] + 10;
        }
        if (b & 0x0040) {
            etc_roll_rot[1] = etc_roll_rot[1] - 10;
        }
    } else {
        etc_roll_rot[2] = etc_roll_rot[2] - 3;
        D_8015015C_v.c = D_8015015C_v.c - 4;
        D_8015015C_v.e = D_8015015C_v.e - 5;
    }
}
