/* @CompileEnv: IDO52_AS1_XBB_VOLATILE_OK=all */
/* @SymbolStart: 0x80133D54 */
/* Address: 0x80133D5C */
/* Size: 2108 bytes (527 insns) */
/* @SymbolSize: 2116 */
/* @RodataBase: 0x8013F288 */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* @PostProc: patch_stack_complete --old 128 --new 136 */
/* @PostProc: remap_stack_slots --from 92 --to 60 --from 48 --to 92 */
/* @PostProc: patch_insn_reg_field 0x1d8:rt:t7:a2 0x1e8:rs:t7:a2 0x1e8:rt:t7:a2 0x1f0:rs:t7:a2 0x1f4:rt:t0:t9 0x1f8:rt:t8:t7 0x1fc:rs:t8:t7 0x1fc:rt:t8:t7 0x204:rs:t8:t7 0x204:rt:t9:t8 0x20c:rt:t9:t8 0x210:rs:t0:t9 0x210:rt:t0:t9 0x214:rt:t2:t1 0x218:rd:t1:t0 0x218:rt:t0:t9 0x21c:rd:t1:t0 0x21c:rs:t1:t0 0x21c:rt:t0:t9 0x220:rd:t1:t0 0x220:rt:t1:t0 0x224:rd:t2:t1 0x224:rs:t2:t1 0x224:rt:t1:t0 0x228:rs:t2:t1 0x228:rt:t2:t1 0x230:rt:t2:t1 0x47c:rt:t3:a2 0x480:rs:t3:a2 0x480:rt:t3:a2 0x488:rd:t4:t3 0x488:rt:t3:a2 0x48c:rd:t5:t4 0x48c:rt:t4:t3 0x490:rs:t5:t4 0x4e4:rt:t6:t5 0x4e8:rs:t6:t5 0x4e8:rt:t6:t5 0x4f0:rs:t6:t5 0x4f0:rt:t7:t6 0x4f8:rt:t7:t6 0x584:rt:t8:t7 0x588:rs:t8:t7 0x588:rt:t8:t7 0x590:rs:t8:t7 0x590:rt:t9:t8 0x598:rt:t9:t8 0x630:rt:t0:t9 0x634:rs:t0:t9 0x634:rt:t0:t9 0x63c:rs:t0:t9 0x63c:rt:t1:t0 0x644:rt:t1:t0 0x69c:rt:t2:t1 0x6a0:rs:t2:t1 0x6a0:rt:t2:t1 0x6a8:rs:t2:t1 0x6a8:rt:t3:t2 0x6b0:rt:t3:t2 0x73c:rt:t4:t3 0x740:rs:t4:t3 0x740:rt:t4:t3 0x748:rs:t4:t3 0x748:rt:t5:t4 0x750:rt:t5:t4 0x778:rt:t6:t5 0x780:rs:t6:t5 0x784:rt:t7:t6 0x788:rs:t7:t6 0x788:rt:t7:t6 0x790:rs:t7:t6 0x794:rd:t8:t7 0x79c:rs:t8:t7 0x7a8:rt:t1:t0 0x7ac:rd:t9:t8 0x7b0:rs:t9:t8 0x7b0:rt:t0:t9 0x7b4:rt:t0:t9 0x7b8:rs:t1:t0 0x7b8:rt:t1:t0 0x7c0:rs:t1:t0 0x7c4:rd:t2:t1 0x7c8:rd:t2:t1 0x7c8:rs:t2:t1 0x7cc:rd:t2:t1 0x7cc:rt:t2:t1 0x7d0:rd:t2:t1 0x7d0:rs:t2:t1 0x7d4:rd:t2:t1 0x7d4:rt:t2:t1 0x7d8:rd:t2:t1 0x7d8:rs:t2:t1 0x7e0:rt:t2:t1 0x7e8:rd:t3:t2 0x7f0:rd:t3:t2 0x7f4:rs:t3:t2 0x7f4:rt:t4:t3 0x7f8:rt:t4:t3 0x808:rd:t5:t4 0x810:rs:t5:t4 0x814:rt:t7:t6 0x818:rd:t6:t5 0x81c:rs:t7:t6 0x81c:rt:t8:t7 0x820:rt:t6:t5 0x824:rt:t8:t7 */
#include "types.h"
#include "tokimeki/class_w.h"

extern u8 step_mode;

