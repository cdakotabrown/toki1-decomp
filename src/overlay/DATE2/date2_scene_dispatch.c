/* @SymbolStart: 0x80135098 */
/* @RodataBase: 0x8013A1F8 */
/* @CompileEnv: IDO52_COPYPROP_SITE=lod:85:deny,lod:86:deny,lod:87:deny,lod:88:deny,lod:89:deny,lod:90:deny,lod:91:deny,lod:92:deny,lod:93:deny,lod:94:deny,lod:95:deny,lod:96:deny,lod:97:deny IDO52_FORCE_WEB_COLOR=0:a3,157:v1 IDO52_COPYPROP_VAR=-16 IDO52_UGEN_DEST_TEMP=243 */
/* @PostProc: swap_instructions --offset1 0xb64 --offset2 0xb68 */
/* @PostProc: swap_instructions --offset1 0xb78 --offset2 0xb7c */
/* @PostProc: swap_instructions --offset1 0xb9c --offset2 0xba0 */
/* @PostProc: swap_instructions --offset1 0xbb0 --offset2 0xbb8 */
/* @PostProc: swap_instructions --offset1 0xc58 --offset2 0xc5c */
/* @PostProc: swap_instructions --offset1 0xcac --offset2 0xcb4 */
/* @PostProc: patch_insn_reg_field 0x8e4:rt:a0:a1 0x8e8:rt:v1:a0 0x8ec:rs:a0:a1 0x8f4:rs:a0:a1 0x8f8:rs:v1:a0 0x8f8:rt:v1:a0 0x900:rs:v1:a0 0xa74:rt:v1:a0 0xa78:rs:v1:a0 0xa78:rt:v1:a0 0xa80:rs:v1:a0 0xa80:rt:v1:a0 0xa84:rs:v1:a0 0xa84:rt:t7:v1 0xa88:rt:t7:v1 0xa90:rt:t7:v1 0xa98:rt:t7:v1 0xaa0:rt:t7:v1 0xaa8:rt:t7:v1 0xab0:rt:t7:v1 0xab8:rt:t7:v1 0xac0:rt:t7:v1 0xac8:rt:t7:v1 0xad0:rt:t7:v1 0xad8:rt:t7:v1 0xae0:rt:t7:v1 0xae4:rs:t7:v1 0xb7c:rt:v0:a3 0xb8c:rt:v0:a3 0xba0:rs:v0:a3 0xba0:rt:v0:a3 0xba8:rt:v0:a3 0xbc4:rt:v0:a3 0xbd8:rs:v0:a3 0xbd8:rt:v0:a3 0xbe0:rt:v0:a3 0xcc0:rt:v0:t8 0xcd4:rs:v0:t8 0xcd4:rt:t8:t6 0xcdc:rt:t8:t6 */
/* Address: 0x80135098 (head) / 0x801350A8, Size: 3500 bytes (875 insns) */
/* @SymbolSize: 3500 */
#include "types.h"

typedef struct {
    s32 attr;
    s16 x;
    s16 y;
    s16 u;
    u16 v;
    s16 clut;
    s8 b0E;
    u8 b0F;
    s16 h10;
    s16 h12;
    s8 b14;
    s8 b15;
    s8 b16;
    s8 pad17;
    s16 h18;
    s16 h1A;
    s16 scaleX;
    s16 scaleY;
    s32 w20;
} DateNode;

typedef struct {
    u8 pad[0x90];
    DateNode nodes[4];
} DateScene;

typedef struct {
    s16 x;
    s16 y;
    s16 w;
    s16 h;
} DateRect;

