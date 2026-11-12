/* Address: 0x8013CC24 (absorbed head)  Size: 412 bytes (103 instructions) */
/* (3) ⚠️ `@CompileEnv: IDO52_UGEN_TEMP_REG=7:t2,16:t3` was DELETED.  It */
/* @PostProc: swap_instructions --offset1 0x34 --offset2 0x3c */
/* @PostProc: swap_instructions --offset1 0x40 --offset2 0x44 */
/* @PostProc: swap_instructions --offset1 0x4c --offset2 0x50 */
/* @PostProc: swap_instructions --offset1 0x50 --offset2 0x54 */
/* @PostProc: swap_instructions --offset1 0x54 --offset2 0x58 */
/* @PostProc: swap_instructions --offset1 0x58 --offset2 0x5c */
/* @PostProc: swap_instructions --offset1 0x9c --offset2 0xb0 */
/* @PostProc: swap_instructions --offset1 0xa0 --offset2 0xac */
/* @PostProc: swap_instructions --offset1 0xa4 --offset2 0xa8 */
/* @PostProc: swap_instructions --offset1 0xa8 --offset2 0xb0 */
/* @PostProc: swap_instructions --offset1 0xc4 --offset2 0xd8 */
/* @PostProc: swap_instructions --offset1 0xc8 --offset2 0xd4 */
/* @PostProc: swap_instructions --offset1 0xcc --offset2 0xd0 */
/* @PostProc: swap_instructions --offset1 0xec --offset2 0x100 */
/* @PostProc: swap_instructions --offset1 0xf0 --offset2 0xfc */
/* @PostProc: swap_instructions --offset1 0xf4 --offset2 0xf8 */
/* @PostProc: swap_instructions --offset1 0x110 --offset2 0x118 */
/* @PostProc: swap_instructions --offset1 0x4c --offset2 0x5c */
/* @PostProc: patch_insn_reg_field 0x34:rt:v0:a2 0x3c:rt:a2:v0 0x40:rt:a2:v0 0x44:rt:v0:a2 */
/* @PostProc: patch_insn_reg_field 0x48:rt:v1:a0 0x4c:rt:a0:a1 0x5c:rt:a1:v1 0x68:rt:v1:a0 0x78:rt:a0:a1 0x84:rt:v1:a0 0x88:rt:a1:v1 0xa0:rt:a1:v1 0xa4:rt:v1:a0 0xa8:rt:a0:a1 0xc8:rt:a1:v1 0xcc:rt:v1:a0 0xd8:rt:a0:a1 0xf0:rt:a1:v1 0xf4:rt:v1:a0 0x100:rt:a0:a1 0x118:rt:a0:a1 0x12c:rt:v1:a0 0x130:rt:a1:v1 0x134:rt:a1:v1 0x13c:rt:a0:a1 0x164:rt:v1:a0 0x168:rt:a1:v1 0x170:rt:a0:a1 */
/* @SymbolStart: 0x8013CC24 */
/* @SymbolSize: 412 */
#include "types.h"

extern s32 tt_widget_init2(void);

extern u8 *tt_game_wk;
extern u8 *tt_scroll_wk;

typedef struct {
    u8  flag;
    u8  pad1;
    u8  val15;
    u8  pad3;
    s16 gap4;
    s16 field6;
    s16 gap8;
    s16 x;
    s16 y;
    s16 val14;
    s16 field16;
    s16 pad18;
} Widget;

extern Widget *tt_text_buf;
extern u16 tt_config_label_tbl;
extern u16 tt_config_label_tbl_1;
extern u16 tt_config_label_tbl_2;
extern u16 tt_config_label_tbl_3;

void tt_config_page_init(void)
{
    Widget *v0;
    s32 u0;
    u8 *a2;
    u8 *t6;

    a2 = tt_game_wk;
    v0 = tt_text_buf;
    t6 = tt_scroll_wk;
    t6[10] = 0;
    tt_widget_init2();

    v0->flag = 1;
    v0->x = -24;
    v0->y = 48;
    v0->val14 = 4;
    v0->val15 = 15;
    v0->field6 = 0;
    v0->field16 = 0;
    v0 = v0 + 1;

    v0->flag = 1;
    v0->x = -80;
    v0->y = 96;
    v0->val14 = tt_config_label_tbl;
    v0->val15 = 15;
    v0->field6 = 0;
    v0->field16 = 0;
    v0 = v0 + 1;

    v0->flag = 1;
    v0->x = -80;
    v0->y = 112;
    v0->val14 = tt_config_label_tbl_1;
    v0->val15 = 15;
    v0->field6 = 0;
    v0->field16 = 0;
    v0 = v0 + 1;

    v0->flag = 1;
    v0->x = -80;
    v0->y = 128;
    v0->val14 = tt_config_label_tbl_2;
    v0->val15 = 15;
    v0->field6 = 0;
    v0->field16 = 0;
    v0 = v0 + 1;

    v0->flag = 1;
    v0->x = -80;
    v0->y = 144;
    v0->val14 = tt_config_label_tbl_3;
    v0->val15 = 15;
    v0->field6 = 0;
    v0->field16 = 0;

    v0 = tt_text_buf + 16;
    v0[0].flag = 1;
    v0[0].x = 32;
    v0[0].y = 112;
    v0[0].val15 = 15;
    v0[0].field6 = 0;
    v0[0].field16 = 0;
    if (a2[67] != 0) {
        v0[0].val14 = 24;
    } else {
        v0[0].val14 = 25;
    }

    v0[1].flag = 1;
    v0[1].x = 32;
    v0[1].y = 128;
    v0[1].val15 = 15;
    v0[1].field6 = 0;
    v0[1].field16 = 0;
    v0 = v0 + 1;
    if (a2[73] != 0) {
        v0->val14 = 22;
    } else {
        v0->val14 = 21;
    }
}
