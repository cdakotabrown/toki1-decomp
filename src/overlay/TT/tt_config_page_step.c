/* @SymbolStart: 0x8013CDC0 */
/* @CompileFlags: -Wc,-pic1 */
/* @SymbolSize: 672 */
/* Address: 0x8013CDC8, window 0x8013CDC0 (+8 absorbed cross-prologue head) */
/* Size: 672 bytes (168 instructions) including the head. */
#include "types.h"

typedef struct {
    u8  pad00[4];
    u16 f;
} Sub;

typedef struct {
    u8  pad00[16];
    Sub sub;
    u8  pad16[45];
    u8  unk43;
    u8  pad44[5];
    u8  unk49;
} Rec;

typedef struct {
    u16 unk00;
    u16 unk02;
    u16 unk04;
    u16 unk06;
    u8  pad08;
    u8  unk09;
    u8  unk0A;
    u8  unk0B;
} Widget;

typedef struct {
    u8  pad00[10];
    u8  page;
} Page;

typedef struct {
    u8  pad000[334];
    u16 unk14E;
    u8  pad150[18];
    u16 unk162;
} Status;

extern Rec    *tt_game_wk;
extern Widget *tt_step_wk;
extern Page   *tt_scroll_wk;
extern Status *tt_text_buf;
extern u8     *tt_db_buf;

extern void SetDefDispEnv(u8 *, s32, s32, s32, s32);
extern void tt_widget_y_set(void);

void tt_config_page_step(void)
{
    Rec *p;
    Widget *q;
    Page *r;
    Sub *v;
    s32 pad;
    u8 *fb;
    u8 *fb2;

    p = tt_game_wk;
    q = tt_step_wk;
    r = tt_scroll_wk;
    v = &p->sub;

    if ((p->sub.f & 0x1000) != 0) {
        r->page = r->page - 1;
        if ((s8)r->page < 0) {
            r->page = 3;
        }
    } else if ((v->f & 0x4000) != 0) {
        r->page = r->page + 1;
        if (r->page >= 4U) {
            r->page = 0;
        }
    }

    if ((v->f & 0x8000) != 0) {
        switch ((u32)r->page) {
        case 1:
            p->unk43 = 0;
            tt_text_buf->unk14E = 25;
            break;
        case 2:
            p->unk49 = 0;
            tt_text_buf->unk162 = 21;
            fb = tt_db_buf;
            SetDefDispEnv(fb + 0x80A4, 320, 0, 320, 240);
            SetDefDispEnv(fb + 0x1015C, 0, 0, 320, 240);
            break;
        }
    } else if ((v->f & 0x2000) != 0) {
        switch ((u32)r->page) {
        case 1:
            p->unk43 = 1;
            tt_text_buf->unk14E = 24;
            break;
        case 2:
            p->unk49 = 1;
            tt_text_buf->unk162 = 22;
            fb2 = tt_db_buf;
            SetDefDispEnv(fb2 + 0x80A4, 352, 0, 256, 240);
            SetDefDispEnv(fb2 + 0x1015C, 32, 0, 256, 240);
            break;
        }
    }

    if ((v->f & 0x820) != 0) {
        switch ((u32)r->page) {
        case 0:
            q->unk02 = 80;
            break;
        case 3:
            q->unk06 = 0;
            q->unk04 = 0;
            q->unk02 = 0;
            q->unk0B = 0;
            q->unk0A = 0;
            q->unk09 = 0;
            q->unk00 = 1;
            break;
        }
    }

    tt_widget_y_set();
}