extern volatile s32 step_counter;
extern u8 cdq_footer_chk;
extern volatile u8 now_sub_sub_step;
extern s32 script_on;
extern s16 date2_intro_script_tbl0[];
extern s16 date2_intro_script_tbl1[];
extern u8 bg_change_tbl0;
extern u8 bg_change_tbl1;
extern u8 dec_bg_show_tbl0;
extern s32 cd_wait_timeout_tbl0;
extern u16 date2_wait_cnt;
extern volatile u16 date2_wait_cnt_v;
extern DateScene Sprites;
extern u8 screen_w[];
extern u8 sprite4_r;
extern u8 sprite4_g;
extern u8 sprite4_b;
extern u8 sprite5_r;
extern u8 sprite5_g;
extern u8 sprite5_b;
extern u8 sprite6_r;
extern u8 sprite6_g;
extern u8 sprite6_b;
extern u8 sprite7_r;
extern u8 sprite7_g;
extern u8 sprite7_b;
extern s16 sprite4_y;
extern s16 sprite5_y;
extern u16 sprite4_h;
extern u16 sprite5_h;
extern u16 sprite6_h;
extern u16 sprite7_h;
extern u8 sprite6_v;
extern u8 sprite7_v;
extern u32 *mdec_bs;
extern u8 date2_scene_dispatch_str0[];
extern s16 date_effect_init_tbl0[];

extern void sub_sub_step_up(void);
extern s32 get_CD_State2(void);
extern void device_cmd_send(s32);
extern void load_image_rect(s32, s32, s32, s32, u32);
extern void cd_read_start(s32, u32, s32);
extern void decdct(s32, s32, s32, s32);
extern void indexed_byte_toggle(s32);
extern void value_cache_find_or_load(s32, s32);
extern void dec_bg_init(s32);
extern void hizuke_disp_switch(s32);
extern void zyotai_sprite_set_chk(void);
extern void girl_season_load(void);
extern void sprite_rgb_set(s16, s32);
extern void StoreImage(DateRect *, u32);
extern void MoveImage(DateRect *, s32, s32);
extern void printf(u8 *);
extern void date2_girl_serifu_load(void);

static s32 date2_wipe_step();

