/* @SymbolStart: 0x8013521C */
/* @RodataBase: 0x8013F504 */
/* @SymbolSize: 1404 */
/* @PostProc: patch_insn_reg_field 0x13c:rt:t3:v0 0x140:rs:t3:v0 0x140:rt:t3:a2 0x148:rd:t4:t3 0x148:rt:t3:a2 0x14c:rt:t4:t3 0x184:rt:t5:t4 0x188:rs:t5:t4 0x188:rt:t5:t4 0x190:rs:t5:t4 0x190:rt:t6:t5 0x198:rt:t6:t5 0x1c0:rt:t7:t6 0x1c4:rs:t7:t6 0x1c4:rt:t7:t6 0x1cc:rd:t8:t7 0x1cc:rt:t7:t6 0x1d0:rt:t8:t7 0x1ec:rt:t9:t8 0x1f0:rs:t9:t8 0x1f0:rt:t9:t8 0x1f8:rs:t9:t8 0x1f8:rt:t0:t9 0x200:rt:t0:t9 0x21c:rt:t1:t0 0x220:rs:t1:t0 0x220:rt:t1:t0 0x228:rd:t2:t1 0x228:rt:t1:t0 0x22c:rt:t2:t1 0x23c:rt:t3:v0 0x240:rs:t3:v0 0x240:rt:t3:a2 0x248:rd:t4:t2 0x248:rt:t3:a2 0x24c:rt:t4:t2 0x284:rt:t5:t3 0x288:rs:t5:t3 0x288:rt:t5:t3 0x290:rs:t5:t3 0x290:rt:t6:t4 0x298:rt:t6:t4 0x2dc:rt:t7:t5 0x2e0:rs:t7:t5 0x2e0:rt:t7:t5 0x2e8:rs:t7:t5 0x2e8:rt:t8:t6 0x2ec:rt:t8:t6 0x330:rt:t9:t7 0x334:rs:t9:t7 0x334:rt:t9:t7 0x33c:rs:t9:t7 0x33c:rt:t0:t8 0x340:rt:t0:t8 0x378:rt:t1:t9 0x37c:rs:t1:t9 0x37c:rt:t1:t9 0x384:rs:t1:t9 0x384:rt:t2:t0 0x38c:rt:t2:t0 0x3a0:rt:t3:t1 0x3ac:rt:t3:t1 0x3b8:rt:t4:t2 0x3bc:rs:t4:t2 0x3bc:rt:t4:t2 0x3c4:rd:t5:t3 0x3c4:rt:t4:t2 0x3c8:rt:t5:t3 0x3d8:rt:t6:v0 0x3dc:rs:t6:v0 0x3dc:rt:t6:a2 0x3e4:rd:t7:t4 0x3e4:rt:t6:a2 0x3e8:rt:t7:t4 0x420:rt:t8:t5 0x424:rs:t8:t5 0x424:rt:t8:t5 0x42c:rs:t8:t5 0x42c:rt:t9:t6 0x434:rt:t9:t6 0x474:rt:t0:t7 0x478:rs:t0:t7 0x478:rt:t0:t7 0x480:rs:t0:t7 0x480:rt:t1:t8 0x488:rt:t1:t8 0x4d4:rt:t2:t9 0x4d8:rs:t2:t9 0x4d8:rt:t2:t9 0x4e0:rs:t2:t9 0x4e0:rt:t3:t0 0x4e8:rt:t3:t0 0x4fc:rt:v0:t1 0x500:rs:v0:t1 0x500:rt:v0:t1 0x504:rt:a0:v1 0x508:rs:v0:t1 0x508:rt:a0:v1 0x50c:rt:v1:v0 0x510:rs:v1:v0 0x510:rt:v1:v0 0x514:rt:a1:a0 0x518:rt:v0:t4 0x51c:rd:t4:t2 0x520:rd:t5:t3 0x520:rs:v1:v0 0x520:rt:t4:t2 0x524:rs:t5:t3 0x524:rt:a1:a0 0x528:rs:v0:t4 0x528:rt:v0:t4 0x530:rs:v0:t4 0x530:rt:a0:v1 0x534:rd:t6:t5 0x538:rd:t7:t6 0x538:rs:v1:v0 0x538:rt:t6:t5 0x540:rs:t7:t6 0x540:rt:a1:a0 */
/* tel_call_start_step - TEL overlay, Address: 0x80135224, Size: 1396 bytes (349 insns) */
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;

extern void k_sub_reset(void);
extern void gnsx(s32);
extern void sndi(s32, s32, s32);
extern void xa_set_m(s32, s32, s32, s32);
extern void k_sub_disp_start(s32);
extern void xa_wait_m(void);
extern void xa_wait(void);
extern void sndisp();
extern void device_cmd_send(s32);
extern void class_clear_f(void);
extern void text_sys_init(void);
extern void set_dec_bri(s32);
extern void return_schedule(s32);
extern void menu_bar_show(s32);
extern void cal_base_show(void);

extern u8 step_mode;
extern volatile u8 now_girl;
extern u8 tel_serifu_girl;
extern u8 screen_w[];
extern u8 p_sei[];

extern s32 date_name_set_tbl0[];
extern s32 date_name_set_tbl0_7;
extern s32 tel_ikari_goyou_serifu_tbl[];
extern s32 tel_ikari_p_serifu_tbl[];
extern s32 tel_ikari_kiru_serifu_tbl[];
extern s32 tel_ikari_sayonara_serifu_tbl[];

