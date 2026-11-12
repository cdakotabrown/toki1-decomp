/* @SymbolStart: 0x8013FF2C */
/* @RodataBase: 0x8014CB00 */
/* @CompileEnv: IDO52_LDA_NOINREG_CLASSES=1,4 IDO52_NO_LOOP_TEST_REPLACEMENT=bb:42 IDO52_CONST_INREG_VALUES=128:8:1:7 IDO52_AS1_NO_LA_PULL=01 IDO52_AS1_NO_FALLTHRU_FILL=03 IDO52_AS1_KEEP_ANDI=occ:38,39 */
/* @PostProc: patch_stack_complete --old 168 --new 112 */
/* @PostProc: remap_stack_slots --map '100:84,104:88' */
/* @PostProc: swap_instructions --offset1 0x2a4 --offset2 0x2a8 */
/* @PostProc: swap_instructions --offset1 0x340 --offset2 0x344 */
/* @PostProc: swap_instructions --offset1 0x358 --offset2 0x364 */
/* @PostProc: swap_instructions --offset1 0x35c --offset2 0x364 */
/* @PostProc: swap_instructions --offset1 0x374 --offset2 0x37c */
/* @PostProc: swap_instructions --offset1 0x378 --offset2 0x37c */
/* @PostProc: swap_instructions --offset1 0x38c --offset2 0x3a0 */
/* @PostProc: swap_instructions --offset1 0x390 --offset2 0x3a0 */
/* @PostProc: swap_instructions --offset1 0x394 --offset2 0x39c */
/* @PostProc: swap_instructions --offset1 0x398 --offset2 0x3a0 */
/* @PostProc: swap_instructions --offset1 0x3a4 --offset2 0x3ac */
/* @PostProc: swap_instructions --offset1 0x3a8 --offset2 0x3b0 */
/* @PostProc: swap_instructions --offset1 0x3ac --offset2 0x3b4 */
/* @PostProc: swap_instructions --offset1 0x3b4 --offset2 0x3b8 */
/* @PostProc: swap_instructions --offset1 0x3bc --offset2 0x3c0 */
/* @PostProc: swap_instructions --offset1 0x3dc --offset2 0x3e0 */
/* @PostProc: swap_instructions --offset1 0x3f0 --offset2 0x3f8 */
/* @PostProc: swap_instructions --offset1 0x3f4 --offset2 0x418 */
/* @PostProc: swap_instructions --offset1 0x718 --offset2 0x71c */
/* @PostProc: swap_instructions --offset1 0x298 --offset2 0x29c */
/* @PostProc: swap_instructions --offset1 0x504 --offset2 0x508 */
/* @PostProc: swap_instructions --offset1 0x500 --offset2 0x504 */
/* @PostProc: swap_instructions --offset1 0x4fc --offset2 0x500 */
/* @PostProc: swap_instructions --offset1 0x4f8 --offset2 0x4fc */
/* @PostProc: swap_instructions --offset1 0x4f4 --offset2 0x4f8 */
/* @PostProc: swap_instructions --offset1 0x4f0 --offset2 0x4f4 */
/* @PostProc: swap_instructions --offset1 0x4ec --offset2 0x4f0 */
/* @PostProc: swap_instructions --offset1 0x4e8 --offset2 0x4ec */
/* @PostProc: swap_instructions --offset1 0x4e4 --offset2 0x4e8 */
/* @PostProc: swap_instructions --offset1 0x4e0 --offset2 0x4e4 */
/* @PostProc: swap_instructions --offset1 0x4dc --offset2 0x4e0 */
/* @PostProc: swap_instructions --offset1 0x508 --offset2 0x50c */
/* @PostProc: swap_instructions --offset1 0x558 --offset2 0x55c */
/* @PostProc: swap_instructions --offset1 0x554 --offset2 0x558 */
/* @PostProc: swap_instructions --offset1 0x550 --offset2 0x554 */
/* @PostProc: swap_instructions --offset1 0x54c --offset2 0x550 */
/* @PostProc: swap_instructions --offset1 0x548 --offset2 0x54c */
/* @PostProc: swap_instructions --offset1 0x544 --offset2 0x548 */
/* @PostProc: swap_instructions --offset1 0x540 --offset2 0x544 */
/* @PostProc: swap_instructions --offset1 0x53c --offset2 0x540 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v1:a1 0x14:rs:v1:a1 0x14:rt:v1:a1 0x1c:rs:v1:a1 0x28:rt:v1:a1 0xac:rt:t7:t0 0xb0:rs:t7:t0 0xb0:rt:t7:t0 0xb8:rs:t7:t0 0xbc:rt:t1:t9 0xc4:rt:t2:t1 0xc8:rt:t3:t2 0xcc:rt:t3:t2 0xd0:rt:t2:t1 0xe4:rd:t8:t7 0xe8:rd:t9:t8 0xe8:rt:t8:t7 0xec:rd:t1:t9 0xec:rs:t1:t9 0xec:rt:t9:t8 0xf0:rs:t1:t9 0xf0:rt:t1:t9 0xf8:rt:t1:t9 0xfc:rt:t4:a1 0x100:rs:t4:a1 0x100:rt:t4:a1 0x108:rs:t4:a1 0x108:rt:t5:t3 0x110:rt:t5:t3 0x130:rt:t6:t4 0x134:rt:t7:t5 0x138:rt:t7:t5 0x13c:rt:t6:t4 0x154:rt:t8:a1 0x158:rs:t8:a1 0x158:rt:t8:a1 0x160:rs:t8:a1 0x160:rt:t9:t6 0x168:rt:t9:t6 0x1a0:rt:t1:a1 0x1a4:rs:t1:a1 0x1a4:rt:t1:a1 0x1ac:rs:t1:a1 0x1ac:rt:t2:t7 0x1b4:rt:t2:t7 0x1c0:rt:t3:t8 0x1c4:rs:t3:t8 0x1c8:rt:t4:a1 0x1cc:rs:t4:a1 0x1cc:rt:t4:a1 0x1d4:rs:t4:a1 0x1d4:rt:t5:t9 0x1dc:rt:t5:t9 0x1e8:rt:t6:t1 0x1ec:rs:t6:t1 0x1f0:rt:t7:a1 0x1f4:rs:t7:a1 0x1f4:rt:t7:a1 0x1fc:rs:t7:a1 0x1fc:rt:t8:t2 0x204:rt:t8:t2 0x29c:rt:a1:a0 0x2a0:rs:a1:a0 0x2a0:rt:a1:a0 0x2b4:rt:a1:a0 0x2c0:rt:t9:t3 0x2c4:rt:t1:t4 0x2c8:rt:t2:t5 0x2cc:rt:t3:t6 0x2d0:rt:t4:t7 0x2d4:rt:t5:t8 0x2d8:rt:t5:t8 0x2dc:rt:t4:t7 0x2e0:rt:t3:t6 0x2e4:rt:t2:t5 0x2e8:rt:t1:t4 0x2ec:rt:t9:t3 0x304:rt:v0:a0 0x30c:rt:v0:a0 0x338:rt:a1:t9 0x33c:rt:t0:t1 0x340:rs:a1:t1 0x340:rt:a1:t1 0x344:rs:t0:t9 0x344:rt:t0:t9 0x34c:rd:t6:t2 0x350:rd:v1:v0 0x354:rt:t7:t3 0x358:rd:v0:v1 0x358:rs:t6:t2 0x358:rt:t7:t3 0x35c:rd:t1:t6 0x35c:rs:v1:v0 0x360:rt:t0:t9 0x364:rt:a1:t1 0x370:rd:t8:t4 0x370:rs:v1:v0 0x370:rt:t0:t9 0x374:rs:v0:v1 0x374:rt:t8:t4 0x378:rd:t2:t7 0x378:rs:v1:v0 0x37c:rd:t9:t5 0x37c:rs:v1:v0 0x37c:rt:a1:t1 0x380:rs:v0:v1 0x380:rt:t9:t5 0x384:rs:v0:v1 0x384:rt:t2:t7 0x388:rs:v0:v1 0x388:rt:t1:t6 0x38c:rt:t5:t1 0x390:rt:t8:t4 0x394:rt:t4:t9 0x398:rt:t7:t3 0x39c:rt:t6:t2 0x3a0:rt:t3:t8 0x3a4:rt:t3:t8 0x3a8:rt:t6:t2 0x3ac:rt:t7:t3 0x3b0:rt:t4:t9 0x3b4:rt:t8:t4 0x3b8:rt:t5:t1 0x3d0:rt:t9:t5 0x3d8:rt:t9:t5 0x41c:rt:a2:t0 0x420:rs:a2:t0 0x424:rt:t3:t8 0x428:rs:t3:t8 0x428:rt:t3:t8 0x42c:rd:t2:t7 0x430:rd:v0:v1 0x430:rs:t2:t7 0x430:rt:t3:t8 0x434:rs:v0:v1 0x438:rs:v0:v1 0x44c:rt:t4:a1 0x450:rs:t4:a1 0x450:rt:t4:a1 0x458:rs:t4:a1 0x458:rt:t5:t9 0x460:rt:t5:t9 0x46c:rt:t6:t1 0x470:rs:t6:t1 0x474:rt:t7:a1 0x478:rs:t7:a1 0x478:rt:t7:a1 0x480:rs:t7:a1 0x480:rt:t8:t2 0x488:rt:t8:t2 0x494:rt:a3:v1 0x498:rs:a3:v1 0x4a8:rt:t9:t3 0x4c8:rt:t9:t3 0x4d4:rt:t1:t4 0x4d8:rs:t1:t4 0x4dc:rt:t4:a1 0x4e0:rt:t2:t5 0x4e4:rs:t2:t5 0x4e4:rt:t2:t5 0x4ec:rs:t2:t5 0x4ec:rt:t3:t6 0x4f0:rt:t3:t6 0x508:rs:t4:a1 0x508:rt:t4:a1 0x51c:rs:t4:a1 0x51c:rt:t5:t7 0x524:rt:t5:t7 0x528:rt:a3:v1 0x52c:rs:a3:v1 0x52c:rt:a3:v1 0x530:rt:a2:t0 0x534:rs:a3:v1 0x53c:rs:a3:v1 0x53c:rt:t6:t8 0x540:rs:a3:v1 0x560:rt:t6:t8 0x564:rs:a2:t0 0x564:rt:a2:t0 0x568:rt:t7:t9 0x56c:rs:a2:t0 0x574:rs:t7:t9 0x574:rt:t7:t9 0x57c:rs:t7:t9 0x590:rt:t8:t1 0x594:rs:t8:t1 0x594:rt:t8:t1 0x598:rt:t9:a1 0x59c:rs:t8:t1 0x5a8:rs:t9:a1 0x5a8:rt:t9:a1 0x5b8:rs:t9:a1 0x5b8:rt:t1:t2 0x5c0:rt:t1:t2 0x5c4:rs:a2:t0 0x5c8:rt:t2:t3 0x5cc:rs:t2:t3 0x5cc:rt:t2:t3 0x5d4:rs:t2:t3 0x5e8:rt:t3:t4 0x5ec:rs:t3:t4 0x5ec:rt:t3:t4 0x5f0:rt:t4:a1 0x5f4:rs:t3:t4 0x600:rs:t4:a1 0x600:rt:t4:a1 0x610:rs:t4:a1 0x620:rs:a2:t0 0x624:rs:v1:a1 0x624:rt:t1:t9 0x650:rt:t8:a1 0x660:rs:t8:a1 0x660:rt:t8:a1 0x670:rs:t8:a1 0x670:rt:t9:t8 0x678:rt:t9:t8 0x680:rt:t1:t9 0x684:rt:t2:t1 0x688:rs:t2:t1 0x688:rt:t2:t1 0x690:rs:t2:t1 0x690:rt:t3:t2 0x698:rt:t3:t2 0x69c:rt:a3:v1 0x6a0:rs:a3:v1 0x6a0:rt:a3:v0 0x6a8:rt:a3:v0 0x6b0:rt:a3:v0 0x6b8:rt:a3:v0 0x6bc:rs:a3:v0 0x6bc:rt:a3:v1 0x6c0:rs:a3:v1 0x6c0:rt:t4:t3 0x6c8:rt:t4:t3 0x6cc:rs:t4:t3 0x6d4:rs:v1:a1 0x6d4:rt:t5:t4 0x6e0:rt:t5:t4 0x6ec:rt:t6:a1 0x6f0:rs:t6:a1 0x6f0:rt:t6:a1 0x6f8:rs:t6:a1 0x6f8:rt:t7:t5 0x700:rt:t7:t5 0x704:rt:v0:a0 0x70c:rt:v0:a0 0x714:rt:v0:a0 0x718:rs:v1:a1 0x718:rt:t8:t6 0x724:rt:t8:t6 0x728:rt:t9:t7 0x734:rt:t9:t7 0x738:rt:t2:a1 0x73c:rs:t2:a1 0x73c:rt:t2:a1 0x740:rt:t1:t8 0x748:rt:t1:t8 0x750:rs:t2:a1 0x750:rt:t3:t9 0x758:rt:t3:t9 0x764:rt:t4:t1 0x768:rs:t4:t1 0x768:rt:t4:t1 0x770:rs:t4:t1 0x770:rt:t5:t2 0x774:rs:t5:t2 0x7a0:rt:t6:a1 0x7a4:rs:t6:a1 0x7a4:rt:t6:a1 0x7ac:rs:t6:a1 0x7ac:rt:t7:t3 0x7b4:rt:t7:t3 0x7c0:rt:t8:t4 0x7d0:rs:t8:t4 0x7d0:rt:t8:t4 0x7d8:rs:t8:t4 0x7fc:rt:t9:t5 0x800:rs:t9:t5 0x800:rt:t9:t5 0x808:rs:t9:t5 0x82c:rt:t1:t6 0x830:rs:t1:t6 0x830:rt:t1:t6 0x838:rs:t1:t6 0x838:rt:t2:t7 0x840:rt:t2:t7 */
/* Address: 0x8013FF34 */
/* Size: 2108 bytes (527 instructions) */
/* @SymbolSize: 2116 */
/* - @RodataBase: 0x8014CB00 resolves the jr-table lui/lw pair ([7..9]). */
#include "types.h"
#include "tokimeki/libgs_types.h"

