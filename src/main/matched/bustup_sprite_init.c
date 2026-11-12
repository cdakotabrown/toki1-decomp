/* Address: 0x80062F5C */
/* Size: 1856 bytes (464 instructions) */
/* Region: Game (0x80041000-0x80086000) */
/* Compiler: IDO 5.2 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: patch_stack_complete --old 64 --new 72 */
/* @PostProc: remap_stack_slots --map '72:80,76:84,80:88' --keep-frame */
#include "types.h"

typedef struct {
    u32 tag;
    s16 x;
    s16 y;
    s16 w;
    s16 h;
    s16 pad0C;
    u8  f0E;
    u8  f0F;
    s16 f10;
    s16 f12;
    u8  r;
    u8  g;
    u8  b;
    u8  pad17;
    s16 f18;
    s16 f1A;
    s16 f1C;
    s16 f1E;
    u32 f20;
} Spr;

extern Spr Sprites[];
extern u8  sprite_state[];

extern s16 bustup2_cw_x;
extern s16 bustup2_cw_y;

extern u8  bustup_cw_prio;
extern u8  bustup_cw_mode;
extern u8  bustup_cw_disp;
extern u8  bustup_cw_rate;
extern u8  bustup_cw_bright;
extern s32 bustup_cw_img;
extern s32 bustup_cw_anim_tbl;
extern s16 bustup_cw_anim;
extern s16 bustup_cw_y;
extern s32 bustup_cw_hdr;
extern u32 bustup_cw_attr;
extern u8  bustup_cw_clut;
extern u8  bustup2_cw_prio;
extern u8  bustup2_cw_mode;
extern u8  bustup2_cw_disp;
extern u8  bustup2_cw_rate;
extern u8  bustup2_cw_bright;
extern s32 bustup2_cw_img;
extern s32 bustup2_cw_anim_tbl;
extern s16 bustup2_cw_anim;
extern s32 bustup2_cw_hdr;
extern u32 bustup2_cw_attr;
extern u8  bustup2_cw_clut;

extern void palette_upload_enqueue(s32 a, s32 b, s32 c, s32 d, s32 e);
extern void load_tpage_buf_lock(s32 slot, s32 val, s32 flag);
extern void class_clear_n(s32 idx);
extern void bustup_timer_clamp(s32 arg0);
extern void bustup_face_set(s32 arg0);

