/* Address: 0x80066D38 */
/* Size: 1968 bytes (492 instructions) */
/* Region: Game (IDO 5.2) */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

typedef struct {
    u32 tag;
    u8  r0, g0, b0, code;
    s16 x0, y0;
    u8  u0, v0;
    u16 clut;
    s16 w, h;
} Sprt;

typedef struct {
    u32 r0 : 5;
    u32 g0 : 5;
    u32 b0 : 5;
    u32 x0 : 1;
    u32 r1 : 5;
    u32 g1 : 5;
    u32 b1 : 5;
    u32 x1 : 1;
} Pix2;

extern u8 byouki_kind;
extern u32 frame_counter;
extern u8 anim_fade;
extern volatile u8 anim_fade_v;
extern u8 update_anim_state_tbl2;
extern u8 update_anim_state_tbl3;
extern u8 update_anim_state_tbl4;
extern u8 screen_bri;
extern s32 activeBuff;
extern u8 ot_buff0[];
extern Pix2 update_anim_state_tbl0[4][8];
extern Pix2 update_anim_state_tbl1[8];

extern s32 get_season(s32);
extern s32 rand(void);
extern s16 rsin(s32);
extern s32 rcos(s32);
extern s32 *prim_alloc(s32, s32);
extern void SetSprt(u32 *);
extern void SetSemiTrans(u8 *, s32);
extern s32 GetClut(s32, s32);
extern void AddPrim(u32 *, u32 *);
extern void goto_tpage(s32, s32, s32, s32, s32);
extern void load_image_rect(s32, s32, s32, s32, void *);

void update_anim_state(s32 mode)
{
    Sprt *p;
    s32 i;
    s32 j;
    s32 x;
    s32 y;
    u32 v;
    s32 r;
    u32 fade;
    Pix2 *sa;
    s32 buf;
    Pix2 *sb;
    Pix2 *d;
    u32 c;
    u32 wa;
    u32 wb;
    u8 grid[10][8];
    s32 pad3;

    buf = get_season(mode);
    switch (byouki_kind & 0xF) {
    case 1:
        v = 1;
        if ((frame_counter & 3) == 3) {
            update_anim_state_tbl3 = update_anim_state_tbl3 - 1;
        }
        if ((frame_counter & 1) == 1) {
            update_anim_state_tbl2 = update_anim_state_tbl2 + 1;
        }
        break;
    case 2:
        v = 1;
        if ((frame_counter & 7) == 7) {
            update_anim_state_tbl3 = update_anim_state_tbl3 - 1;
        }
        if ((frame_counter & 3) == 3) {
            update_anim_state_tbl2 = update_anim_state_tbl2 + 1;
        }
        break;
    case 3:
        v = 1;
        if ((frame_counter & 0x15) == 0x15) {
            update_anim_state_tbl3 = update_anim_state_tbl3 - 1;
        }
        if ((frame_counter & 7) == 7) {
            update_anim_state_tbl2 = update_anim_state_tbl2 + 1;
        }
        break;
    case 4:
        v = 1;
        r = rand() & 0x1F;
        update_anim_state_tbl3 = update_anim_state_tbl3 + ((16 - r) << 0);
        r = rand() & 0x1F;
        update_anim_state_tbl2 = update_anim_state_tbl2 + ((16 - r) << 0);
        break;
    case 5:
        v = 1;
        r = rsin(frame_counter);
        update_anim_state_tbl3 = update_anim_state_tbl3 + r / 32;
        r = rcos(frame_counter);
        update_anim_state_tbl2 = update_anim_state_tbl2 + r / 32;
        break;
    case 0:
    default:
        v = 1;
        if ((frame_counter & 1) == 1) {
            update_anim_state_tbl3 = update_anim_state_tbl3 - 1;
        }
        update_anim_state_tbl2 = update_anim_state_tbl2 + 1;
        break;
    }
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 10; j++) {
            grid[j][i] = v;
        }
    }
    switch (mode) {
    case 0:
        break;
    case 1:
        for (i = 1; i < 6; i++) {
            for (j = 1; j < 9; j++) {
                grid[j][i] = 0;
            }
        }
        break;
    case 2:
        for (i = 0; i < 6; i++) {
            for (j = 0; j < 10; j++) {
                grid[j][i] = 0;
            }
        }
        grid[0][6] = 1;
        grid[9][6] = 1;
        grid[0][5] = 1;
        grid[9][5] = 1;
        break;
    }
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 10; j++) {
            if (grid[j][i]) {
                x = j * 32;
                y = i * 32 + 240;
                p = (Sprt *)prim_alloc(20, activeBuff);
                SetSprt((u32 *)p);
                SetSemiTrans((u8 *)p, 0);
                p->x0 = x;
                p->y0 = y - activeBuff * 240;
                p->u0 = (update_anim_state_tbl2 & 0x1F) + 128;
                p->v0 = (update_anim_state_tbl3 & 0x1F) + 128;
                p->h = 32;
                p->w = 32;
                p->r0 = screen_bri;
                p->g0 = screen_bri;
                p->b0 = screen_bri;
                p->clut = GetClut(160, 481);
                AddPrim((u32 *)(ot_buff0 + activeBuff * 1024 + update_anim_state_tbl4 * 4), (u32 *)p);
            }
        }
    }
    goto_tpage(960, 256, update_anim_state_tbl4, 0, 0);
    if ((frame_counter & 3) == 3) {
        fade = anim_fade_v;
        if (fade >= 239) {
            fade = (u8)(fade + 1);
            if (fade == 255) {
                anim_fade = fade;
                buf = buf + 1;
                load_image_rect(160, 481, 16, 1, update_anim_state_tbl0[(buf + 3) & 3]);
            } else {
                anim_fade_v = fade;
                if (fade >= 240) {
                    wb = fade - 239;
                    wa = 255 - fade;
                    sa = update_anim_state_tbl0[(buf + 3) & 3];
                    sb = update_anim_state_tbl0[(buf + 4) & 3];
                    d = update_anim_state_tbl1;
                    anim_fade_v = fade;
                    for (j = 0; j < 8; j++) {
                        c = (wa + 0) * sa->r0 + wb * sb->r0;
                        d->r0 = c >> 4;
                        c = (wa + 0) * sa->g0 + wb * sb->g0;
                        d->g0 = c >> 4;
                        c = (wa + 0) * sa->b0 + wb * sb->b0;
                        d->b0 = c >> 4;
                        c = (wa + 0) * sa->r1 + wb * sb->r1;
                        d->r1 = c >> 4;
                        c = (wa + 0) * sa->g1 + wb * sb->g1;
                        d->g1 = c >> 4;
                        c = (wa + 0) * sa->b1 + wb * sb->b1;
                        d->b1 = c >> 4;
                        sa++;
                        sb++;
                        d++;
                    }
                    load_image_rect(160, 481, 16, 1, update_anim_state_tbl1);
                }
            }
        }
    }
}
