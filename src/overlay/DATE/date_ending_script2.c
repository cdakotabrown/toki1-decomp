/* @CompileEnv: IDO52_COPYPROP_SITE=lod:80:deny IDO52_FORCE_WEB_COLOR=0:a3 */
/* @SymbolStart: 0x801470FC */
/* Address: 0x8014710C */
/* Size: 1980 bytes (495 insns) */
/* @SymbolSize: 1996 */
/* @CompileFlags: -Wo,-loopunroll,0 -Wo,-notail */
/* @RodataBase: 0x8015B218 */
/* @PostProc: swap_instructions --offset1 0xC0 --offset2 0xC8 */
/* @PostProc: swap_instructions --offset1 0xC4 --offset2 0xCC */
/* @PostProc: swap_instructions --offset1 0x120 --offset2 0x124 */
/* @PostProc: swap_instructions --offset1 0x2BC --offset2 0x2C0 */
/* @PostProc: swap_instructions --offset1 0x2C0 --offset2 0x2C4 */
/* @PostProc: swap_instructions --offset1 0x330 --offset2 0x334 */
/* @PostProc: swap_instructions --offset1 0x338 --offset2 0x33C */
/* @PostProc: swap_instructions --offset1 0x3BC --offset2 0x3C0 */
/* @PostProc: swap_instructions --offset1 0x3C4 --offset2 0x3C8 */
/* @PostProc: swap_instructions --offset1 0x450 --offset2 0x454 */
/* @PostProc: swap_instructions --offset1 0x458 --offset2 0x45C */
/* @PostProc: swap_instructions --offset1 0x55C --offset2 0x560 */
/* @PostProc: swap_instructions --offset1 0x5B4 --offset2 0x5BC */
/* @PostProc: swap_instructions --offset1 0x5F0 --offset2 0x5F8 */
/* @PostProc: swap_instructions --offset1 0x62C --offset2 0x634 */
/* @PostProc: patch_insn_reg_field 0x310:rt:v1:t8 0x314:rs:v1:t8 0x314:rt:v1:t8 0x31c:rs:v1:t8 0x31c:rt:v0:a1 0x320:rt:v0:a1 0x328:rt:v0:a1 0x330:rt:v0:a1 0x39c:rt:v1:t9 0x3a0:rs:v1:t9 0x3a0:rt:v1:t9 0x3a8:rs:v1:t9 0x3a8:rt:v0:a1 0x3ac:rt:v0:a1 0x3b4:rt:v0:a1 0x3bc:rt:v0:a1 0x430:rt:v1:t5 0x434:rs:v1:t5 0x434:rt:v1:t5 0x43c:rs:v1:t5 0x43c:rt:v0:a1 0x440:rt:v0:a1 0x448:rt:v0:a1 0x450:rt:v0:a1 */
#include "types.h"
#include "tokimeki/sprite.h"

extern void sub_sub_step_up(void);
extern void palette_upload_enqueue(s32, s32, s32, s32, s32);
extern void device_cmd_send(s32);
extern s32  get_CD_State2(void);
extern void cd_read_start(s32, u32, s32);
extern void cd_read_start2(s32, u32, s32);
extern void decdct(s32, s32, s32, s32);
extern void indexed_byte_toggle(s32);
extern void value_cache_find_or_load(s32, s32);
extern void dec_bg_init(s32);
extern s32  hizuke_disp_switch(s32);
extern void printf(char *);
extern void date_girl_cdread(void);
extern void date_load2_tbl(void);
extern void date_load3_tbl(void);
extern void date_workbuf_init(void);
extern void date_sprite_set(s32, s32, s32, s32, s32, s32);

