/* @SymbolSize: 1536 */
/* @CompileEnv: IDO52_CONST_NOINREG_VALUES=1 */
/* @PostProc: patch_stack_complete --old 72 --new 80 */
/* @PostProc: remap_stack_slots --map '36:48,48:68,52:56,60:64,64:76,68:52' */
/* @PostProc: swap_instructions --offset1 0x40 --offset2 0x44 */
/* @PostProc: swap_instructions --offset1 0x98 --offset2 0xa0 */
/* @PostProc: patch_insn_reg_field 0x14:rt:t6:t9 0x18:rs:t6:t9 0x18:rt:t6:t9 0x1c:rt:t8:t6 0x20:rt:t9:t8 0x24:rs:t8:t6 0x24:rt:t8:t6 0x28:rs:t9:t8 0x28:rt:t9:t8 0x30:rt:t0:t7 0x34:rs:t0:t7 0x38:rs:t6:t9 0x38:rt:t7:t0 0x48:rt:t8:t6 0x4c:rt:t9:t8 0x50:rs:v1:a3 0x50:rt:v1:a3 0x54:rt:v0:v1 0x60:rs:t8:t6 0x60:rt:a0:v0 0x64:rs:v0:v1 0x68:rs:v1:a3 0x68:rt:v1:a3 0x70:rs:v0:v1 0x78:rs:v0:v1 0x84:rs:v0:v1 0xc4:rt:v1:a3 0xf0:rt:v1:a3 0x100:rt:v1:a3 0x104:rt:a0:v1 0x10c:rs:a0:v1 0x118:rs:a0:v1 0x11c:rt:t6:t8 0x124:rs:a0:v1 0x140:rt:v1:a3 0x17c:rt:v1:a3 0x188:rt:t7:t6 0x190:rs:t7:t6 0x190:rt:t6:t8 0x194:rt:v1:a3 0x1d4:rt:v1:a3 0x1ec:rt:v1:a3 0x22c:rt:v1:a3 0x248:rt:v1:a3 0x264:rt:v1:a3 0x268:rt:t8:t7 0x270:rs:v1:a3 0x270:rt:t8:t7 0x278:rt:v1:a3 0x284:rt:v1:a3 0x28c:rs:v1:a3 0x294:rt:a0:v1 0x2a0:rs:v1:a3 0x2a4:rs:v1:a3 0x2a8:rs:v1:a3 0x2a8:rt:a0:v1 0x2ac:rs:v1:a3 0x2b0:rs:v1:a3 0x2b4:rs:v1:a3 0x2bc:rs:v1:a3 0x2bc:rt:v1:a3 0x2c4:rt:t7:t6 0x2d4:rt:t6:t8 0x2d8:rs:v1:a3 0x2d8:rt:t6:t8 0x2e0:rt:a1:a0 0x2e4:rt:a1:a0 0x2e8:rs:v1:a3 0x2e8:rt:a1:a0 0x2ec:rs:v1:a3 0x2f0:rs:v1:a3 0x2f0:rt:t7:t6 0x2f4:rs:v1:a3 0x2f4:rt:a0:v1 0x2f8:rs:v1:a3 0x2fc:rs:v1:a3 0x300:rs:v1:a3 0x308:rs:v1:a3 0x308:rt:v1:a3 0x30c:rt:t8:t7 0x314:rs:t8:t7 0x344:rt:a1:a0 0x348:rt:a1:a0 0x35c:rd:t6:t8 0x360:rs:t6:t8 0x360:rt:t7:t6 0x364:rt:t8:t7 0x36c:rt:t7:t6 0x378:rt:t8:t7 0x38c:rs:v1:a3 0x390:rs:v1:a3 0x394:rs:v1:a3 0x398:rs:v1:a3 0x398:rt:a0:v1 0x39c:rs:v1:a3 0x3a0:rs:v1:a3 0x3a4:rs:v1:a3 0x3c4:rt:t6:t8 0x3cc:rs:t6:t8 0x3d0:rt:t7:t6 0x3d8:rt:t7:t6 0x3e0:rt:t8:t7 0x400:rt:t8:t7 0x434:rt:v1:a0 0x440:rt:v1:a0 0x448:rd:a0:v1 0x450:rd:a0:v1 0x454:rs:a0:v1 0x464:rt:v1:a0 0x46c:rt:t7:t6 0x470:rt:t6:t8 0x478:rs:t6:t8 0x484:rs:t7:t6 0x484:rt:t7:t6 0x48c:rs:t7:t6 0x4a8:rt:t8:t7 0x4b0:rs:t8:t7 0x4e8:rt:t6:t8 0x4ec:rt:t6:t8 0x4f0:rt:t7:t6 0x4f8:rs:t7:t6 0x500:rs:a0:v0 0x504:rt:t8:t7 0x518:rt:t8:t7 0x548:rt:t3:v1 0x550:rd:t4:t3 0x550:rs:t3:v1 0x554:rs:t4:t3 0x554:rt:t5:t4 0x558:rs:t5:t4 0x558:rt:t6:t5 0x55c:rs:t6:t5 0x564:rt:t7:t8 0x568:rs:t7:t8 0x568:rt:t7:t8 0x570:rs:t7:t8 0x574:rs:t7:t8 0x574:rt:v1:a3 0x578:rs:v1:a3 0x57c:rt:t9:t6 0x584:rs:t9:t6 0x598:rt:t2:t1 0x59c:rt:t0:t9 0x5a0:rt:t4:t3 0x5a4:rs:t0:t9 0x5a4:rt:t8:t0 0x5a8:rs:t8:t0 0x5a8:rt:t1:t7 0x5ac:rs:t1:t7 0x5b4:rt:t8:t0 0x5b8:rt:t3:t2 0x5bc:rt:t2:t1 0x5c4:rt:t3:t2 0x5c8:rt:t4:t3 0x5cc:rt:t5:t4 0x5d0:rt:t6:t5 0x5d4:rs:t5:t4 0x5d8:rt:t7:t8 0x5e0:rt:t6:t5 0x5e4:rt:t7:t8 0x40:rt:v1:a3 0x44:rt:t7:t0 0xa0:rt:v1:a3 */
/* tt_level_select_scene_main - TT overlay, Address: 0x80144D60, Size: 1536 bytes (384 insns) */
/* here only by @SymbolSize: 1536.  The split should be */
/* - @CompileEnv: IDO52_CONST_NOINREG_VALUES=1 — the target rematerializes the */
#include "types.h"

