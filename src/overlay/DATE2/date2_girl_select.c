/* @SymbolStart: 0x801330D4 */
/* @SymbolSize: 948 */
/* @RodataBase: 0x8013A100 */
/* @CompileEnv: IDO52_AS1_NO_FALLTHRU_FILL=0 IDO52_AS1_XBB_VOLATILE_OK=all */
/* @PostProc: patch_stack_complete --old 72 --new 80 */
/* @PostProc: remap_stack_slots --map 52:56 */
/* @PostProc: patch_insn_reg_field 0x58:rd:t8:t9 0x60:rs:t8:t9 0x60:rt:t9:t2 0x64:rt:t2:t8 0x74:rs:t9:t8 0x9c:rd:t8:t9 0xa0:rs:t8:t9 0xa0:rt:t9:t8 0xa8:rt:t9:t8 0xdc:rd:t8:t9 0xe0:rt:t8:t9 0xe4:rt:v0:t8 0xec:rd:t9:t3 0xec:rt:v0:t8 0xf0:rs:t9:t3 0x108:rd:t2:t4 0x10c:rt:t2:t4 0x11c:rt:v0:t7 0x120:rs:v0:t7 0x144:rt:t4:t9 0x148:rs:t4:t9 0x148:rt:t4:t9 0x15c:rt:t4:t9 */
/* date2_girl_select -- DATE2 overlay, Address: 0x801330D4 */
#include "types.h"

extern u8 date2_mode_no;
extern volatile u8 now_girl;
extern u8 girl_no;
extern u8 screen_w[];

extern s32 date2_girl_res_tbl[][11];



extern s32 date2_girl_ptr[];

void date2_girl_res_set(void);
void date2_scroll_add(void);
void sub_sub_step_up(void);
void event_log_add(s32 arg0, s32 arg1);
void date_name_set(s32 arg0);

void date2_girl_select(void)
{
    volatile s16 sums[10];
    s16 i;
    s16 j;
    s32 minVal;
    s32 flags;
    u8 sel;
    u8 saved;

    minVal = 100;

    date2_mode_no = 0;

    for (i = 1; i < 10; i++) {
        sums[i] = 0;
        for (j = 1; j < 37; j++) {
            sums[i] = sums[i] + screen_w[i * 56 + 464 + j];
        }
        sums[i] = sums[i] - screen_w[i * 56U + 490];
    }

    for (i = 1; i < 10; i++) {
        if (sums[i] < minVal) {
            flags = *(s32 *)&screen_w[i * 56U + 456];
            if ((flags << 30) < 0) {
                now_girl = (u8)(i | 0);
                minVal = sums[i];
            }
        }
    }

    sel = now_girl;
    girl_no = sel;
    date_name_set(sel);

    now_girl = 0;
    event_log_add(12, 0);

    saved = girl_no;
    now_girl = saved;
    event_log_add(12, 0);

    date2_girl_res_set();
    date2_scroll_add();
    sub_sub_step_up();
}

void date2_girl_res_set(void)
{
    switch (now_girl) {
    case 0:
        date2_girl_ptr[0] = date2_girl_res_tbl[0][1];
        date2_girl_ptr[1] = date2_girl_res_tbl[1][1];
        date2_girl_ptr[2] = date2_girl_res_tbl[2][1];
        break;
    case 1:
        date2_girl_ptr[0] = date2_girl_res_tbl[0][5];
        date2_girl_ptr[1] = date2_girl_res_tbl[1][5];
        date2_girl_ptr[2] = date2_girl_res_tbl[2][5];
        break;
    case 2:
        date2_girl_ptr[0] = date2_girl_res_tbl[0][2];
        date2_girl_ptr[1] = date2_girl_res_tbl[1][2];
        date2_girl_ptr[2] = date2_girl_res_tbl[2][2];
        break;
    case 3:
        date2_girl_ptr[0] = date2_girl_res_tbl[0][3];
        date2_girl_ptr[1] = date2_girl_res_tbl[1][3];
        date2_girl_ptr[2] = date2_girl_res_tbl[2][3];
        break;
    case 4:
        date2_girl_ptr[0] = date2_girl_res_tbl[0][9];
        date2_girl_ptr[1] = date2_girl_res_tbl[1][9];
        date2_girl_ptr[2] = date2_girl_res_tbl[2][9];
        break;
    case 5:
        date2_girl_ptr[0] = date2_girl_res_tbl[0][6];
        date2_girl_ptr[1] = date2_girl_res_tbl[1][6];
        date2_girl_ptr[2] = date2_girl_res_tbl[2][6];
        break;
    case 6:
        date2_girl_ptr[0] = date2_girl_res_tbl[0][7];
        date2_girl_ptr[1] = date2_girl_res_tbl[1][7];
        date2_girl_ptr[2] = date2_girl_res_tbl[2][7];
        break;
    case 7:
        date2_girl_ptr[0] = date2_girl_res_tbl[0][4];
        date2_girl_ptr[1] = date2_girl_res_tbl[1][4];
        date2_girl_ptr[2] = date2_girl_res_tbl[2][4];
        break;
    case 8:
        date2_girl_ptr[0] = date2_girl_res_tbl[0][0];
        date2_girl_ptr[1] = date2_girl_res_tbl[1][0];
        date2_girl_ptr[2] = date2_girl_res_tbl[2][0];
        break;
    case 9:
        date2_girl_ptr[0] = date2_girl_res_tbl[0][8];
        date2_girl_ptr[1] = date2_girl_res_tbl[1][8];
        date2_girl_ptr[2] = date2_girl_res_tbl[2][8];
        break;
    }
}