extern s32 draw2d3d(s32, s32);
extern s32 back_clear_switch(s32);
extern void palette_upload_enqueue(s32, s32, s32, s32, s32);
extern s32 device_cmd_send(s32);
extern void device_load_start(s32, s32, s32, s32, s32, s32);
extern s32 device_load_poll(void);
extern s32 get_CD_State2(void);
extern s32 get_CD_State(void);
extern void set_XAsector(s32, s32, s32);
extern void cd_read_start(s32, s32, s32);
extern void cd_read_start2(s32, s32, s32);
extern s32 class_switch(s32);
extern s32 class_clear_all(s32);
extern void bg_set();
extern void text_sys_init(void);
extern void title_init_View(void);
extern void title_init_Light(void);
extern void title_init_Coordinate(void);
extern void set_flag_b3_high_bit(s32);
extern void prim_init(void);
extern void sub_step_up(void);
extern void tpage_buf_clear(void);
extern void load_tpage_buf_lock(s32, s32, s32);
extern void memcpy(s32, s32, s32);
extern void etc_xa_step(s32);
extern void etc_model_init(void);
extern void etc_model_matrix_set(void);
extern void etc_frame_save2(void);
extern void etc_girl_record_search(void);
extern void etc_event_load(s32);
extern void etc_record_reset(void);
extern s32 date2_menu_call_judge(void);