extern volatile s32 step_counter;
extern u8  cdq_footer_chk;
extern u8  now_sub_sub_step;
extern u16 date_script_cnt;
extern s32 script_on;
extern s16 date2_intro_script_tbl0[];
extern s16 date2_intro_script_tbl1[];
extern u8  bg_change_tbl0;
extern u8  bg_change_tbl1;
extern u8  dec_bg_show_tbl0;
extern s32 cd_wait_timeout_tbl0;
extern volatile u8 sprite4_r;
extern u32 *mdec_bs;
extern u8  Sprites[];
extern u8  screen_w[];
extern s32 date_workbuf_tbl[];
extern char date_ending_script2_str0[];
extern s16 date_effect_init_tbl0[];

s32 date_ending_script2(void)
{
    s16 i;
    u8 *p;
    s32 n;
    s32 idx;
    s8 b;
    u32 v1;
    u32 cnt;
    u32 *pad;

    idx = step_counter;
    cdq_footer_chk = 0;
    switch (idx) {
    case 0:
        device_cmd_send(513);
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
        date_script_cnt = 0;
        i = 0;
        do {
            p = Sprites + (u32)i * 36U;
            *(s16 *)(p + 368) = (i << 6) + 128;
            *(s32 *)(p + 360) = 0x82000000;
            n = 160; *(s16 *)(p + 370) = n;
            *(u8 *)(p + 374) = 0;
            *(u8 *)(p + 375) = 0;
            *(s16 *)(p + 376) = 0;
            *(s16 *)(p + 372) = (i << 1) + 21;
            *(s16 *)(p + 378) = 480;
            *(u8 *)(p + 380) = 0;
            *(u8 *)(p + 381) = 0;
            *(u8 *)(p + 382) = 0;
            *(s16 *)(p + 384) = 0;
            *(s16 *)(p + 386) = 0;
            *(s16 *)(p + 388) = 4096;
            *(s16 *)(p + 390) = 4096;
            *(s32 *)(p + 392) = 0;
            *(s16 *)(p + 364) = (i << 7) - 160;
            *(s16 *)(p + 366) = -150;
            screen_w[i + 4271] = 12;
            i = i + 1;
        } while (i < 2);
        value_cache_find_or_load(17346, 1);
        idx = step_counter; idx += 1; step_counter = idx;
        return 0;
    case 1:
        if (get_CD_State2() & 1) {
            decdct(320, 256, 320, 160);
            idx = step_counter; idx += 1; step_counter = idx;
        }
        return 0;
    case 2:
        cd_read_start2(49, 0x801AE000U, 48351);
        date_workbuf_init();
        idx = step_counter; idx += 1; step_counter = idx;
        return 0;
    case 3:
    case 11:
    case 14:
    case 16:
    case 18:
        if (get_CD_State2() & 1) {
            idx = step_counter; idx += 1; step_counter = idx;
        }
        return 0;
    case 4:
        palette_upload_enqueue(date_workbuf_tbl[0], 17, 1, 0, 0);
        date_sprite_set(date_workbuf_tbl[1], date_workbuf_tbl[2], date_workbuf_tbl[3], date_workbuf_tbl[4],
                      date_workbuf_tbl[5], date_workbuf_tbl[6]);
        idx = step_counter; idx += 1; step_counter = idx;
        return 0;
    case 5:
        i = 0;
        do {
            p = Sprites + (u32)i * 36U;
            *(u32 *)(p + 144) = *(u32 *)(p + 144) & 0x7FFFFFFFU;
            i = i + 1;
        } while (i < 8);
        idx += 1; step_counter = idx;
        return 0;
    case 6:
        b = sprite4_r + 1;
        sprite4_r = b;
        ((SPRITE_REC *)Sprites)[4].b = b;
        ((SPRITE_REC *)Sprites)[4].g = b;
        i = 1;
        do {
            p = Sprites + (u32)i * 36U;
            n = sprite4_r;
            *(u8 *)(p + 166) = n;
            *(u8 *)(p + 165) = n;
            *(u8 *)(p + 164) = n;
            i = i + 1;
        } while (i < 8);
        if (sprite4_r == 64) {
            idx += 1; step_counter = idx;
        }
        return 0;
    case 7:
        b = sprite4_r + 1;
        sprite4_r = b;
        ((SPRITE_REC *)Sprites)[4].b = b;
        ((SPRITE_REC *)Sprites)[4].g = b;
        i = 1;
        do {
            p = Sprites + (u32)i * 36U;
            n = sprite4_r;
            *(u8 *)(p + 166) = n;
            *(u8 *)(p + 165) = n;
            *(u8 *)(p + 164) = n;
            i = i + 1;
        } while (i < 8);
        if (sprite4_r == 128) {
            idx += 1;
        }
        goto Ltail;
    case 8:
        goto Ltail;
    case 9:
        b = sprite4_r - 2;
        sprite4_r = b;
        ((SPRITE_REC *)Sprites)[4].b = b;
        ((SPRITE_REC *)Sprites)[4].g = b;
        i = 1;
        do {
            p = Sprites + (u32)i * 36U;
            n = sprite4_r;
            *(u8 *)(p + 166) = n;
            *(u8 *)(p + 165) = n;
            *(u8 *)(p + 164) = n;
            i = i + 1;
        } while (i < 8);
        if (sprite4_r == 0) {
            i = 0;
            do {
                p = Sprites + (u32)i * 36U;
                *(u32 *)(p + 144) = *(u32 *)(p + 144) | 0x80000000U;
                i = i + 1;
            } while (i < 8);
            idx += 1;
        }
        goto Ltail;
    case 10:
        cd_read_start(10, 0x80162000U, 17232);
        idx = step_counter; idx += 1; step_counter = idx;
        return 0;
    case 12:
        pad = mdec_bs;
        if (((pad[0] & 0xFFFF0000U) == 0x38000000U) &&
            ((pad[1] & 0xFF010000U) == 0x00010000U)) {
            goto L12ok;
        }
        printf(date_ending_script2_str0);
        date2_intro_script_tbl1[0] = 0;
        date2_intro_script_tbl1[1] = 0;
        idx = step_counter; idx -= 2; step_counter = idx;
        return 0;
L12ok:
        dec_bg_init(1);
        indexed_byte_toggle(1);
        idx = step_counter; idx += 1; step_counter = idx;
        return 0;
    case 13:
        step_counter = idx;
        date_girl_cdread();
        now_sub_sub_step = now_sub_sub_step - 1;
        idx += 1; step_counter = idx;
        return 0;
    case 15:
        step_counter = idx;
        date_load2_tbl();
        now_sub_sub_step = now_sub_sub_step - 1;
        idx += 1; step_counter = idx;
        return 0;
    case 17:
        step_counter = idx;
        date_load3_tbl();
        now_sub_sub_step = now_sub_sub_step - 1;
        idx += 1; step_counter = idx;
        return 0;
    default:
        sub_sub_step_up();
        script_on = 0;
        device_cmd_send(512);
        hizuke_disp_switch(1);
        return 0;
    }
Ltail:
        cnt = date_script_cnt + 1;
        cnt &= 0xFFFF;
        if (cnt % 19U == 18U) {
            i = 0;
            do {
                p = Sprites + (u32)i * 36U;
                *(s16 *)(p + 366) = *(s16 *)(p + 366) + 1;
                i = i + 1;
            } while (i < 2);
            p = Sprites + (u32)i * 36U;
            *(s16 *)(p + 370) = *(u16 *)(p + 370) - 1;
        }
        if (cnt % 5U == 4U) {
            i = 0;
            do {
                p = Sprites + (u32)i * 36U;
                *(s16 *)(p + 150) = *(s16 *)(p + 150) + 1;
                i = i + 1;
            } while (i < 6);
            i = 3;
            do {
                p = Sprites + (u32)i * 36U;
                v1 = *(u16 *)(p + 154);
                if (v1 != 0) {
                    *(s16 *)(p + 154) = v1 - 1;
                } else {
                    *(s16 *)(p + 46) = *(u16 *)(p + 46) - 1;
                }
                i = i + 1;
            } while (i < 6);
        }
        step_counter = idx;
        date_script_cnt = cnt;
        if (cnt == 300) {
            idx += 1; step_counter = idx;
        }

}
