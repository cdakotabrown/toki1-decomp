/* @PostProc: patch_stack_complete --old 160 --new 176 */
/* @PostProc: remap_stack_slots --from 64 --to 80 --from 104 --to 140 --from 128 --to 108 */
/* @PostProc: swap_instructions --offset1 0xDC --offset2 0xE0 */
/* @PostProc: patch_insn_reg_field 0xa0:rt:t6:t3 0xa4:rs:t6:t3 0xa4:rt:t6:t3 0xac:rt:t6:t3 0xb8:rt:a0:t4 0xbc:rs:a0:t4 0xbc:rt:a0:t4 0xc4:rt:a0:t4 0xdc:rt:t7:t5 0xe4:rs:t7:t5 0x134:rt:t9:t7 0x138:rt:t1:t8 0x13c:rt:t2:t9 0x140:rt:t8:t6 0x144:rs:t9:t7 0x144:rt:t9:t7 0x148:rs:t1:t8 0x148:rt:t1:t8 0x14c:rs:t2:t9 0x14c:rt:t2:t9 0x158:rt:t8:t6 0x160:rt:t9:t7 0x164:rt:t1:t8 0x168:rt:t2:t9 0x16c:rt:v0:t0 0x170:rs:v0:t0 0x170:rt:v0:t0 0x17c:rt:t3:t1 0x188:rt:t3:t1 0x190:rt:v0:t0 0x194:rt:t4:t2 0x198:rt:t6:t4 0x19c:rd:t5:t3 0x19c:rt:t4:t2 0x1a0:rs:t6:t4 0x1a0:rt:t6:t4 0x1a4:rd:t5:t3 0x1a4:rs:t5:t3 0x1a4:rt:t4:t2 0x1a8:rs:t5:t3 0x1a8:rt:t6:t4 0x1ac:rt:t7:t5 0x1b4:rs:t7:t5 0x1b4:rt:t8:t6 0x1b8:rt:t8:t6 0x1c0:rt:t9:t7 0x1c4:rt:t9:t7 0x1cc:rt:t1:t8 0x1d0:rt:t1:t8 0x1d4:rt:t2:t9 0x1dc:rs:t2:t9 0x1dc:rt:t3:t0 0x1e0:rt:t3:t0 0x1e8:rt:t4:t1 0x1ec:rt:t4:t1 0x1f4:rt:t5:t2 0x1f8:rt:t5:t2 0x1fc:rt:t6:t3 0x204:rs:t6:t3 0x204:rt:t7:t4 0x208:rt:t7:t4 0x210:rt:t8:t5 0x214:rt:t8:t5 0x21c:rt:t9:t6 0x220:rt:t9:t6 0x224:rt:t1:t7 0x22c:rs:t1:t7 0x22c:rt:t2:t8 0x230:rt:t2:t8 0x238:rt:t3:t9 0x23c:rt:t3:t9 0x244:rt:t4:t0 0x248:rt:t4:t0 0x24c:rt:t5:t1 0x254:rs:t5:t1 0x254:rt:t6:t2 0x258:rt:t6:t2 0x260:rt:t7:t3 0x264:rt:t7:t3 0x26c:rt:t8:t4 0x270:rt:t8:t4 0x274:rt:t9:t5 0x27c:rs:t9:t5 0x27c:rt:t1:t6 0x280:rt:t1:t6 0x288:rt:t2:t7 0x28c:rt:t2:t7 0x294:rt:t3:t8 0x298:rt:t3:t8 0x29c:rt:t4:t9 0x2a4:rs:t4:t9 0x2a4:rt:t5:t0 0x2a8:rt:t5:t0 0x2b0:rt:t6:t1 0x2b4:rt:t6:t1 0x2bc:rt:t7:t2 0x2c0:rt:t7:t2 0x2c8:rt:t8:t3 0x2cc:rt:t8:t3 0x2d4:rt:t9:t4 0x2d8:rt:t9:t4 0x2e0:rt:t1:t5 0x2e8:rt:t1:t5 0x2fc:rt:t2:t6 0x308:rt:t2:t6 0x310:rt:t3:t7 0x314:rs:t3:t7 0x314:rt:t3:t7 0x318:rt:t4:t8 0x31c:rt:t3:t7 0x320:rs:t4:t8 0x320:rt:t4:t8 0x324:rt:v0:t9 0x328:rt:t4:t8 0x32c:rs:v0:t9 0x32c:rt:v0:t9 0x338:rt:t5:t0 0x348:rt:t5:t0 0x350:rt:v0:t9 0x354:rt:t6:t1 0x35c:rt:t6:t1 0x364:rt:t7:t2 0x368:rt:t7:t2 0x370:rt:t8:t3 0x374:rt:t8:t3 0x37c:rs:t6:t1 0x3a4:rt:t9:t4 0x3ac:rd:t1:t5 0x3ac:rt:t9:t4 0x3b0:rt:t1:t5 0x408:rt:t2:t6 0x424:rs:t8:t2 0x48c:rs:v1:v0 0x498:rd:t3:t5 0x49c:rd:t4:t7 0x49c:rt:t3:t5 0x4a0:rt:t4:t7 0x50c:rd:t6:t2 0x510:rd:t9:t3 0x510:rt:t6:t2 0x514:rt:t9:t3 0x5a4:rd:t3:t4 0x5a8:rd:t3:t4 0x5a8:rs:t3:t4 0x5ac:rs:t3:t4 0x5b0:rd:t4:t5 0x5b4:rd:t4:t5 0x5b4:rs:t4:t5 0x5b8:rd:t4:t5 0x5b8:rt:t4:t5 0x5c0:rs:t4:t5 0x5fc:rd:v1:t8 0x600:rd:t2:t9 0x600:rt:v1:t8 0x604:rs:t2:t9 0x608:rt:t0:v1 0x610:rs:t0:v1 0x610:rt:t0:v1 0x614:rs:t0:v1 0x614:rt:t0:v1 0x61c:rs:t0:v1 0x638:rd:t7:t6 0x644:rs:t7:t6 0x674:rt:t8:t0 0x67c:rs:t8:t0 0x690:rd:t6:t2 0x694:rd:t9:t3 0x694:rt:t6:t2 0x698:rt:t9:t3 0x6d0:rd:t4:t5 0x6d4:rd:t5:t7 0x6d4:rt:t4:t5 0x6d8:rd:t2:t8 0x6d8:rt:t5:t7 0x6dc:rs:t2:t8 0x70c:rt:t7:t9 0x714:rs:t7:t9 0x750:rd:t6:t0 0x754:rd:t9:t2 0x754:rt:t6:t0 0x758:rd:t9:t2 0x758:rs:t9:t2 0x758:rt:t6:t0 0x75c:rd:t9:t2 0x75c:rt:t9:t2 0x760:rs:t9:t2 0x790:rt:t1:t3 0x798:rd:t3:t1 0x798:rt:t1:t3 0x79c:rd:t3:t1 0x79c:rs:t3:t1 0x79c:rt:t1:t3 0x7a0:rd:t3:t1 0x7a0:rt:t3:t1 0x7a4:rs:t3:t1 */
/* @PostProc: patch_insn_reg_field 0x68:rd:v1:v0 0x6c:rs:v1:v0 0x6c:rt:v0:t1 0x70:rt:t5:t2 0x74:rs:v0:t1 0x74:rt:t1:t4 0x78:rs:t1:t4 0x78:rt:t2:t7 0x7c:rs:v1:v0 0x7c:rt:t1:t4 0x80:rs:t2:t7 0x80:rt:t3:t0 0x84:rs:v1:v0 0x84:rt:t2:t7 0x88:rs:v1:v0 0x88:rt:t3:t0 0x8c:rs:t3:t0 0x8c:rt:t4:t1 0x90:rs:v1:v0 0x90:rt:t4:t1 0x94:rs:t5:t2 0x94:rt:t5:t2 0x9c:rt:t5:t2 0xc8:rt:v1:v0 0x404:rt:t3:t7 0x40c:rs:t3:t7 0x40c:rt:t4:t8 0x410:rs:t2:t6 0x410:rt:t2:t6 0x414:rs:t4:t8 0x414:rt:t5:t9 0x418:rt:t5:t9 0x418:rd:t7:t0 0x41c:rs:t2:t6 0x41c:rt:t7:t0 0x41c:rd:t8:t2 0x440:rt:t6:t3 0x448:rs:t6:t3 0x448:rt:v1:v0 0x450:rt:v1:v0 0x450:rd:t9:t1 0x454:rt:t9:t1 0x454:rd:t1:t4 0x458:rs:t1:t4 0x460:rs:t1:t4 0x460:rd:v1:v0 0x468:rs:t1:t4 0x4b4:rt:t5:t8 0x4cc:rt:t5:t8 0x4d4:rs:t5:t8 0x4d4:rt:t2:t9 0x4dc:rt:t2:t9 0x4dc:rd:t7:t6 0x4e0:rt:t7:t6 0x4e0:rd:t8:t0 0x4e4:rs:t8:t0 0x500:rs:t8:t0 0x5ec:rt:t5:t7 0x5f4:rs:t5:t7 0x6c0:rt:t3:t4 0x6c8:rs:t3:t4 0x740:rt:t8:t6 0x748:rs:t8:t6 */
/* Address: 0x8013CF5C, Size: 2044 bytes (511 insns) */
#include "types.h"
#include "tokimeki/class_w.h"