extern u8 back_clear_r;
extern u8 back_clear_g;
extern u8 back_clear_b;
extern u8 screen_bri;
extern u8 now_girl;
extern s32 pad_trig;
extern s32 step_counter;
extern u8 step_mode;
extern s32 Bg;
extern s32 etc_xa_phase;
extern s32 etc_event_step;
extern s32 etc_xa_sel;
extern s32 etc_xa_tbl;
extern s32 etc_model_roty;
extern s32 etc_call_choice;
extern s32 etc_call_pass_cnt;
extern u8 date2_menu_select_tbl0;
extern u8 date2_menu_select_tbl1;
extern u8 etc_init_tbl0;

extern s32 etc_xa_list_tbl[];

void etc_init(void)
{
    switch (step_mode) {
    case 0:
        draw2d3d(1, 0);
        back_clear_g = 0;
        back_clear_r = 0;
        back_clear_b = 0;
        class_switch(1);
        class_clear_all(0);
        set_flag_b3_high_bit(0);
        prim_init();
        text_sys_init();
        back_clear_switch(1);
        tpage_buf_clear();
        etc_xa_sel = 0;
        etc_record_reset();
        etc_xa_tbl = etc_xa_list_tbl[now_girl % 13U];
        device_load_start(0, 0xBFA6, 0xBFA6, 0xCFB5, 0xCF99, 0xCF91);
        step_mode++;
        break;

    case 1:
        if (device_load_poll() != 1) {
            return;
        }
        device_cmd_send(513);
        device_load_start(1, 0xC5C5, 0xC5A2, 0xD6C0, 0xD672, 0xD651);
        step_mode++;
        break;

    case 2:
        if (device_load_poll() != 1) {
            return;
        }
        cd_read_start(1, 0x801FA800, 0xBE17);
        cd_read_start2(105, 0x80180000, 0xB389);
        step_mode++;
        break;

    case 3:
        if ((get_CD_State2() & 1) == 0) {
            return;
        }
        step_mode++;
        break;

    case 4:
        if ((get_CD_State2() & 1) == 0) {
            return;
        }
        step_mode++;
        break;

    case 5: {
        u8 *sp84[17];
        s32 n;
        draw2d3d(1, 1);
        etc_model_init();
        title_init_View();
        title_init_Light();
        title_init_Coordinate();
        palette_upload_enqueue(0x80180000, 31, 1, 3, 0);
        palette_upload_enqueue(0x80180200, 30, 1, 3, 0);
        palette_upload_enqueue(0x80180400, 29, 1, 3, 0);
        load_tpage_buf_lock(15, 0x801B0600, 1);
        {
            s32 *dst = (s32 *)sp84;
            s32 val = 0x80198600;
            while (val < (s32)0x801B0600) {
                *dst = val;
                val += 0x4000;
                dst++;
            }
        }
        bg_set(sp84, 1, 6, 3, 2, 510, -160, -125, 12, 21);
        (*(s32 *)&((LIBGS_BG *)&Bg)[1].attribute) = 0x1000000;
        ((LIBGS_BG *)&Bg)[1].b = 0;
        ((LIBGS_BG *)&Bg)[1].g = 0;
        ((LIBGS_BG *)&Bg)[1].r = 0;
        screen_bri = 0;
        for (n = 0; n < 6; n++) {
            sp84[n] = (u8 *)0x80180600 + n * 0x4000;
        }
        bg_set(sp84, 0, 6, 3, 2, 511, -160, -125, 13, 24);
        Bg = (s32)0x80000000;
        ((LIBGS_BG *)&Bg)[0].g = 128;
        ((LIBGS_BG *)&Bg)[0].r = 128;
        screen_bri = 0;
        ((LIBGS_BG *)&Bg)[0].b = 128;
        memcpy(0x801E2000, 0x80180600, 0x18000);
        {
            extern s32 etc_init_xa_tbl[]; s32 idx = now_girl;
            s32 *entry = etc_init_xa_tbl + idx * 2;
            set_XAsector(entry[0], entry[1], idx);
        }
        device_cmd_send(768);
        step_mode++;
        break;
    }

    case 6:
        if ((get_CD_State() & 4) == 0) {
            return;
        }
        step_mode++;
        break;

    case 7:
        date2_menu_call_judge();
        {
            u8 v1 = screen_bri;
            if (v1 < 129U) {
                ((LIBGS_BG *)&Bg)[1].b = v1;
                ((LIBGS_BG *)&Bg)[1].g = v1;
                ((LIBGS_BG *)&Bg)[1].r = v1;
                screen_bri = v1 + 1;
            }
        }
        if ((get_CD_State() & 2) == 0) {
            return;
        }
        etc_call_pass_cnt++;
        date2_menu_select_tbl0 = 0;
        date2_menu_select_tbl1 = 0;
        etc_init_tbl0 = 0;
        step_counter = 0;
        step_mode++;
        break;

    case 8: {
        s32 t0;
        u32 v1 = screen_bri;
        u8 m = v1;
        if (v1 < 129U) {
            ((LIBGS_BG *)&Bg)[1].b = m;
            ((LIBGS_BG *)&Bg)[1].g = m;
            ((LIBGS_BG *)&Bg)[1].r = m;
            screen_bri = (u8)(v1 + 1);
        }
        t0 = now_girl;
        if (t0 == 0) {
            if ((u32)step_counter >= 30U) {
                etc_xa_step(0);
                if (etc_xa_phase >= 3) {
                    etc_xa_phase = 0;
                    step_mode++;
                }
            }
        } else if (t0 == 9) {
            if ((u32)step_counter >= 30U) {
                etc_xa_step(0);
                if (etc_xa_phase >= 3) {
                    etc_xa_phase = 0;
                    step_mode++;
                }
            }
        } else if (t0 == 12) {
            if ((u32)step_counter >= 30U) {
                etc_xa_step(0);
                if (etc_xa_phase >= 3) {
                    etc_xa_phase = 0;
                    step_mode++;
                }
            }
        } else {
            step_mode = step_mode + 1;
        }
        step_counter++;
        break;
    }

    case 9: {
        u8 v0 = screen_bri;
        u8 nv;
        ((LIBGS_BG *)&Bg)[1].b = v0;
        ((LIBGS_BG *)&Bg)[1].g = v0;
        ((LIBGS_BG *)&Bg)[1].r = v0;

        nv = screen_bri = v0 + 1;
        if (nv < 128U) {
            return;
        }
        step_mode = step_mode + 1;
        break;
    }

    case 10:
        etc_frame_save2();
        step_mode++;
        break;

    case 11:
        (*(s32 *)&((LIBGS_BG *)&Bg)[1].attribute) = 0x1000000;
        Bg = 0x1000000;
        step_mode = step_mode + 1;
        break;

    case 12:
        etc_model_roty = 0x800;
        etc_model_matrix_set();
        (*(s32 *)&((LIBGS_BG *)&Bg)[1].attribute) = (s32)0x80000000;
        step_mode++;
        break;

    case 13:
        date2_menu_call_judge();
        if ((pad_trig & 0x20) == 0) {
            return;
        }
        {
            s32 v0 = etc_call_choice;
            if (v0 == 0 || v0 == 2) {
                etc_girl_record_search();
            }
        }
        step_mode++;
        break;

    case 14: {
        s32 v0 = etc_event_step;
        if (v0 >= 3) {
            if (etc_model_roty >= 3072) {
                etc_event_step = 0;
                sub_step_up();
                return;
            }
        }
        if (v0 == -1) {
            if (etc_model_roty >= 3072) {
                sub_step_up();
                return;
            }
        }
        etc_event_load(1);
        etc_model_roty += 8;
        etc_model_matrix_set();
        break;
    }
    }
}
