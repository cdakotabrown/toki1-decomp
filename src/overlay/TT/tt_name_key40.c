/* @CompileFlags: -Wc,-pic0 */
/* @SymbolSize: 1332 */
/* @PostProc: remap_stack_slots --map '29:43' */
/* @PostProc: patch_insn_reg_field 0x10c:rt:v0:t7 0x114:rs:v0:t7 0x114:rt:v0:t4 0x118:rs:v0:t4 0x118:rt:t7:t5 0x11c:rs:t7:t5 0x124:rt:v0:t4 0x130:rt:t4:t8 0x138:rs:t4:t8 0x140:rt:t5:t9 0x144:rs:t5:t9 0x148:rt:t6:t8 0x14c:rt:v0:t0 0x154:rs:v0:t0 0x154:rt:v0:t1 0x158:rs:v0:t1 0x158:rt:t8:t2 0x15c:rs:t8:t2 0x164:rt:v0:t1 0x174:rt:t9:t3 0x178:rt:t0:t6 0x17c:rt:t3:t5 0x180:rs:t0:t6 0x180:rt:t1:t7 0x184:rt:t1:t7 0x188:rt:t2:t4 0x18c:rt:t9:t3 0x190:rs:t2:t4 0x194:rt:t8:t4 0x19c:rt:t3:t5 0x1a0:rs:t6:t8 0x1a4:rt:t8:t4 0x1a8:rt:v0:t9 0x1b0:rs:v0:t9 0x1b0:rt:v0:t0 0x1b4:rs:v0:t0 0x1b4:rt:t7:t1 0x1b8:rs:t7:t1 0x1c0:rt:v0:t0 0x1d0:rt:t4:t2 0x1d4:rt:v0:t3 0x1d8:rt:t4:t2 0x1dc:rs:v0:t3 0x1dc:rt:v0:t6 0x1e0:rs:v0:t6 0x1e0:rt:t5:t7 0x1e4:rs:t5:t7 0x1ec:rt:v0:t6 0x1f4:rt:t8:t4 0x1fc:rs:t8:t4 0x204:rt:v0:t5 0x208:rs:v0:t5 0x20c:rt:v0:t5 0x210:rt:a1:v0 0x218:rs:a1:v0 0x230:rt:v0:t8 0x238:rs:v0:t8 0x238:rt:v0:t9 0x23c:rs:v0:t9 0x23c:rt:a1:v0 0x240:rt:v0:t9 0x244:rt:t9:t1 0x248:rt:t1:t0 0x24c:rd:t0:t2 0x24c:rt:t9:t1 0x250:rd:t0:t2 0x250:rs:t0:t2 0x250:rt:t9:t1 0x254:rd:t0:t2 0x254:rt:t0:t2 0x258:rd:t2:t3 0x258:rt:t1:t2 0x25c:rd:t3:t6 0x25c:rs:t2:t3 0x25c:rt:a1:v0 0x260:rs:t3:t6 0x260:rt:t6:t7 0x268:rt:t6:t7 0x26c:rt:t7:t4 0x274:rs:t7:t4 0x27c:rt:t3:t5 0x280:rt:v0:t5 0x284:rs:v0:t5 0x288:rt:t3:t5 0x28c:rt:a1:v0 0x294:rs:a1:v0 0x2b0:rt:v0:t8 0x2b8:rs:v0:t8 0x2b8:rt:v0:t9 0x2bc:rs:v0:t9 0x2bc:rt:a1:v0 0x2c0:rt:v0:t9 0x2c4:rt:t4:t0 0x2c8:rt:t8:t1 0x2cc:rd:t5:t2 0x2cc:rt:t4:t0 0x2d0:rd:t5:t2 0x2d0:rs:t5:t2 0x2d0:rt:t4:t0 0x2d4:rd:t5:t2 0x2d4:rt:t5:t2 0x2d8:rd:t9:t3 0x2d8:rs:t5:t1 0x2d8:rt:t8:t2 0x2dc:rd:t0:t6 0x2dc:rs:t9:t3 0x2dc:rt:a1:v0 0x2e0:rs:t0:t6 0x2e0:rt:t1:t7 0x2e8:rt:t1:t7 0x2ec:rt:t2:t4 0x2f4:rs:t2:t4 0x2fc:rt:t3:t5 0x300:rs:t3:t5 0x304:rt:t0:t6 0x318:rt:t8:t2 0x320:rt:t7:t9 0x32c:rt:t6:t8 0x330:rt:t6:t8 0x334:rt:t7:t9 0x338:rt:t4:t0 0x340:rs:t4:t0 0x348:rt:t0:t6 0x34c:rt:t5:t1 0x350:rs:t8:t2 0x350:rt:t5:t1 0x354:rt:t9:t3 0x35c:rs:t9:t3 0x364:rt:t0:t6 0x368:rs:t0:t6 0x36c:rt:t2:t3 0x370:rt:t1:t7 0x378:rs:t1:t7 0x37c:rt:t2:t3 0x390:rt:t3:t5 0x394:rt:t6:t8 0x398:rt:t5:t1 0x39c:rd:t7:t9 0x39c:rt:t6:t8 0x3a0:rd:t7:t9 0x3a0:rs:t7:t9 0x3a0:rt:t6:t8 0x3a4:rd:t7:t9 0x3a4:rt:t7:t9 0x3a8:rd:t4:t0 0x3a8:rs:t3:t5 0x3a8:rt:t7:t9 0x3ac:rt:t2:t4 0x3b0:rd:t8:t2 0x3b0:rs:t4:t0 0x3b0:rt:t5:t1 0x3b4:rs:t8:t2 0x3b4:rt:t2:t4 0x3b8:rt:v0:t3 0x3bc:rt:t9:t8 0x3c0:rs:v0:t3 0x3c0:rt:v0:t6 0x3c4:rt:v0:t6 0x3c8:rd:t0:t5 0x3c8:rt:t9:t8 0x3cc:rt:t1:t7 0x3d0:rd:t0:t5 0x3d0:rs:t0:t5 0x3d0:rt:t9:t8 0x3d4:rd:t0:t5 0x3d4:rt:t0:t5 0x3d8:rs:v0:t6 0x3d8:rt:t3:t0 0x3dc:rd:t6:t9 0x3dc:rs:t0:t7 0x3dc:rt:t1:t5 0x3e0:rd:t7:t1 0x3e0:rs:t6:t9 0x3e0:rt:t3:t0 0x3e4:rs:t7:t1 0x3f0:rt:t5:t2 0x3f8:rs:t5:t2 0x400:rt:t2:t3 0x404:rs:t2:t3 0x414:rt:t9:t8 0x41c:rt:t8:t6 0x420:rs:t9:t8 0x420:rt:t8:t6 0x424:rt:t6:t9 0x428:rt:t1:t5 0x42c:rd:t3:t0 0x42c:rt:t6:t9 0x430:rd:t3:t0 0x430:rs:t3:t0 0x430:rt:t6:t9 0x438:rd:t3:t0 0x438:rt:t3:t0 0x43c:rt:t0:t7 0x440:rd:t7:t1 0x440:rs:t1:t5 0x440:rt:t3:t0 0x444:rd:t5:t2 0x444:rs:t7:t1 0x448:rs:t5:t2 0x448:rt:t0:t7 0x458:rt:t2:t3 0x460:rt:t8:t6 0x46c:rt:t9:t8 0x474:rt:t6:t9 0x47c:rt:t2:t3 0x484:rt:t8:t6 0x488:rt:t9:t8 0x48c:rt:t6:t9 0x490:rt:t7:t1 0x494:rs:t6:t9 0x494:rt:t1:t5 0x498:rt:a2:v0 0x49c:rs:t1:t5 0x49c:rt:t3:t0 0x4a0:rs:t3:t0 0x4a8:rt:a2:v0 0x4b0:rs:t7:t1 0x4b0:rt:a2:v0 0x4b4:rs:t7:t1 0x4b4:rt:a2:v0 0x4b8:rs:t7:t1 0x4b8:rt:a2:v0 0x4bc:rs:t7:t1 0x4bc:rt:a2:v0 0x4c0:rt:v0:a2 0x4c4:rt:t0:t4 0x4c8:rd:t4:t7 0x4c8:rt:v0:a2 0x4cc:rd:t4:t7 0x4cc:rs:t4:t7 0x4cc:rt:v0:a2 0x4d0:rd:t4:t7 0x4d0:rt:t4:t7 0x4d4:rt:t0:t7 0x4e4:rs:v0:a2 0x4ec:rt:t9:t8 0x4f0:rt:t5:t2 0x4f8:rd:t2:t3 0x4f8:rt:t5:t2 0x4fc:rs:t2:t3 0x4fc:rt:t8:t6 0x500:rs:t9:t8 0x500:rt:t8:t6 0x504:rt:t6:t9 0x50c:rd:t1:t5 0x50c:rt:t6:t9 0x510:rs:t1:t5 0x510:rt:t3:t0 0x514:rs:t9:t8 0x514:rt:t3:t0 0x518:rt:t7:t1 0x520:rs:t7:t1 0x528:rd:t0:t7 0x52c:rs:t0:t7 0x52c:rt:t5:t2 0x530:rs:t9:t8 0x530:rt:t5:t2 */
/* tt_name_key40 - TT overlay, Address: 0x80146338, Size: 1332 bytes (333 insns) */
#include "types.h"

