/* @SymbolSize: 304 */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* @CompileEnv: IDO52_AS1_TIE_TAKE=85 */
/* tt_pad_config_init - TT overlay, Address: 0x8013C8DC, true body 304 bytes */
#include "types.h"

extern void tt_widget_init2(void);
extern s32 tt_pad_config_icon_get(u8 a0, void *a1);

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
extern u16 tt_pad_label_tbl[3];

void tt_pad_config_init(void)
{
    u8 *saved;
    u8 *p;
    Widget *w;
    u16 *tbl;
    u16 *tblEnd;
    s32 x, y;
    s32 i;
    s32 val;

    saved = tt_game_wk;
    p = tt_scroll_wk;
    w = tt_text_buf;
    p[10] = 0;
    tt_widget_init2();

    w->flag = 1;
    w->x = -40;
    w->y = 48;
    w->val14 = 18;
    w->val15 = 15;
    w->field6 = 0;
    w->field16 = 0;
    w++;

    tblEnd = tt_pad_label_tbl + 3;
    tbl = tt_pad_label_tbl;
    x = -80;
    y = 96;
    for (; tbl != tblEnd; tbl++, y += 16, w++) {
        w->flag = 1;
        w->x = x;
        w->y = y;
        w->val14 = *tbl;
        w->val15 = 15;
        w->field6 = 0;
        w->field16 = 0;
    }

    w = tt_text_buf + 16;
    w->flag = 1;
    w->x = 48;
    w->y = 96;
    w->val14 = (s16)tt_pad_config_icon_get(saved[68], &w->val15);
    w->field6 = 0;
    w->field16 = 0;
    w++;
    w->flag = 1;
    w->x = 48;
    w->y = 112;
    w->val14 = (s16)tt_pad_config_icon_get(saved[69], &w->val15);
    w->field6 = 0;
    w->field16 = 0;
}