extern void palette_upload_enqueue(void *p, s32 c, s32 a, s32 b, s32 i);
extern void class_clear_f(void);
extern void class_clear_n(s32 a0);
extern void set_kanji_string(s32 x, s32 y, s32 attr, void *s, s32 f);
extern void strcpy(void *buf, void *s);
extern void strcat(void *buf, void *s);

extern u8   screen_w[];
extern u16  pass_days;
extern s32  girl12_birth;

extern u8  *tpage_set_adr;
extern u8  *img_set_adr;
extern u8  *anim_set_adr;
extern u8  *clut_set_adr;
extern u8   tel_slot_tpage_buf[];
extern u8   tel_slot_img_buf[];
extern u8   tel_slot_clut_buf[];

extern u8   class_w[];
extern u8   cw8_rec[];
extern u8   basyo_init_tbl1[];
extern u8   basyo_init_tbl2[];
extern u8   _sys_default_tpage_set_tbl2[];
extern s16  basyo_init_tbl3;

extern u8   holiday_tel_exit_tbl0[];
extern void *girl_full_name_tbl[];
extern void *telephone_choice_menu_init_tbl1;
extern void **card_file_save_tbl0;
extern void *album_info_disp_tbl0[];
extern u8   space_str[];

extern u8   tel_slot_disp_str0[];
extern u8   tel_slot_disp_str1[];
extern u8   tel_slot_disp_str2[];
extern u8   tel_slot_disp_str3[];
extern u8   tel_slot_disp_str4[];
extern u8   tel_slot_disp_str5[];
extern u8   tel_slot_disp_str6[];
extern u8   tel_slot_disp_str7[];
extern u8   tel_slot_disp_str8[];
extern u8   tel_slot_disp_str9[];