void bustup_sprite_init(s32 arg0, s32 *arg1, s16 **arg2, s32 arg3, s32 arg4)
{
    s16 *p;
    s16 dx, dy, x0, y0, x1, y1, w1, h0, h1, e0;

    palette_upload_enqueue(arg0, 16, 1, 2, 0);
    load_tpage_buf_lock(5, arg1[0], 1);
    load_tpage_buf_lock(6, arg1[1], 1);
    load_tpage_buf_lock(7, arg1[2], 1);
    load_tpage_buf_lock(8, arg1[3], 1);
    load_tpage_buf_lock(9, arg1[4], 1);

    class_clear_n(31);
    bustup2_cw_prio = 12;
    bustup2_cw_mode = 0;
    bustup2_cw_attr = 0xC1000000;
    bustup2_cw_disp = 140;
    bustup2_cw_img = (s32)arg2;
    bustup2_cw_anim_tbl = arg3;
    bustup2_cw_hdr = (s32)arg1;
    bustup2_cw_anim = 0;
    bustup2_cw_rate = 2;
    bustup2_cw_clut = 16;
    bustup2_cw_y = -32;
    bustup2_cw_bright = 0;

    class_clear_n(30);
    bustup_cw_prio = 12;
    bustup_cw_mode = 0;
    bustup_cw_attr = 0xC1000000;
    bustup_cw_disp = 140;
    bustup_cw_img = (s32)arg2;
    bustup_cw_anim_tbl = arg3;
    bustup_cw_hdr = (s32)arg1;
    bustup_cw_anim = 1;
    bustup_cw_rate = 2;
    bustup_cw_clut = 16;
    bustup_cw_y = -32;
    bustup_cw_bright = 0;

    p = *arg2;

    if (p[0] >= 6) {
        dx = p[1];
        dy = p[2];
        x0 = p[39];
        y0 = p[40];
        x1 = p[32];
        y1 = p[33];
        Sprites[0].tag = 0x81000000;
        Sprites[0].w = 16;
        Sprites[0].h = 16;
        Sprites[0].f0E = 48;
        Sprites[0].f0F = 0;
        Sprites[0].f10 = 0;
        Sprites[0].f12 = 496;
        Sprites[0].r = 128;
        Sprites[0].g = 128;
        Sprites[0].b = 128;
        Sprites[0].f18 = 0;
        Sprites[0].f1A = 0;
        Sprites[0].f1C = 4096;
        Sprites[0].f1E = 4096;
        Sprites[0].f20 = 0;
        Sprites[0].x = (s16)((bustup2_cw_x + x0) - dx);
        Sprites[0].y = (s16)((bustup2_cw_y + y0) - dy);
        sprite_state[0] = 11;
        Sprites[1].tag = 0x81000000;
        Sprites[1].w = 48;
        Sprites[1].h = 32;
        Sprites[1].f0E = 0;
        Sprites[1].f0F = 0;
        Sprites[1].f10 = 0;
        Sprites[1].f12 = 496;
        Sprites[1].r = 128;
        Sprites[1].g = 128;
        Sprites[1].b = 128;
        Sprites[1].f18 = 0;
        Sprites[1].f1A = 0;
        Sprites[1].f1C = 4096;
        Sprites[1].f1E = 4096;
        Sprites[1].f20 = 0;
        Sprites[1].x = (s16)((bustup2_cw_x + x1) - dx);
        Sprites[1].y = (s16)((bustup2_cw_y + y1) - dy);
        sprite_state[1] = 11;
    }
    if (p[0] >= 4) {
        x0 = p[18];
        y0 = p[19];
        h0 = p[23];
        e0 = p[24];
        x1 = p[25];
        y1 = p[26];
        w1 = p[30];
        h1 = p[31];
        Sprites[2].tag = 0xC1000000;
        Sprites[2].w = 16;
        Sprites[2].h = h0;
        Sprites[2].f0E = (u8)e0;
        Sprites[2].f0F = 0;
        Sprites[2].f10 = 0;
        Sprites[2].f12 = 496;
        Sprites[2].r = 128;
        Sprites[2].g = 128;
        Sprites[2].b = 128;
        Sprites[2].f18 = 0;
        Sprites[2].f1A = 0;
        Sprites[2].f1C = 4096;
        Sprites[2].f1E = 4096;
        Sprites[2].f20 = 0;
        Sprites[2].x = (s16)((bustup2_cw_x + x0) - dx);
        Sprites[2].y = (s16)((bustup2_cw_y + y0) - dy);
        sprite_state[2] = 139;
        Sprites[3].tag = 0xC1000000;
        Sprites[3].w = w1;
        Sprites[3].h = h1;
        Sprites[3].f0E = 0;
        Sprites[3].f0F = 0;
        Sprites[3].f10 = 0;
        Sprites[3].f12 = 496;
        Sprites[3].r = 128;
        Sprites[3].g = 128;
        Sprites[3].b = 128;
        Sprites[3].f18 = 0;
        Sprites[3].f1A = 0;
        Sprites[3].f1C = 4096;
        Sprites[3].f1E = 4096;
        Sprites[3].f20 = 0;
        Sprites[3].x = (s16)((bustup2_cw_x + x1) - dx);
        Sprites[3].y = (s16)((bustup2_cw_y + y1) - dy);
        sprite_state[3] = 139;
    }
    if (p[0] >= 7) {
        x0 = p[46];
        y0 = p[47];
        h0 = p[51];
        e0 = p[52];
        Sprites[4].tag = 0xC1000000;
        Sprites[4].w = 16;
        Sprites[4].h = h0;
        Sprites[4].f0E = (u8)e0;
        Sprites[4].f0F = 0;
        Sprites[4].f10 = 0;
        Sprites[4].f12 = 496;
        Sprites[4].r = 128;
        Sprites[4].g = 128;
        Sprites[4].b = 128;
        Sprites[4].f18 = 0;
        Sprites[4].f1A = 0;
        Sprites[4].f1C = 4096;
        Sprites[4].f1E = 4096;
        Sprites[4].f20 = 0;
        Sprites[4].x = (s16)((bustup2_cw_x + x0) - dx);
        Sprites[4].y = (s16)((bustup2_cw_y + y0) - dy);
        sprite_state[4] = 139;
    }
    if (p[0] == 8) {
        x1 = p[53];
        y1 = p[54];
        w1 = p[58];
        h1 = p[59];
        Sprites[5].tag = 0xC1000000;
        Sprites[5].w = w1;
        Sprites[5].h = h1;
        Sprites[5].f0E = 0;
        Sprites[5].f0F = 0;
        Sprites[5].f10 = 0;
        Sprites[5].f12 = 496;
        Sprites[5].r = 128;
        Sprites[5].g = 128;
        Sprites[5].b = 128;
        Sprites[5].f18 = 0;
        Sprites[5].f1A = 0;
        Sprites[5].f1C = 4096;
        Sprites[5].f1E = 4096;
        Sprites[5].f20 = 0;
        Sprites[5].x = (s16)((bustup2_cw_x + x1) - dx);
        Sprites[5].y = (s16)((bustup2_cw_y + y1) - dy);
        sprite_state[5] = 139;
    }

    bustup_face_set(arg4);
    bustup_timer_clamp(arg4);
}
