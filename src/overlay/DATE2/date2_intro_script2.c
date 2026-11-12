/* @CompileFlags: -Wc,-pic0 */
/* @PostProc: remap_stack_slots --from 32 --to 48 */
/* @SymbolStart: 0x801348E4 */
/* date2_intro_script2 -- DATE2 overlay, Address: 0x801348F4, Size: 1956 bytes (489 insns) */
/* @RodataBase: 0x8013A1AC */
/* @SymbolSize: 1972 */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* @PostProc: scheduler_normalizer --block-aware --const-fp --store-fp --load-fp */
/* @PostProc: swap_instructions --offset1 0x784 --offset2 0x794 */
/* @PostProc: patch_insn_reg_field 0x17c:rt:t9:a3 0x180:rs:t9:a3 0x180:rt:t9:a3 0x188:rs:t9:a3 0x188:rt:t5:a3 0x18c:rt:t5:a3 0x1a0:rt:t6:t9 0x1a4:rs:t6:t9 0x1a8:rt:t7:t5 0x1ac:rs:t7:t5 0x1ac:rt:t7:t5 0x1b4:rs:t7:t5 0x1b4:rt:t8:t6 0x1bc:rd:t9:t7 0x1bc:rs:t8:t6 0x1c4:rs:t9:t7 0x1cc:rs:t7:t5 0x1cc:rt:t5:t8 0x1d4:rd:t6:t9 0x1d4:rs:t5:t8 0x1dc:rs:t6:t9 0x1f0:rt:t8:a3 0x1f8:rs:t8:a3 0x1f8:rt:t8:a3 0x208:rs:t8:a3 0x208:rt:t9:a3 0x20c:rt:t9:a3 0x228:rt:t7:a3 0x22c:rs:t7:a3 0x22c:rt:t7:a3 0x234:rs:t7:a3 0x234:rt:t5:a3 0x238:rt:t5:a3 0x260:rt:t6:a3 0x264:rs:t6:a3 0x264:rt:t6:a3 0x26c:rs:t6:a3 0x26c:rt:t8:a3 0x270:rt:t8:a3 0x284:rt:t9:t6 0x288:rs:t9:t6 0x28c:rt:t7:a3 0x290:rs:t7:a3 0x290:rt:t7:a3 0x298:rs:t7:a3 0x298:rt:t5:a3 0x29c:rt:t5:a3 0x2c4:rt:t6:t7 0x2c8:rt:t8:t5 0x2cc:rs:t8:t5 0x2cc:rt:t8:t5 0x2d0:rs:t6:t7 0x2d0:rt:t6:t7 0x2f4:rt:t8:t5 0x2fc:rt:t6:t7 0x300:rt:t9:a3 0x304:rs:t9:a3 0x304:rt:t9:a3 0x30c:rs:t9:a3 0x30c:rt:t7:a3 0x310:rt:t7:a3 0x33c:rd:t9:t7 0x340:rt:t9:t7 0x348:rd:t5:t8 0x34c:rt:t5:t8 0x350:rt:t6:t9 0x358:rd:t8:t6 0x358:rs:t6:t9 0x360:rt:t8:t6 0x370:rd:t9:t7 0x374:rt:t9:t7 0x37c:rd:t5:t8 0x380:rt:t5:t8 0x384:rt:t6:t9 0x38c:rd:t8:t6 0x38c:rs:t6:t9 0x394:rt:t8:t6 0x398:rt:t5:a3 0x3a0:rt:t5:a3 0x3ac:rt:v0:t8 0x3b0:rs:v0:t8 0x3b0:rt:v0:t8 0x3b8:rs:v0:t8 0x3b8:rt:v0:a1 0x3bc:rt:v0:a1 0x3c4:rt:v0:a1 0x3cc:rt:v0:a1 0x3f0:rd:t8:t7 0x3f4:rt:t8:t7 0x428:rd:t5:t9 0x42c:rt:t5:t9 0x434:rd:t7:t8 0x438:rt:t7:t8 0x44c:rt:t8:t7 0x450:rs:t8:t7 0x450:rt:t8:t7 0x458:rs:t8:t7 0x45c:rt:t9:a3 0x464:rt:t9:a3 0x470:rt:v0:t5 0x474:rs:v0:t5 0x474:rt:v0:t5 0x47c:rs:v0:t5 0x47c:rt:v0:a1 0x480:rt:v0:a1 0x488:rt:v0:a1 0x490:rt:v0:a1 0x4b4:rd:t5:t6 0x4b8:rt:t5:t6 0x4c0:rd:t7:t9 0x4c4:rt:t7:t9 0x4ec:rd:t9:t8 0x4f0:rt:t9:t8 0x4f8:rd:t8:t5 0x4fc:rt:t8:t5 0x510:rt:t5:t6 0x514:rs:t5:t6 0x514:rt:t5:t6 0x51c:rs:t5:t6 0x52c:rt:v0:t7 0x530:rs:v0:t7 0x530:rt:v0:t7 0x538:rs:v0:t7 0x538:rt:v0:a1 0x53c:rt:v0:a1 0x544:rt:v0:a1 0x54c:rt:v0:a1 0x570:rd:t8:t9 0x574:rt:t8:t9 0x57c:rd:t6:t8 0x580:rt:t6:t8 0x5cc:rt:t8:t9 0x5d0:rs:t8:t9 0x5d0:rt:t8:t9 0x5d8:rs:t8:t9 0x5e8:rd:t6:t8 0x5ec:rt:t6:t8 0x5f4:rd:t9:t6 0x5f8:rt:t9:t6 0x61c:rd:t6:t8 0x620:rt:t6:t8 0x628:rd:t9:t6 0x62c:rt:t9:t6 0x660:rt:t9:a3 0x664:rs:t9:a3 0x664:rt:t9:a3 0x66c:rs:t9:a3 0x66c:rt:t7:a3 0x670:rt:t7:a3 0x67c:rt:t5:t6 0x680:rs:t5:t6 0x680:rt:t5:t6 0x688:rs:t5:t6 0x688:rt:t6:t7 0x690:rd:t8:t5 0x690:rs:t6:t7 0x698:rs:t8:t5 0x6a0:rs:t5:t6 0x6a0:rt:t9:t8 0x6a8:rd:t7:t9 0x6a8:rs:t9:t8 0x6b0:rs:t7:t9 0x6c4:rt:t6:a3 0x6cc:rs:t6:a3 0x6cc:rt:t6:a3 0x6dc:rs:t6:a3 0x6dc:rt:t8:a3 0x6e0:rt:t8:a3 0x6fc:rt:t5:a3 0x700:rs:t5:a3 0x700:rt:t5:a3 0x708:rs:t5:a3 0x708:rt:t9:a3 0x70c:rt:t9:a3 0x738:rt:t6:t5 0x73c:rt:t6:t5 0x744:rt:t8:a3 0x748:rt:t8:a3 0x77c:rt:a1:v0 0x780:rs:a1:v0 0x780:rt:a1:v0 0x788:rs:a1:v0 0x788:rt:t5:v0 0x78c:rs:t5:v0 0x78c:rt:v1:t6 0x790:rt:v1:t6 0x7a0:rs:v1:t6 0x7a4:rs:v1:t6 0x7a8:rt:t7:a3 0x7b0:rt:t7:a3 */
#include "types.h"
#include "tokimeki/sprite.h"