extern s16  cal_cw_frame, cal_cw_x, cal_cw_y;
extern s16  cw97_frame, cw97_x, cw97_y;
extern s16  cw98_frame, cw98_x, cw98_y;

void tel_slot_disp(s32 mode, s32 arg1)
{
    u8 sbuf[32];
    u8 *slot;
    u8 *sp1;
    u8 *vp;
    u8 *buf;
    u8 *tbl;
    s32 idx;
    s32 i;
    s32 b;
    s16 hcopy;
    u8 *pcopy;
    s32 phase;
    s32 v;
    s32 n;
    s32 ten;
    s32 rank;

    if (mode == 11) {
        idx = 12;
    } else {
        idx = mode;
    }

    slot = &screen_w[idx * 56];
    b = slot[456];
    b = b | 8;    *(volatile u8 *)(slot + 456) = b;
    b = b | 0x40; *(volatile u8 *)(slot + 456) = b;
    b = b | 4;    *(volatile u8 *)(slot + 456) = b;
    b = b | 0x80; *(volatile u8 *)(slot + 456) = b;

    tpage_set_adr = (u8 *)0x801B01B0;
    img_set_adr = (u8 *)0x801B01C0;
    anim_set_adr = (u8 *)0;
    clut_set_adr = (u8 *)0x801B0200;

    for (i = 0; i < 16; i++) {
        palette_upload_enqueue(clut_set_adr, (i + 16) & 0xFF, 1, 2, i);
    }
    class_clear_f();

    sp1 = class_w;
    for (i = 0; i < 8; i++) {
        class_clear_n(i + 96);
        sp1[6529] = 4;
        sp1[6530] = 64;
        *(u32 *)(sp1 + 6584) = 0x40000000;
        sp1[6531] = 132;
        *(u8 **)(sp1 + 6540) = basyo_init_tbl1;
        *(u8 **)(sp1 + 6544) = basyo_init_tbl2;
        *(u8 **)(sp1 + 6580) = _sys_default_tpage_set_tbl2;
        hcopy = basyo_init_tbl3;
        sp1 += 68;
        sp1[6464] = 8;
        sp1[6465] = 1;
        *(s16 *)(sp1 + 6480) = hcopy;
    }

    vp = &holiday_tel_exit_tbl0[idx * 7];
    cal_cw_frame = vp[0] + 78; cal_cw_x = 39;  cal_cw_y = -90;
    cw97_frame = vp[1] + 78; cw97_x = 49;  cw97_y = -90;
    cw98_frame = vp[2] + 78; cw98_x = 59;  cw98_y = -90;
    ((CLASS_W *)class_w)[99].frame = vp[3] + 78; ((CLASS_W *)class_w)[99].x.half[1] = 76;  ((CLASS_W *)class_w)[99].y.half[1] = -90;
    ((CLASS_W *)class_w)[100].frame = vp[4] + 78; ((CLASS_W *)class_w)[100].x.half[1] = 86;  ((CLASS_W *)class_w)[100].y.half[1] = -90;
    ((CLASS_W *)class_w)[101].frame = vp[5] + 78; ((CLASS_W *)class_w)[101].x.half[1] = 96;  ((CLASS_W *)class_w)[101].y.half[1] = -90;
    ((CLASS_W *)class_w)[102].frame = vp[6] + 78; ((CLASS_W *)class_w)[102].x.half[1] = 106; ((CLASS_W *)class_w)[102].y.half[1] = -90;
    ((CLASS_W *)class_w)[103].frame = 65; ((CLASS_W *)class_w)[103].x.half[1] = -16; ((CLASS_W *)class_w)[103].y.half[1] = -27;

    sp1 = cw8_rec;
    for (i = 8; i != 9; i++) {
        class_clear_n(i + 96);
        sp1[6529] = 4;
        sp1[6530] = 64;
        *(u32 *)(sp1 + 6584) = 0x01000000;
        sp1[6531] = 132;
        *(u32 *)(sp1 + 6540) = (u32)img_set_adr;
        *(u32 *)(sp1 + 6544) = (u32)anim_set_adr;
        pcopy = tpage_set_adr;
        sp1 += 68;
        *(s16 *)(sp1 + 6480) = 0;
        sp1[6464] = 8;
        sp1[6465] = 128;
        *(u32 *)(sp1 + 6512) = (u32)pcopy;
    }

    ((CLASS_W *)class_w)[104].frame = mode;
    ((CLASS_W *)class_w)[104].x.half[1] = 64;
    ((CLASS_W *)class_w)[104].y.half[1] = -40;

    if (mode == 11) {
        set_kanji_string(-128, -98, 3, telephone_choice_menu_init_tbl1, 0);
    } else {
        set_kanji_string(-128, -98, 3, girl_full_name_tbl[mode], 0);
    }
    set_kanji_string(-32, -98, 5, tel_slot_disp_str0, 0);
    set_kanji_string(-138, -78, 4, tel_slot_disp_str1, 0);

    buf = sbuf;
    ten = 10;

    strcpy(buf, card_file_save_tbl0[(*(s32 *)(slot + 460) & 0xF) - 1]);
    strcat(buf, tel_slot_disp_str2);
    n = ((u32)*(s32 *)(slot + 460) << 23) >> 27;
    if ((u32)n >= 10) {
        strcat(buf, album_info_disp_tbl0[n / ten]);
    } else {
        strcat(buf, space_str);
    }
    strcat(buf,
                  album_info_disp_tbl0[(s32)(((u32)*(s32 *)(slot + 460) << 23) >> 27)
                             % ten]);
    strcat(buf, tel_slot_disp_str3);
    set_kanji_string(-88, -78, 3, buf, 0);

    set_kanji_string(-138, -30, 4, tel_slot_disp_str4, 0);

    v = pass_days;
    if ((u32)v >= 730) {
        phase = 2;
    } else if ((u32)v >= 365) {
        phase = 1;
    } else {
        phase = 0;
    }
    set_kanji_string(-88, -30, 3,
                  (u8 *)0x801B2334 + (mode * 3 + phase) * 18, 0);

    set_kanji_string(-138, -62, 4, tel_slot_disp_str5, 0);

    n = ((u32)*(s32 *)(slot + 460) << 23) >> 27;
    rank = (*(s32 *)(slot + 460) & 0xF) - 1;
    if ((u32)n >= 22) {
        rank = rank + 1;
    }
    set_kanji_string(-88, -62, 3,
                  (u8 *)0x801B22BC + (rank % 12) * ten, 0);

    set_kanji_string(-138, -46, 4, tel_slot_disp_str6, 0);

    if (mode == 11) {
        set_kanji_string(-88, -46, 3,
                      (u8 *)0x801B229C +
                          (((u32)girl12_birth << 21) >> 30) * 8, 0);
    } else {
        set_kanji_string(-88, -46, 3,
                      (u8 *)0x801B229C +
                          (((u32)*(s32 *)(slot + 460) << 21) >> 30) * 8, 0);
    }

    set_kanji_string(-138, -14, 4, tel_slot_disp_str7, 0);

    if (mode == 11) {
        set_kanji_string(-88, -14, 3, tel_slot_disp_str8, 0);
    } else {
        set_kanji_string(-88, -14, 3,
                      (u8 *)0x801B2200 + ((u32)slot[458] >> 4) * 12, 0);
    }

    set_kanji_string(-138, 2, 4, tel_slot_disp_str9, 0);

    tbl = (u8 *)0x801B25F4 + (mode * 6 + phase * 2) * 32;
    set_kanji_string(-88, 2, 3, tbl, 0);
    set_kanji_string(-88, 18, 3, tbl + 32, 0);
    (void)arg1;
}