extern u8 tel_call_start_step_str0[];
extern u8 tel_call_start_step_str1[];
extern u8 tel_call_start_step_str2[];
extern u8 tel_call_start_step_str3[];
extern u8 tel_call_start_step_str4[];
extern u8 tel_call_start_step_str5[];
extern u8 tel_call_start_step_str6[];
extern u8 tel_call_start_step_str7[];
extern u8 tel_ikari_goyou_xa_tbl[];
extern u8 tel_ikari_goyou_xa_tbl2[];
extern u8 tel_ikari_kiru_xa_tbl[];
extern u8 tel_ikari_kiru_xa_tbl2[];
extern u8 tel_ikari_sayonara_xa_tbl[];
extern u8 tel_ikari_sayonara_xa_tbl2[];

void tel_call_start_step(void)
{
    s32 t6_hidden = step_mode;
    s32 idx;

    switch ((u32)t6_hidden) {
    case 0:
        k_sub_reset();
        gnsx(date_name_set_tbl0_7);
        sndi((s32)tel_call_start_step_str0, 0, 7);
        xa_set_m(0x05000000, 0x0100000B, -1, 7);
        k_sub_disp_start(0);
        step_mode = (u8)(step_mode + 1);
        break;

    case 1:
        xa_wait_m();
        break;

    case 2:
        k_sub_reset();
        gnsx((s32)p_sei);
        sndi((s32)tel_call_start_step_str1, 0, 15);
        sndi((s32)tel_call_start_step_str2, 1, 15);
        k_sub_disp_start(0);
        step_mode = (u8)(step_mode + 1);
        break;

    case 3:
        xa_wait();
        break;

    case 4:
        k_sub_reset();
        gnsx(date_name_set_tbl0[tel_serifu_girl]);
        sndisp(tel_ikari_goyou_serifu_tbl[tel_serifu_girl], 0);
        xa_set_m((s32)tel_ikari_goyou_xa_tbl, (s32)tel_ikari_goyou_xa_tbl2,
                      tel_serifu_girl, tel_serifu_girl);
        k_sub_disp_start(0);
        step_mode = (u8)(step_mode + 1);
        break;

    case 5:
        xa_wait_m();
        break;

    case 6:
        k_sub_reset();
        gnsx((s32)p_sei);
        sndisp(tel_ikari_p_serifu_tbl[tel_serifu_girl], 0, 0x1F);
        k_sub_disp_start(0);
        step_mode = (u8)(step_mode + 1);
        break;

    case 7:
        xa_wait();
        break;

    case 8:
        k_sub_reset();
        gnsx(date_name_set_tbl0[tel_serifu_girl]);
        sndisp(tel_ikari_kiru_serifu_tbl[tel_serifu_girl], 0);
        xa_set_m((s32)tel_ikari_kiru_xa_tbl, (s32)tel_ikari_kiru_xa_tbl2,
                      tel_serifu_girl, tel_serifu_girl);
        k_sub_disp_start(0);
        step_mode = (u8)(step_mode + 1);
        break;

    case 9:
        xa_wait_m();
        break;

    case 10:
        k_sub_reset();
        gnsx((s32)p_sei);
        sndisp((s32)tel_call_start_step_str3, 0, 15);
        k_sub_disp_start(0);
        step_mode = (u8)(step_mode + 1);

    case 11:
        xa_wait();
        break;

    case 12:
        k_sub_reset();
        gnsx((s32)p_sei);
        sndisp((s32)tel_call_start_step_str4, 0, 0x1F);
        k_sub_disp_start(0);
        step_mode = (u8)(step_mode + 1);

    case 13:
        xa_wait();
        break;

    case 14:
        k_sub_reset();
        sndisp((s32)tel_call_start_step_str5, 0, 0x2F);
        k_sub_disp_start(0);
        step_mode = (u8)(step_mode + 1);
        break;

    case 15:
        xa_wait();
        break;

    case 16:
        step_mode = 20;
        break;

    case 20:
        k_sub_reset();
        gnsx(date_name_set_tbl0[tel_serifu_girl]);
        sndisp(tel_ikari_sayonara_serifu_tbl[tel_serifu_girl], 0);
        xa_set_m((s32)tel_ikari_sayonara_xa_tbl, (s32)tel_ikari_sayonara_xa_tbl2,
                      tel_serifu_girl, tel_serifu_girl);
        k_sub_disp_start(0);
        step_mode = (u8)(step_mode + 1);
        break;

    case 21:
        xa_wait_m();
        break;

    case 22:
        k_sub_reset();
        device_cmd_send(0x50B);
        sndi((s32)tel_call_start_step_str6, 0, 0x2F);
        k_sub_disp_start(0);
        step_mode = (u8)(step_mode + 1);
        break;

    case 23:
        xa_wait();
        break;

    case 24:
        k_sub_reset();
        device_cmd_send(0x50C);
        gnsx((s32)p_sei);
        sndisp((s32)tel_call_start_step_str7, 0, 0x1F);
        k_sub_disp_start(0);
        step_mode = (u8)(step_mode + 1);
        break;

    case 25:
        xa_wait();
        break;

    case 17:
    case 18:
    case 19:
    default:
        idx = now_girl;
        *(s16 *)((u8 *)screen_w + idx * 56 + 0x1C2) = 15;
        idx = now_girl;
        *(s16 *)((u8 *)screen_w + idx * 56 + 0x1C2) = 15;
        class_clear_f();
        text_sys_init();
        set_dec_bri(0);
        return_schedule(0);
        break;
    }

    menu_bar_show(0);
    cal_base_show();
}