s32 date2_scene_dispatch(void)
{
    s32 idx;
    s16 i;
    u32 oldA;
    s32 cur;
    DateRect r;
    u32 newA;
    u8 lvl;
    s32 cnt;
    u32 *p;

    idx = step_counter;
    cdq_footer_chk = 0;

    switch (idx) {
    case 0:
        device_cmd_send(207);
        script_on = 1;
        date2_intro_script_tbl1[1] = 0;
        date2_intro_script_tbl0[1] = 0;
        date2_intro_script_tbl1[0] = 0;
        date2_intro_script_tbl0[0] = 0;
        bg_change_tbl1 = 0;
        bg_change_tbl0 = 0;
        cd_wait_timeout_tbl0 = -1;
        date_effect_init_tbl0[1] = 0;
        dec_bg_show_tbl0 = 0;
        hizuke_disp_switch(0);
        date2_wait_cnt = 0;
        for (i = 0; i < 2; i++) {
            Sprites.nodes[i].u = i * 64 + 128;
            Sprites.nodes[i].attr = (s32)0x82000000;
            Sprites.nodes[i].v = 160;
            Sprites.nodes[i].b0E = 0;
            Sprites.nodes[i].b0F = 0;
            Sprites.nodes[i].h10 = 0;
            Sprites.nodes[i].clut = i * 2 + 21;
            Sprites.nodes[i].h12 = 480;
            Sprites.nodes[i].b14 = 0;
            Sprites.nodes[i].b15 = 0;
            Sprites.nodes[i].b16 = 0;
            Sprites.nodes[i].h18 = 0;
            Sprites.nodes[i].h1A = 0;
            Sprites.nodes[i].scaleX = 4096;
            Sprites.nodes[i].scaleY = 4096;
            Sprites.nodes[i].w20 = 0;
            Sprites.nodes[i].x = i * 128 - 160;
            Sprites.nodes[i].y = -120;
            screen_w[i + 4265] = 11;
        }
        for (i = 0; i < 2; i++) {
            Sprites.nodes[i + 2].u = i * 64 + 128;
            Sprites.nodes[i + 2].attr = (s32)0x82000000;
            Sprites.nodes[i + 2].v = 0;
            Sprites.nodes[i + 2].b0E = 0;
            Sprites.nodes[i + 2].b0F = 160;
            Sprites.nodes[i + 2].h10 = 0;
            Sprites.nodes[i + 2].clut = i * 2 + 10;
            Sprites.nodes[i + 2].h12 = 480;
            Sprites.nodes[i + 2].b14 = 0;
            Sprites.nodes[i + 2].b15 = 0;
            Sprites.nodes[i + 2].b16 = 0;
            Sprites.nodes[i + 2].h18 = 0;
            Sprites.nodes[i + 2].h1A = 0;
            Sprites.nodes[i + 2].scaleX = 4096;
            Sprites.nodes[i + 2].scaleY = 4096;
            Sprites.nodes[i + 2].w20 = 0;
            Sprites.nodes[i + 2].x = i * 128 - 160;
            Sprites.nodes[i + 2].y = -120;
            screen_w[i + 4267] = 11;
        }
        value_cache_find_or_load(17329, 1);
        idx = step_counter;
        idx += 1;
        step_counter = idx;
        return;
    case 1:
    case 4:
    case 7:
    case 10:
    case 13:
    case 16:
    case 31:
    case 34:
    case 37:
        if (get_CD_State2() & 1) {
            idx = step_counter;
            idx += 1;
            step_counter = idx;
        }
        return;
    case 2:
        decdct(320, 256, 320, 160);
        r.x = 320;
        r.y = 256;
        r.w = 320;
        r.h = 160;
        StoreImage(&r, 0x80183000U);
        idx = step_counter;
        idx += 1;
        step_counter = idx;
        return;
    case 3:
        value_cache_find_or_load(17322, 1);
        idx = step_counter;
        idx += 1;
        step_counter = idx;
        return;
    case 5:
        decdct(320, 256, 320, 160);
        r.x = 320;
        r.y = 256;
        r.w = 320;
        r.h = 160;
        StoreImage(&r, 0x8019C000U);
        idx = step_counter;
        idx += 1;
        step_counter = idx;
        return;
    case 6:
        value_cache_find_or_load(17315, 1);
        idx = step_counter;
        idx += 1;
        step_counter = idx;
        return;
    case 8:
        decdct(320, 256, 320, 160);
        r.x = 320;
        r.y = 256;
        r.w = 320;
        r.h = 160;
        StoreImage(&r, 0x801B5000U);
        idx = step_counter;
        idx += 1;
        step_counter = idx;
        return;
    case 9:
        value_cache_find_or_load(17308, 1);
        idx = step_counter;
        idx += 1;
        step_counter = idx;
        return;
    case 11:
        decdct(320, 256, 320, 160);
        r.x = 320;
        r.y = 256;
        r.w = 320;
        r.h = 160;
        StoreImage(&r, 0x801CE000U);
        idx = step_counter;
        idx += 1;
        step_counter = idx;
        return;
    case 12:
        value_cache_find_or_load(17301, 1);
        idx = step_counter;
        idx += 1;
        step_counter = idx;
        return;
    case 14:
        decdct(640, 0, 320, 160);
        idx = step_counter;
        idx += 1;
        step_counter = idx;
        return;
    case 15:
        value_cache_find_or_load(17293, 1);
        idx = step_counter;
        idx += 1;
        step_counter = idx;
        return;
    case 17:
        decdct(320, 256, 320, 160);
        idx = step_counter;
        idx += 1;
        step_counter = idx;
        return;
    case 18:
        for (i = 0; i < 4; i++) {
            Sprites.nodes[i].attr &= 0x7FFFFFFF;
        }
        idx += 1;
        step_counter = idx;
        return;
    case 19:
        for (i = 4; i < 8; i++) {
            sprite_rgb_set(i, 128);
        }
        for (i = 0; i < 2; i++) {
            Sprites.nodes[i].attr = 0x02000000;
            Sprites.nodes[i].v = 160;
            Sprites.nodes[i].b0F = 0;
            Sprites.nodes[i].y = -120;
        }
        for (i = 0; i < 2; i++) {
            Sprites.nodes[i + 2].attr = 0x02000000;
            Sprites.nodes[i + 2].v = 0;
            Sprites.nodes[i + 2].b0F = 160;
            Sprites.nodes[i + 2].y = -120;
        }
        date2_wait_cnt = 0;
        device_cmd_send(1280);
        idx = step_counter;
        idx += 1;
        step_counter = idx;
        return;
    case 20:
    case 22:
    case 24:
        date2_wipe_step(8);
        date2_wait_cnt += 1;
        if (date2_wait_cnt != 20) {
            return;
        }
        idx = step_counter;
        idx += 1;
        step_counter = idx;
        return;
    case 21:
        r.x = 640;
        r.y = 0;
        r.w = 320;
        r.h = 160;
        MoveImage(&r, 320, 256);
        load_image_rect(640, 0, 320, 160, 0x801CE000U);
        for (i = 0; i < 2; i++) {
            Sprites.nodes[i].v = 160;
            Sprites.nodes[i].b0F = 0;
            Sprites.nodes[i].y = -120;
        }
        for (i = 0; i < 2; i++) {
            Sprites.nodes[i + 2].v = 0;
            Sprites.nodes[i + 2].b0F = 160;
            Sprites.nodes[i + 2].y = -120;
        }
        idx = step_counter;
        date2_wait_cnt_v = 0;
        idx += 1;
        step_counter = idx;
        return;
    case 23:
        r.x = 640;
        r.y = 0;
        r.w = 320;
        r.h = 160;
        MoveImage(&r, 320, 256);
        load_image_rect(640, 0, 320, 160, 0x801B5000U);
        for (i = 0; i < 2; i++) {
            Sprites.nodes[i].v = 160;
            Sprites.nodes[i].b0F = 0;
            Sprites.nodes[i].y = -120;
        }
        for (i = 0; i < 2; i++) {
            Sprites.nodes[i + 2].v = 0;
            Sprites.nodes[i + 2].b0F = 160;
            Sprites.nodes[i + 2].y = -120;
        }
        idx = step_counter;
        date2_wait_cnt_v = 0;
        idx += 1;
        step_counter = idx;
        return;
    case 25:
        r.x = 640;
        r.y = 0;
        r.w = 320;
        r.h = 160;
        MoveImage(&r, 320, 256);
        load_image_rect(640, 0, 320, 160, 0x8019C000U);
        for (i = 0; i < 2; i++) {
            Sprites.nodes[i].v = 160;
            Sprites.nodes[i].b0F = 0;
            Sprites.nodes[i].y = -120;
        }
        for (i = 0; i < 2; i++) {
            Sprites.nodes[i + 2].v = 0;
            Sprites.nodes[i + 2].b0F = 160;
            Sprites.nodes[i + 2].y = -120;
        }
        idx = step_counter;
        date2_wait_cnt_v = 0;
        idx += 1;
        step_counter = idx;
        return;
    case 26:
        date2_wipe_step(8);
        cnt = date2_wait_cnt;
        if ((u32)cnt >= 8) {
            lvl = sprite4_r - 4;
            sprite7_b = lvl;
            sprite7_g = lvl;
            sprite7_r = lvl;
            sprite6_b = lvl;
            sprite6_g = lvl;
            sprite6_r = lvl;
            sprite5_b = lvl;
            sprite5_g = lvl;
            sprite5_r = lvl;
            sprite4_b = lvl;
            sprite4_g = lvl;
            sprite4_r = lvl;
        }
        date2_wait_cnt = cnt + 1;
        if (date2_wait_cnt != 20) {
            return;
        }
        idx = step_counter;
        idx += 1;
        step_counter = idx;
        return;
    case 27:
        r.x = 640;
        r.y = 0;
        r.w = 320;
        r.h = 160;
        MoveImage(&r, 320, 256);
        load_image_rect(640, 0, 320, 160, 0x80183000U);
        for (i = 0; i < 2; i++) {
            Sprites.nodes[i].v = 160;
            Sprites.nodes[i].b0F = 0;
            Sprites.nodes[i].y = -120;
        }
        for (i = 0; i < 2; i++) {
            Sprites.nodes[i + 2].v = 0;
            Sprites.nodes[i + 2].b0F = 160;
            Sprites.nodes[i + 2].y = -120;
        }
        idx = step_counter;
        date2_wait_cnt_v = 0;
        idx += 1;
        step_counter = idx;
        return;
    case 28:
        date2_wipe_step(8);
        lvl = (u8)(sprite4_r - 4);
        sprite7_b = lvl;
        sprite7_g = lvl;
        sprite7_r = lvl;
        sprite6_b = lvl;
        sprite6_g = lvl;
        sprite6_r = lvl;
        sprite5_b = lvl;
        sprite5_g = lvl;
        sprite5_r = lvl;
        sprite4_b = lvl;
        sprite4_g = lvl;
        sprite4_r = lvl;
        if ((u32)lvl < 4) {
            idx = step_counter;
            idx += 1;
            step_counter = idx;
        }
        return;
    case 29:
        for (i = 0; i < 4; i++) {
            Sprites.nodes[i].attr |= (s32)0x80000000;
        }
        idx += 1;
        step_counter = idx;
        return;
    case 30:
        cur = idx;
        oldA = now_sub_sub_step;
        step_counter = cur;
        girl_season_load();
        newA = now_sub_sub_step;
        step_counter = cur;
        if (oldA == newA) {
            return;
        }
        now_sub_sub_step = newA - 1;
        cur += 1;
        step_counter = cur;
        return;
    case 32:
        cur = idx;
        step_counter = cur;
        zyotai_sprite_set_chk();
        now_sub_sub_step -= 1;
        cur += 1;
        step_counter = cur;
        return;
    case 33:
        cd_read_start(10, 0x80162000U, 17214);
        idx = step_counter;
        idx += 1;
        step_counter = idx;
        return;
    case 35:
        p = mdec_bs;
        if ((p[0] & 0xFFFF0000) == 0x38000000) {
            if ((p[1] & 0xFF010000) == 0x00010000) {
                goto Lthen35;
            }
        }
        printf(date2_scene_dispatch_str0);
        date2_intro_script_tbl1[0] = 0;
        date2_intro_script_tbl1[1] = 0;
        idx = step_counter;
        idx -= 2;
        step_counter = idx;
        return 0;
    Lthen35:
        dec_bg_init(1);
        indexed_byte_toggle(1);
        idx = step_counter;
        idx += 1;
        step_counter = idx;
        return;
    case 36:
        cur = idx;
        step_counter = cur;
        date2_girl_serifu_load();
        now_sub_sub_step -= 1;
        step_counter = cur + 1;
        return;
    default:
        break;
    }

    sub_sub_step_up();
    script_on = 0;
    device_cmd_send(512);
    hizuke_disp_switch(1);
}

static s32 date2_wipe_step(dx)
s16 dx;
{
    Sprites.nodes[0].y += dx;
    Sprites.nodes[1].y += dx;
    Sprites.nodes[0].v -= dx;
    Sprites.nodes[1].v -= dx;
    Sprites.nodes[2].v += dx;
    Sprites.nodes[3].v += dx;
    Sprites.nodes[2].b0F -= dx;
    Sprites.nodes[3].b0F -= dx;
    return dx;
}