extern s32 tt_cmd_once();
extern s32 tt_name_set();

extern u8 *tt_game_wk;
extern u8 *tt_step_wk;
extern u8 *tt_scroll_wk;
extern u8 *tt_anim_buf;
extern u8 *tt_text_buf;

void tt_name_key40(void)
{
    u8 *base;
    u8 *snd;
    u8 *st;
    u8 *rec;
    u8 *pa;
    u8 *pad;
    u16 col;
    u32 tmp;
    u32 btn;
    u16 rep;
    u16 row;
    u16 gl;
    u8 sel;
    u8 v;

    base = tt_game_wk;
    snd = tt_step_wk;
    st = tt_scroll_wk;
    rec = tt_text_buf + *(u16 *)(st + 24) * 60;
    pa = tt_anim_buf;
    pad = base + 16;

    switch (*(u16 *)(tt_step_wk + 4)) {
    case 0:
        *(u16 *)(st + 26) = 0;
        *(u16 *)(st + 28) = 10;
        *(u16 *)(snd + 4) = 64;
        break;

    case 64:
        if (*(u16 *)(pad + 4) & 0x8000) {
            tt_cmd_once(1557);
            *(u16 *)(st + 28) = *(u16 *)(st + 28) - 1;
            if (*(s16 *)(st + 28) < 0) {
                *(u16 *)(st + 28) = 47;
            }
            *(u16 *)(st + 16) = 0;
        } else if (*(u16 *)(pad + 4) & 0x2000) {
            tt_cmd_once(1557);
            tmp = *(u16 *)(st + 28) + 1;
            *(u16 *)(st + 28) = tmp;
            gl = tmp;
            if (gl >= 48U) {
                *(u16 *)(st + 28) = 0;
            }
            *(u16 *)(st + 16) = 0;
        }

        if (*(u16 *)(pad + 0) & 0x8000) {
            tmp = *(u16 *)(st + 16) + 1;
            *(u16 *)(st + 16) = tmp;
            rep = tmp;
            if (rep >= 31U) {
                tt_cmd_once(1557);
                *(u16 *)(st + 16) = 26;
                *(u16 *)(st + 28) = *(u16 *)(st + 28) - 1;
                if (*(s16 *)(st + 28) < 0) {
                    *(u16 *)(st + 28) = 47;
                }
            }
        } else if (*(u16 *)(pad + 0) & 0x2000) {
            tmp = *(u16 *)(st + 16) + 1;
            *(u16 *)(st + 16) = tmp;
            rep = tmp;
            if (rep >= 31U) {
                tt_cmd_once(1557);
                *(u16 *)(st + 16) = 26;
                tmp = *(u16 *)(st + 28) + 1;
                *(u16 *)(st + 28) = tmp;
                gl = tmp;
                if (gl >= 48U) {
                    *(u16 *)(st + 28) = 0;
                }
            }
        }

        btn = *(u16 *)(pad + 4) & 0x0004;
        if (btn != 0) {
            col = *(u16 *)(st + 26);
            if (col != 0) {
                tt_cmd_once(1559);
                tmp = *(u16 *)(st + 26) - 1;
                *(u16 *)(st + 26) = tmp;
                col = tmp;
            }
            *(u16 *)(st + 28) = *(base + *(u16 *)(st + 24) * 6 + col + 50);
        } else {
            btn = *(u16 *)(pad + 4) & 0x0008;
            if (btn != 0) {
                col = *(u16 *)(st + 26);
                if (col < 2U) {
                    tt_cmd_once(1559);
                    tmp = *(u16 *)(st + 26) + 1;
                    *(u16 *)(st + 26) = tmp;
                    col = tmp;
                }
                *(u16 *)(st + 28) =
                    *(base + *(u16 *)(st + 24) * 6 + col + 50);
            }
        }

        if (*(u16 *)(pad + 4) & 0x0020) {
            tt_cmd_once(1558);
            if (*(u16 *)(st + 26) < 2U) {
                *(u16 *)(st + 26) = *(u16 *)(st + 26) + 1;
                *(u16 *)(st + 28) = 10;
            } else {
                snd[10] = 1;
            }
        }

        if ((*(u16 *)(pad + 4) & 0x0040) && *(u16 *)(st + 26) != 0) {
            tt_cmd_once(1559);
            *(base + *(u16 *)(st + 24) * 6 + *(u16 *)(st + 26) + 50) = 63;
            tmp = *(u16 *)(st + 26) - 1;
            *(u16 *)(st + 26) = tmp;
            col = tmp;
            *(u16 *)(st + 28) = *(base + *(u16 *)(st + 24) * 6 + col + 50);
        }

        if (*(u16 *)(pad + 4) & 0x0800) {
            tt_cmd_once(1558);
            snd[10] = 1;
        }

        *(base + *(u16 *)(st + 24) * 6 + *(u16 *)(st + 26) + 50) =
            (u8)*(u16 *)(st + 28);
        break;
    }

    switch (*(u16 *)(st + 24)) {
    case 0:
        sel = 2;
        break;
    case 1:
        sel = 6;
        break;
    case 2:
        sel = 4;
        break;
    }

    if ((*(u16 *)(base + 10) & 3) != 0) {
        v = sel;
    } else {
        v = 15;
    }
    rec[42] = v;
    rec[22] = v;
    rec[2] = v;

    row = *(u16 *)(st + 24);
    tt_name_set(base + row * 6 + 50, row + 43);

    *(u16 *)(pa + 20) = *(u16 *)(st + 26) * 8 + 52;
    *(u16 *)(pa + 22) = *(u16 *)(st + 24) * 16 + 126;
    pa[0] = (u8)(((u32)*(u16 *)(base + 10) >> 3) & 1);
}