extern s32 device_cmd_send();
extern s32 get_reads();
extern s32 bzero();
extern s32 tt_obj_clear();
extern s32 tt_shot_clear();
extern s32 tt_cd_read();
extern s32 get_tt_read_prev();
extern s32 tt_recs_clear();
extern s32 tt_pool_clear();

extern s32 tt_game_wk;
extern s32 tt_step_wk;
extern s32 tt_scroll_wk;
extern s32 tt_stage_wk;
extern s32 tt_anim_buf;
extern s32 tt_text_buf;

void tt_level_select_scene_main(void)
{
    u8 *pMenu;
    u8 *pW;
    u8 *pSnd;
    u8 *st;
    u8 *rec;
    u8 *pa;
    u8 *pad;
    u8 *base;
    s32 x;
    s32 y;
    s32 d;
    s32 t;
    u32 sel;

    pMenu = (u8 *)tt_anim_buf + 3200;
    pW = (u8 *)tt_game_wk;
    pSnd = (u8 *)tt_stage_wk;
    sel = *(u16 *)((u8 *)tt_step_wk + 2);
    st = (u8 *)tt_step_wk;
    rec = (u8 *)tt_text_buf + 320;
    pa = (u8 *)tt_scroll_wk;
    pad = pW + 16;

    switch (sel) {
    case 0:
        device_cmd_send(34);
        device_cmd_send(47);
        tt_pool_clear();
        tt_obj_clear();
        tt_shot_clear();
        st[17] = 0;
        if ((u32)pa[11] < 3U) {
            device_cmd_send(4);
            device_cmd_send(514);
            switch (pa[11]) {
            case 0:
                *(u16 *)(pW + 164) = 1;
                tt_cd_read(0x80016000, 0xC877, 9248);
                tt_cd_read(0x8002E800, 0xC825, 27108);
                tt_cd_read(0x8019A000, 0xC828, 0x27D60);
                break;
            case 1:
                *(u16 *)(pW + 164) = 2;
                tt_cd_read(0x80016000, 0xC8CB, 9248);
                tt_cd_read(0x8002E800, 0xC87B, 27108);
                tt_cd_read(0x8019A000, 0xC87F, 0x27D60);
                break;
            case 2:
                *(u16 *)(pW + 164) = 3;
                tt_cd_read(0x80016000, 0xC924, 9248);
                tt_cd_read(0x8002E800, 0xC8CF, 27108);
                tt_cd_read(0x8019A000, 0xC8D4, 0x27D60);
                break;
            }
            tt_cd_read(0x8001A000, 0xBFDF, 3616);
            tt_cd_read(0x801C2000, 0xBFAA, 0x1A680);
            *(s16 *)(rec + 10) = -56;
        } else {
            device_cmd_send(4);
            device_cmd_send(512);
            *(s16 *)(rec + 10) = -32;
        }
        *(u16 *)(rec + 12) = 116;
        *(u16 *)(rec + 14) = 11;
        rec[2] = 4;
        rec[0] = 1;
        *(u16 *)(rec + 6) = 0;
        *(u16 *)(rec + 16) = 0;
        rec += 20;
        if ((u32)pa[11] < 3U) {
            *(u16 *)(rec + 10) = 8;
        } else {
            *(u16 *)(rec + 10) = 16;
        }
        *(u16 *)(rec + 12) = 116;
        *(u16 *)(rec + 14) = 12;
        rec[2] = 4;
        rec[0] = 1;
        *(u16 *)(rec + 6) = 0;
        *(u16 *)(rec + 16) = 0;
        rec += 20;
        if ((u32)pa[11] < 3U) {
            pMenu[0] = 1;
            pMenu[1] = 0;
            *(u16 *)(pMenu + 68) = 0;
            *(s16 *)(pMenu + 20) = -12;
            *(u16 *)(pMenu + 22) = 108;
            *(u16 *)(pMenu + 60) = 16;
            *(u16 *)(pMenu + 62) = 16;
            pMenu[70] = (u8)((pa[11] << 4) + 16);
            pMenu[71] = 32;
            *(u16 *)(pMenu + 64) = 10;
            *(u16 *)(pMenu + 66) = 15368;
            pMenu[59] = 101;
            *(u16 *)(pMenu + 16) = 0;
        } else {
            *(s16 *)(rec + 10) = -64;
            *(u16 *)(rec + 12) = 116;
            *(u16 *)(rec + 14) = 10;
            rec[2] = 4;
            rec[0] = 1;
            *(u16 *)(rec + 6) = 0;
            *(u16 *)(rec + 16) = 0;
        }
        *(u16 *)(st + 46) = 128;
        st[29] = 128;
        st[30] = 128;
        st[31] = 128;
        st[28] = 3;
        *(s32 *)(pa + 4) = 0;
        tt_recs_clear();
        *(u16 *)(st + 2) = 64;
        break;

    case 64:
        *(u16 *)(st + 46) = *(u16 *)(st + 46) - 4;
        if (*(u16 *)(st + 46) == 0) {
            *(s32 *)(pa + 0) = 0;
            *(u16 *)(st + 38) = 0;
            *(u16 *)(st + 2) = 80;
        }
        st[29] = (u8)*(u16 *)(st + 46);
        st[30] = (u8)*(u16 *)(st + 46);
        st[31] = (u8)*(u16 *)(st + 46);
        if (*(s32 *)(pa + 4) < *(s32 *)(pa + 0)) {
            *(s32 *)(pa + 0) = *(s32 *)(pa + 0) - 1024;
        } else {
            *(s32 *)(pa + 0) = *(s32 *)(pa + 0) + 512;
        }
        t = *(s32 *)(pa + 0) - *(s32 *)(pa + 4);
        if (t < 0) {
            d = -t;
        } else {
            d = t;
        }
        if (d < 4096) {
            *(s32 *)(pa + 0) = *(s32 *)(pa + 4);
        }
        break;

    case 80:
        if ((u32)((u8 *)tt_scroll_wk)[11] < 3U) {
            if ((u32)*(u16 *)((u8 *)tt_step_wk + 38) >= 401U) {
                *(u16 *)(st + 2) = 160;
            } else {
                *(u16 *)(st + 38) = *(u16 *)(st + 38) + 1;
            }
        } else {
            if ((u32)*(u16 *)((u8 *)tt_step_wk + 38) >= 431U) {
                *(u16 *)(st + 2) = 160;
            } else {
                *(u16 *)(st + 38) = *(u16 *)(st + 38) + 1;
            }
        }
        if (st[34] != 0) {
            break;
        }
        if ((*(u16 *)(pad + 4) & 0x820) == 0) {
            break;
        }
        *(u16 *)(st + 2) = 160;
        break;

    case 160:
        if ((u32)((u8 *)tt_scroll_wk)[11] < 3U) {
            x = get_reads();
            y = get_tt_read_prev();
            if ((s32)(u8)((x - y) + 256) < 5) {
                break;
            }
        }
        base = (u8 *)tt_text_buf;
        rec = base + 320;
        rec[0] = 0;
        rec += 20;
        rec[0] = 0;
        pMenu[0] = 0;
        bzero(pSnd, 32);
        pa[11] = pa[11] + 1;
        if ((u32)pa[11] < 4U) {
            *(u16 *)(st + 0) = 2;
            *(u16 *)(st + 2) = 1;
        } else {
            pa[15] = 1;
            if (st[34] != 0) {
                *(u16 *)(st + 0) = 8;
            } else {
                *(u16 *)(st + 0) = 6;
            }
            *(u16 *)(st + 6) = 0;
            *(u16 *)(st + 4) = 0;
            *(u16 *)(st + 2) = 0;
            st[11] = 0;
            st[10] = 0;
            st[9] = 0;
        }
        break;
    }
}