extern u8 cdq_footer_chk;

extern void device_cmd_send(s32);
extern s32  hizuke_disp_switch(s32);
extern void date2_bg_cache_step2(s32);
extern s32  get_CD_State2(void);
extern s32  printf(char *);
extern void decdct(s32, s32, s32, s32);
extern void cd_read_start2(s32, u32, s32);
extern void date2_buf_table_init(void);
extern void palette_upload_enqueue(s32, s32, s32, s32, s32);
extern void date_sprite_init(s32, s32, s32, s32, s32, s32);
extern void cd_read_start(s32, u32, s32);
extern void dec_bg_init(s32);
extern void indexed_byte_toggle(s32);
extern void date2_girl_serifu_load(void);
extern void sub_sub_step_up(void);

extern volatile s32 step_counter;
extern volatile u8  now_sub_sub_step;
extern u16 date2_wait_cnt;
extern s32 script_on;
extern s16 date2_intro_script_tbl0[];
extern s16 date2_intro_script_tbl1[];
extern u8  bg_change_tbl0; extern u8  bg_change_tbl1; extern u8  dec_bg_show_tbl0;
extern s32 cd_wait_timeout_tbl0;
extern volatile u8 sprite58_r;extern u32 *mdec_bs;
extern u8  Sprites[];
extern u8  screen_w[];
extern char date2_intro_script2_str0[];
extern char date2_intro_script2_str1[];
extern s32 date2_intro_buf_tbl[];
extern s16 date_effect_init_tbl0[];