extern void text_sys_init(void);
extern void tel_girl_status_init(void);
extern void k_sub_reset_point_set(void);
extern void k_sub_disp_start(s32);
extern void k_sub_reset(void);
extern void gnsx(s32);
extern void sndi(char *, s32, s32);
extern void xa_set_m(s32, s32, s32, s32);
extern void xa_wait_m(void);
extern void xa_wait(void);
extern void menu_girl_taku_set(s32, s32 *);
extern void k_disp_start(s32);
extern void menu_check(s32, s32, s32);
extern void menu_bar_show(s32);
extern void menu_cursor_move(s32);
extern void get_g_name(char *, s32);
extern void strcpy(char *, s32);
extern void strcat(char *, s32);
extern void goto_sub_sub_step(s32);
extern s32  rsin(s32);
extern void appraisal_base_show(void);

extern u8  step_mode;
extern volatile s8 menu_csr;
extern u32 pad_trig;
extern s32 step_counter;
extern s16 cursor_cw_x;
extern s16 cursor_cw_y;
extern s32 get_g_name_tbl0[];
extern s32 date_name_set_tbl0[];

typedef struct { s32 flag; u8 rest[52]; } S56;
extern S56 girl0_flag[];

typedef struct {
    u8  pad0[3];
    u8  f3;
    u8  pad4[20];
    s16 f24;
    s16 f26;
    s16 f28;
    u8  pad30[18];
    s32 f48;
    u8  pad52[16];
} S68;

extern char p_sei[];
extern char tel_dialog_disp_str0[];
extern char tel_dialog_disp_str1[];
extern char tel_dialog_disp_str2[];
extern char tel_dialog_disp_str3[];
extern char tel_dialog_disp_str4[];
extern char tel_dialog_disp_str5[];
extern char tel_dialog_disp_str6[];
extern char tel_dialog_disp_str7[];
extern char tel_dialog_disp_str8[];
extern char tel_dialog_disp_str9[];
extern char tel_dialog_disp_str10[];
extern char tel_dialog_disp_str11[];
extern char tel_dialog_disp_str12[];
extern char tel_dialog_disp_str13[];
extern char tel_dialog_disp_str14[];
extern char tel_dialog_disp_str15[];
extern char tel_dialog_disp_str16[];
extern char tel_dialog_disp_str17[];
extern char tel_dialog_disp_str18[];
extern char tel_dialog_disp_str19[];
extern char tel_dialog_disp_str20[];
extern char tel_yoshio_name_xa_tbl[];
extern char tel_yoshio_name_xa_tbl2[];
extern s32 work_fuku[];
extern CLASS_W class_w[];

void tel_dialog_disp(void)
{
    s32 idx = step_mode;
    char buf60[32];
    char buf92[44];
    S68 *p;
    s32 i;
    s32 v;
    s8  c;

    switch (idx) {
    case 0:
        text_sys_init();
        tel_girl_status_init();
        k_sub_reset_point_set();
        k_sub_disp_start(0);
        step_mode += 1;
        goto Lexit;

    case 1:
        k_sub_reset();
        gnsx(get_g_name_tbl0[1]);
        sndi(tel_dialog_disp_str0, 0, 13);
        xa_set_m((s32)0x1E00005E, (s32)0x12000061, -1, 13);
        k_sub_disp_start(0);
        step_mode += 1;
        goto Lexit;

    case 2:
        xa_wait_m();
        goto Lexit;

    case 3:
        k_sub_reset();
        gnsx((s32)p_sei);
        sndi(tel_dialog_disp_str1, 0, 31);
        sndi(tel_dialog_disp_str2, 1, 31);
        k_sub_disp_start(0);
        step_mode += 1;
        goto Lexit;

    case 4:
        xa_wait();
        goto Lexit;

    case 5:
        k_sub_reset();
        menu_girl_taku_set(1, &work_fuku[2]);
        k_disp_start(work_fuku[2] + 1);
        step_mode += 1;
        goto Lexit;

    case 6:
        menu_check(1, cursor_cw_x, cursor_cw_y);
        menu_bar_show(1);
        menu_cursor_move(1);
        if ((pad_trig & 0x20) == 0)
            goto Lexit;
        if (menu_csr == 11) {
            step_mode += 1;
            goto Lexit;
        }
        if ((girl0_flag[menu_csr].flag << 30) < 0)
            step_mode += 1;
        goto Lexit;

    case 7:
        k_sub_reset();
        gnsx((s32)p_sei);
        c = menu_csr;
        if (c == 11)
            get_g_name(buf60, 12);
        else
            get_g_name(buf60, c);
        strcpy(buf92, (s32)buf60);
        strcat(buf92, (s32)tel_dialog_disp_str3);
        sndi(buf92, 0, 15);
        sndi(tel_dialog_disp_str4, 1, 15);
        sndi(tel_dialog_disp_str5, 2, 15);
        k_sub_disp_start(0);
        c = menu_csr;
        if (c == 10) {
            step_mode = 32;
            goto Lexit;
        }
        if (c == 11) {
            goto_sub_sub_step(4);
            goto Lexit;
        }
        step_mode += 1;
        goto Lexit;

    case 8:
        xa_wait();
        goto Lexit;

    case 9:
        k_sub_reset();
        gnsx(get_g_name_tbl0[1]);
        sndi(tel_dialog_disp_str6, 0, 13);
        sndi(tel_dialog_disp_str7, 1, 13);
        xa_set_m((s32)0x180000C2, (s32)0x150000C9, -1, 13);
        k_sub_disp_start(0);
        step_mode += 1;
        goto Lexit;

    case 10:
        xa_wait_m();
        goto Lexit;

    case 11:
        k_sub_reset();
        gnsx(get_g_name_tbl0[1]);
        c = menu_csr;
        if (c == 11) {
            strcpy(buf92, get_g_name_tbl0[0]);
            strcat(buf92, (s32)tel_dialog_disp_str8);
            strcat(buf92, get_g_name_tbl0[0]);
        } else {
            strcpy(buf92, date_name_set_tbl0[c]);
            strcat(buf92, (s32)tel_dialog_disp_str9);
            strcat(buf92, date_name_set_tbl0[menu_csr]);
        }
        strcat(buf92, (s32)tel_dialog_disp_str10);
        sndi(buf92, 0, 13);
        xa_set_m((s32)tel_yoshio_name_xa_tbl, (s32)tel_yoshio_name_xa_tbl2, menu_csr, 13);
        k_sub_disp_start(0);
        step_mode += 1;
        goto Lexit;

    case 12:
        xa_wait_m();
        goto Lexit;

    case 13:
        k_sub_reset();
        gnsx(get_g_name_tbl0[1]);
        sndi(tel_dialog_disp_str11, 0, 13);
        sndi(tel_dialog_disp_str12, 1, 13);
        sndi(tel_dialog_disp_str13, 2, 13);
        xa_set_m((s32)0x19000064, (s32)0x0A00006E, -1, 13);
        k_sub_disp_start(0);
        step_mode += 1;
        goto Lexit;

    case 14:
        xa_wait_m();
        goto Lexit;

    case 15:
        goto_sub_sub_step(4);
        goto Lexit;

    case 32:
        xa_wait();
        goto Lexit;

    case 33:
        k_sub_reset();
        gnsx(get_g_name_tbl0[1]);
        sndi(tel_dialog_disp_str14, 0, 13);
        sndi(tel_dialog_disp_str15, 1, 13);
        xa_set_m((s32)0x1900006F, (s32)0x0B00007A, -1, 13);
        k_sub_disp_start(0);
        step_mode += 1;
        goto Lexit;

    case 34:
        xa_wait_m();
        goto Lexit;

    case 35:
        k_sub_reset();
        gnsx((s32)p_sei);
        sndi(tel_dialog_disp_str16, 0, 15);
        sndi(tel_dialog_disp_str17, 1, 15);
        k_sub_disp_start(0);
        step_mode += 1;
        goto Lexit;

    case 36:
        xa_wait();
        goto Lexit;

    case 37:
        k_sub_reset();
        gnsx(get_g_name_tbl0[1]);
        sndi(tel_dialog_disp_str18, 0, 13);
        sndi(tel_dialog_disp_str19, 1, 13);
        sndi(tel_dialog_disp_str20, 2, 13);
        xa_set_m((s32)0x120000C7, (s32)0x090000E5, -1, 13);
        k_sub_disp_start(0);
        step_mode += 1;
        goto Lexit;

    case 38:
        xa_wait_m();
        goto Lexit;

    case 39:
        goto_sub_sub_step(4);

    }

Lexit:
    p = ((S68 *)&class_w[97].active);
    for (i = 97; i != 130; i++) {
        if (p->f24 == 96) {
            v = rsin((step_counter + i) * 64);
            p->f28 = v / 3 + 2730;
            v = rsin((step_counter + i) * 90);
            p->f26 = v / 4 + 3072;
            v = rsin(step_counter * 64);
            p->f48 = v * 4;
            p->f3 |= 0x10;
        }
        p++;
    }
    appraisal_base_show();
}