s32 date2_intro_script2(void)
{
    s16 i;
    u8 *p;
    s32 b;
    u8 bv;
    u32 cnt;
    s32 n;
    u32 *pad;
    s32 step;

    step = step_counter;
    cdq_footer_chk = 0;

    switch (step) {
    case 0:
        script_on = 1;
        device_cmd_send(513);
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
        i = 0;
        do {
            p = Sprites + (u32)i * 36U;
            *(s16 *)(p + 368) = (i << 6) + 128;
            *(s32 *)(p + 360) = 0x82000000;
            *(s16 *)(p + 370) = 160;
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
        date2_bg_cache_step2(17346);
        step_counter += 1;
        return 0;
    case 1:
        if (get_CD_State2() & 1) {
            pad = mdec_bs;
            if (((pad[0] & 0xFFFF0000U) == 0x38000000U) &&
                ((pad[1] & 0xFF010000U) == 0x00010000U)) {
                goto ok1;
            }
            printf(date2_intro_script2_str0);
            date2_intro_script_tbl1[0] = 0;
            date2_intro_script_tbl1[1] = 0;
            step_counter -= 1;
            return 0;
ok1:
            decdct(320, 256, 320, 160);
            step_counter += 1;
        }
        return 0;
    case 2:
        cd_read_start2(49, 0x801AE000U, 0xBCDF);
        date2_buf_table_init();
        step_counter += 1;
        return 0;
    case 3:
    case 11:
    case 14:
    case 16:
    case 18:
        if (get_CD_State2() & 1) {
            step_counter += 1;
        }
        return 0;
    case 4:
        palette_upload_enqueue(date2_intro_buf_tbl[0], 17, 1, 0, 0);
        date_sprite_init(date2_intro_buf_tbl[1], date2_intro_buf_tbl[2], date2_intro_buf_tbl[3], date2_intro_buf_tbl[4],
                      date2_intro_buf_tbl[5], date2_intro_buf_tbl[6]);
        step_counter += 1;
        return 0;
    case 5:
        i = 10;
        do {
            p = Sprites + (u32)i * 36U;
            *(s32 *)p &= 0x7FFFFFFF;
            i = i + 1;
        } while (i < 12);
        i = 58;
        do {
            p = Sprites + (u32)i * 36U;
            *(s32 *)p &= 0x7FFFFFFF;
            i = i + 1;
        } while (i < 64);
        step_counter = step + 1;
        return 0;
    case 6:
        b = sprite58_r + 4;
        sprite58_r = b;
        ((SPRITE_REC *)Sprites)[58].b = b;
        ((SPRITE_REC *)Sprites)[58].g = b;
        i = 10;
        do {
            p = Sprites + (u32)i * 36U;
            bv = sprite58_r;
            *(p + 22) = bv;
            *(p + 21) = bv;
            *(p + 20) = bv;
            i = i + 1;
        } while (i < 12);
        i = 59;
        do {
            p = Sprites + (u32)i * 36U;
            bv = sprite58_r;
            *(p + 22) = bv;
            *(p + 21) = bv;
            *(p + 20) = bv;
            i = i + 1;
        } while (i < 64);
        if (sprite58_r == 64) {
            step_counter = step + 1;
        }
        return 0;
    case 7:
        b = sprite58_r + 4;
        sprite58_r = b;
        ((SPRITE_REC *)Sprites)[58].b = b;
        ((SPRITE_REC *)Sprites)[58].g = b;
        i = 10;
        do {
            p = Sprites + (u32)i * 36U;
            bv = sprite58_r;
            *(p + 22) = bv;
            *(p + 21) = bv;
            *(p + 20) = bv;
            i = i + 1;
        } while (i < 12);
        i = 59;
        do {
            p = Sprites + (u32)i * 36U;
            bv = sprite58_r;
            *(p + 22) = bv;
            *(p + 21) = bv;
            *(p + 20) = bv;
            i = i + 1;
        } while (i < 64);
        if (sprite58_r == 128) {
            step = step + 1;
        }
        goto counter;
    case 9:
        b = sprite58_r - 4;
        sprite58_r = b;
        ((SPRITE_REC *)Sprites)[58].b = b;
        ((SPRITE_REC *)Sprites)[58].g = b;
        i = 10;
        do {
            p = Sprites + (u32)i * 36U;
            bv = sprite58_r;
            *(p + 22) = bv;
            *(p + 21) = bv;
            *(p + 20) = bv;
            i = i + 1;
        } while (i < 12);
        i = 59;
        do {
            p = Sprites + (u32)i * 36U;
            bv = sprite58_r;
            *(p + 22) = bv;
            *(p + 21) = bv;
            *(p + 20) = bv;
            i = i + 1;
        } while (i < 64);
        if (sprite58_r != 0) {
            goto counter;
        }
        i = 10;
        do {
            p = Sprites + (u32)i * 36U;
            *(s32 *)p |= 0x80000000;
            i = i + 1;
        } while (i < 12);
        i = 58;
        do {
            p = Sprites + (u32)i * 36U;
            *(s32 *)p |= 0x80000000;
            i = i + 1;
        } while (i < 64);
        step = step + 1;
        goto counter;
    case 10:
        cd_read_start(10, 0x80162000U, 17232);
        step_counter += 1;
        return 0;
    case 12:
        pad = mdec_bs;
        if (((pad[0] & 0xFFFF0000U) == 0x38000000U) &&
            ((pad[1] & 0xFF010000U) == 0x00010000U)) {
            goto ok12;
        }
        printf(date2_intro_script2_str1);
        date2_intro_script_tbl1[0] = 0;
        date2_intro_script_tbl1[1] = 0;
        step_counter -= 2;
        return 0;
ok12:
        dec_bg_init(1);
        indexed_byte_toggle(1);
        step_counter += 1;
        return 0;
    case 13:
        step_counter = step;
        date2_girl_serifu_load();
        now_sub_sub_step = now_sub_sub_step - 1;
        step_counter = step + 1;
        return 0;
    case 15:
    case 17:
    default:
        sub_sub_step_up();
        script_on = 0;
        device_cmd_send(512);
        hizuke_disp_switch(1);
        return 0;
    case 8:
counter:
        cnt = date2_wait_cnt;
        cnt += 1;
        n = (date2_wait_cnt = cnt & 0xFFFF);
        step_counter = step;
        if (n == 150) {
            step_counter = step + 1;
        }
        return n;
    }
}
