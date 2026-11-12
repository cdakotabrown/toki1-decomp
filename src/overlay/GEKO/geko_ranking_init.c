/* @PostProc: patch_stack_complete --old 112 --new 1152 */
/* @PostProc: remap_stack_slots --map 88:64,90:66,92:68,94:70,96:72,98:74,100:76,102:78,104:80,106:82,108:84,110:86 */
/* @PostProc: swap_instructions --offset1 0x7c --offset2 0x8c */
/* @PostProc: swap_instructions --offset1 0x80 --offset2 0xa4 */
/* @PostProc: swap_instructions --offset1 0x84 --offset2 0x90 */
/* @PostProc: swap_instructions --offset1 0x88 --offset2 0xa8 */
/* @PostProc: swap_instructions --offset1 0x8c --offset2 0xa8 */
/* @PostProc: swap_instructions --offset1 0x90 --offset2 0xa8 */
/* @PostProc: swap_instructions --offset1 0x98 --offset2 0xa0 */
/* @PostProc: swap_instructions --offset1 0x9c --offset2 0xa4 */
/* @PostProc: swap_instructions --offset1 0xa4 --offset2 0xa8 */
/* @PostProc: swap_instructions --offset1 0x10c --offset2 0x114 */
/* @PostProc: swap_instructions --offset1 0x110 --offset2 0x114 */
/* @PostProc: swap_instructions --offset1 0x114 --offset2 0x118 */
/* @PostProc: swap_instructions --offset1 0x11c --offset2 0x120 */
/* @PostProc: swap_instructions --offset1 0x124 --offset2 0x128 */
/* @PostProc: swap_instructions --offset1 0x128 --offset2 0x12c */
/* @PostProc: swap_instructions --offset1 0x134 --offset2 0x13c */
/* @PostProc: swap_instructions --offset1 0x138 --offset2 0x13c */
/* @PostProc: swap_instructions --offset1 0x13c --offset2 0x140 */
/* @PostProc: swap_instructions --offset1 0x17c --offset2 0x180 */
/* @PostProc: swap_instructions --offset1 0x1ac --offset2 0x1b4 */
/* @PostProc: swap_instructions --offset1 0x1b0 --offset2 0x1b4 */
/* @PostProc: swap_instructions --offset1 0x1b4 --offset2 0x1b8 */
/* @PostProc: swap_instructions --offset1 0x1bc --offset2 0x1c0 */
/* @PostProc: swap_instructions --offset1 0x1c4 --offset2 0x1c8 */
/* @PostProc: swap_instructions --offset1 0x1d4 --offset2 0x1d8 */
/* @PostProc: swap_instructions --offset1 0x20c --offset2 0x210 */
/* @PostProc: swap_instructions --offset1 0x23c --offset2 0x240 */
/* @PostProc: swap_instructions --offset1 0x24c --offset2 0x250 */
/* @PostProc: swap_instructions --offset1 0x284 --offset2 0x288 */
/* @PostProc: swap_instructions --offset1 0x2ac --offset2 0x2b0 */
/* @PostProc: swap_instructions --offset1 0x2bc --offset2 0x2c0 */
/* @PostProc: swap_instructions --offset1 0x2e4 --offset2 0x2e8 */
/* @PostProc: swap_instructions --offset1 0x32c --offset2 0x330 */
/* @PostProc: swap_instructions --offset1 0x358 --offset2 0x35c */
/* @PostProc: swap_instructions --offset1 0x380 --offset2 0x3a4 */
/* @PostProc: swap_instructions --offset1 0x384 --offset2 0x398 */
/* @PostProc: swap_instructions --offset1 0x388 --offset2 0x398 */
/* @PostProc: swap_instructions --offset1 0x38c --offset2 0x398 */
/* @PostProc: swap_instructions --offset1 0x390 --offset2 0x3a0 */
/* @PostProc: swap_instructions --offset1 0x394 --offset2 0x398 */
/* @PostProc: swap_instructions --offset1 0x398 --offset2 0x3a4 */
/* @PostProc: swap_instructions --offset1 0x39c --offset2 0x3a8 */
/* @PostProc: swap_instructions --offset1 0x3a4 --offset2 0x3a8 */
/* @PostProc: swap_instructions --offset1 0x3a8 --offset2 0x3ac */
/* @PostProc: swap_instructions --offset1 0x3b4 --offset2 0x3b8 */
/* @PostProc: swap_instructions --offset1 0x308 --offset2 0x30c */
/* @PostProc: swap_instructions --offset1 0x3b8 --offset2 0x3c4 */
/* @PostProc: patch_insn_reg_field 0x78:rt:a1:s2 0x7c:rt:a2:a1 0x80:rt:a3:a2 0x84:rt:t0:a3 0x88:rt:t1:t0 0x8c:rt:t3:t1 0x94:rt:t5:t3 0x9c:rt:ra:t5 0xa0:rt:s0:ra 0xa4:rt:s1:s0 0xa8:rt:s2:s1 0xb0:rs:a1:s2 0xb0:rt:a1:s2 0xc4:rs:a1:s2 0xd0:rd:t9:t7 0xd4:rs:t9:t7 0xdc:rt:t7:t8 0xe0:rt:t8:t6 0xe8:rs:t7:t8 0xe8:rt:t8:t6 0xec:rt:s1:s0 0xf4:rt:ra:t5 0xf8:rd:t3:t1 0xf8:rt:a2:a1 0xfc:rt:a3:a2 0x100:rd:a2:a1 0x100:rt:t0:a3 0x104:rd:a3:a2 0x104:rt:t1:t0 0x108:rd:t7:t8 0x108:rt:a2:a1 0x10c:rd:t8:t6 0x10c:rt:a3:a2 0x110:rd:t0:a3 0x114:rd:t1:t0 0x114:rt:t5:t3 0x118:rd:a2:a1 0x118:rs:t7:t8 0x11c:rd:a3:a2 0x11c:rs:t8:t6 0x120:rt:ra:t5 0x124:rd:t5:t3 0x124:rt:s0:ra 0x128:rd:t6:t9 0x128:rt:t3:t1 0x12c:rd:t7:t8 0x130:rd:t8:t6 0x130:rt:t5:t3 0x134:rd:t3:t1 0x134:rs:t6:t9 0x138:rs:t7:t8 0x13c:rd:t5:t3 0x13c:rs:t8:t6 0x140:rt:t7:t6 0x148:rd:t9:t7 0x14c:rd:t6:t9 0x14c:rt:t0:a3 0x150:rd:ra:t5 0x150:rt:s1:s0 0x154:rs:t9:t7 0x158:rd:t0:a3 0x158:rs:t6:t9 0x15c:rd:t9:t7 0x15c:rt:t1:t0 0x160:rd:t6:t9 0x160:rt:ra:t5 0x164:rd:s0:ra 0x164:rt:s2:s1 0x168:rd:s1:s0 0x168:rs:t7:t8 0x168:rt:t8:t6 0x16c:rd:t1:t0 0x16c:rs:t9:t7 0x170:rd:ra:t5 0x170:rs:t6:t9 0x174:rd:s2:s1 0x178:rd:t9:t7 0x178:rt:s0:ra 0x17c:rd:t6:t9 0x17c:rt:s1:s0 0x180:rd:t7:t8 0x180:rt:s2:s1 0x184:rd:s0:ra 0x184:rs:t9:t7 0x188:rd:s1:s0 0x188:rt:t6:t9 0x190:rd:s2:s1 0x190:rs:t7:t8 0x194:rt:ra:t5 0x1a0:rd:t3:t1 0x1a0:rt:a2:a1 0x1a4:rt:a3:a2 0x1a8:rd:t8:t6 0x1a8:rt:t3:t1 0x1ac:rd:t6:t9 0x1b0:rd:a2:a1 0x1b0:rt:t0:a3 0x1b4:rd:a3:a2 0x1b4:rt:t1:t0 0x1b8:rd:t3:t1 0x1b8:rs:t8:t6 0x1bc:rs:t6:t9 0x1c0:rd:t0:a3 0x1c4:rd:t1:t0 0x1c4:rt:t5:t3 0x1c8:rd:t8:t6 0x1c8:rt:t0:a3 0x1cc:rd:t6:t9 0x1cc:rt:t1:t0 0x1d0:rd:t0:a3 0x1d0:rs:t8:t6 0x1d4:rd:t1:t0 0x1d4:rs:t6:t9 0x1d8:rt:t8:t9 0x1e0:rd:t9:t7 0x1e0:rt:a2:a1 0x1e4:rt:ra:t5 0x1e8:rd:a2:a1 0x1e8:rs:t9:t7 0x1ec:rd:t7:t8 0x1ec:rt:a3:a2 0x1f0:rd:t9:t7 0x1f4:rd:t5:t3 0x1f4:rt:s0:ra 0x1f8:rd:ra:t5 0x1f8:rs:t8:t6 0x1f8:rt:t6:t9 0x1fc:rd:a3:a2 0x1fc:rs:t7:t8 0x200:rs:t9:t7 0x204:rd:s0:ra 0x208:rd:t7:t8 0x208:rt:t5:t3 0x20c:rd:t9:t7 0x20c:rt:ra:t5 0x210:rd:t8:t6 0x210:rt:s0:ra 0x214:rd:t5:t3 0x214:rs:t7:t8 0x218:rd:ra:t5 0x218:rt:t9:t7 0x220:rd:s0:ra 0x220:rs:t8:t6 0x22c:rt:t0:a3 0x230:rd:t3:t1 0x230:rt:a2:a1 0x234:rt:a3:a2 0x238:rd:a2:a1 0x238:rt:t0:a3 0x23c:rd:a3:a2 0x23c:rt:t1:t0 0x240:rd:t7:t8 0x240:rt:a2:a1 0x244:rd:t8:t6 0x244:rt:a3:a2 0x248:rd:a2:a1 0x248:rs:t7:t8 0x24c:rd:a3:a2 0x24c:rs:t8:t6 0x250:rt:t7:t6 0x258:rd:t6:t9 0x258:rt:t3:t1 0x25c:rd:t0:a3 0x260:rd:t3:t1 0x260:rs:t6:t9 0x264:rd:t9:t7 0x268:rd:t6:t9 0x268:rt:t0:a3 0x26c:rd:t1:t0 0x26c:rt:t5:t3 0x270:rs:t7:t8 0x270:rt:t8:t6 0x274:rs:t9:t7 0x278:rd:t0:a3 0x278:rs:t6:t9 0x27c:rd:t5:t3 0x280:rd:t9:t7 0x280:rt:t1:t0 0x284:rd:t6:t9 0x288:rd:t7:t8 0x288:rt:t5:t3 0x28c:rd:t1:t0 0x28c:rs:t9:t7 0x290:rt:t6:t9 0x298:rd:t5:t3 0x298:rs:t7:t8 0x29c:rt:t0:a3 0x2a4:rt:a2:a1 0x2a8:rd:t3:t1 0x2a8:rt:a2:a1 0x2ac:rt:a3:a2 0x2b0:rd:t8:t6 0x2b0:rt:t3:t1 0x2b4:rd:t6:t9 0x2b8:rd:t3:t1 0x2b8:rs:t8:t6 0x2bc:rs:t6:t9 0x2c0:rt:t8:t9 0x2c8:rd:a2:a1 0x2c8:rt:t0:a3 0x2cc:rd:t9:t7 0x2cc:rt:a2:a1 0x2d0:rd:a3:a2 0x2d0:rt:t1:t0 0x2d4:rd:t0:a3 0x2d4:rs:t8:t6 0x2d4:rt:t6:t9 0x2d8:rd:a2:a1 0x2d8:rs:t9:t7 0x2dc:rd:t1:t0 0x2e0:rd:t7:t8 0x2e0:rt:a3:a2 0x2e4:rd:t9:t7 0x2e4:rt:t0:a3 0x2e8:rd:t8:t6 0x2e8:rt:t1:t0 0x2ec:rd:a3:a2 0x2ec:rs:t7:t8 0x2f0:rd:t0:a3 0x2f0:rt:t9:t7 0x2f8:rd:t1:t0 0x2f8:rs:t8:t6 0x2fc:rt:a2:a1 0x304:rt:t3:t1 0x30c:rt:t7:t6 0x310:rd:t3:t1 0x310:rt:a2:a1 0x314:rd:t6:t9 0x314:rt:t3:t1 0x318:rt:a3:a2 0x31c:rd:a2:a1 0x31c:rs:t7:t8 0x31c:rt:t8:t6 0x320:rd:t3:t1 0x320:rs:t6:t9 0x324:rd:a3:a2 0x328:rd:t9:t7 0x32c:rd:t6:t9 0x32c:rt:a2:a1 0x330:rd:t7:t8 0x330:rt:a3:a2 0x334:rs:t9:t7 0x338:rd:a2:a1 0x338:rt:t6:t9 0x340:rd:a3:a2 0x340:rs:t7:t8 0x344:rt:t3:t1 0x350:rd:t3:t1 0x358:rd:t8:t6 0x358:rt:t3:t1 0x35c:rd:t6:t9 0x360:rd:t3:t1 0x360:rs:t8:t6 0x364:rs:t6:t9 0x36c:rd:t9:t7 0x370:rt:t9:t7 0x380:rt:s0:ra 0x384:rt:a2:a1 0x38c:rt:t8:t6 0x390:rt:s2:s1 0x394:rt:s1:s0 0x398:rt:ra:t5 0x39c:rt:t5:t3 0x3a4:rt:t3:t1 0x3a8:rt:t1:t0 0x3ac:rt:t0:a3 0x3b0:rt:a3:a2 0x3b4:rt:t8:t6 0x46c:rt:t6:t9 0x470:rs:t6:t9 0x470:rt:t6:t9 0x478:rt:t9:t7 0x47c:rs:t9:t7 0x47c:rt:t9:t7 0x480:rt:t6:t9 0x488:rt:t7:t8 0x48c:rs:t7:t8 0x48c:rt:t7:t8 0x490:rt:t9:t7 0x4ac:rt:t7:t8 0x4bc:rt:t8:t6 0x4c4:rt:t8:t6 0x4c8:rt:t6:t9 0x4cc:rs:t6:t9 0x4cc:rt:t6:t9 0x4e8:rs:t6:t9 0x4e8:rt:t9:t7 0x4ec:rt:t9:t7 */
/* Address: 0x80142014 */
/* Size: 1256 bytes (314 instructions) */
#include "types.h"
#include "tokimeki/class_w.h"

typedef struct {
    u8           unk0[446];
    volatile s16 base;
    s16          unk448;
    volatile s16 bonus;
    s16          unk452;
    s32          flags;
} GekoRec;

typedef struct {
    s16 e0;
    s16 e1;
    s16 e2;
    s16 e3;
    s16 e4;
    s16 e5;
} Rank6;

extern void draw2d3d(s32 a, s32 b);
extern void class_switch(s32 a);
extern void prim_init(void);
extern void class_clear_all(s32 a);
extern void back_clear_switch(s32 a);
extern void tpage_buf_clear(void);
extern void text_sys_init(void);
extern void basyo_init(u8 *p);
extern void bustup_ptr_init(void);
extern void geko_ptr_tbl_init(void);
extern void hizuke_init(void);
extern void message_window_init(void);
extern void yazirushi_off(void);
extern void date_name_set(s32 step);
extern void shared_system_state_reset(void);
extern void strcpy(u8 *a, u8 *b);
extern void sub_sub_step_up(void);

extern u8 screen_w[];
extern Rank6 geko_ranking_init_tbl0;
extern Rank6 geko_ranking_init_tbl1;
extern s8 girl_no;
extern u8 now_girl;
extern s8 cw3_disp;
extern s16 now_serifuev;
extern s16 now_serifuev_in;
extern s8 fade_bri;
extern s32 geko_ss_serifuev;
extern s32 geko_ss_serifuev_kind;
extern s32 geko_sss_soundev;
extern s8 geko_bustup_kuchi_flag;
extern u8 now_event_no;
extern u8 ev_flag52;
extern u8 ba_string[];
extern u8 geko_ranking_basyo_str[];
extern u8 geko_ranking_init_str0[];
extern CLASS_W class_w[];
extern s32 geko_serifuev_tbl[];
extern s32 geko_serifuev_kind_tbl[];
extern s32 geko_soundev_tbl[];

void geko_ranking_init(void)
{
    Rank6 sc;
    Rank6 id;
    GekoRec *p;
    s32 v;
    s16 i;
    s32 c56;
    s16 s0, s1, s2, s3, s4, s5;
    s16 i0, i1, i2, i3, i4, i5;

    sc = geko_ranking_init_tbl0;
    id = geko_ranking_init_tbl1;
    s0 = sc.e0; s1 = sc.e1; s2 = sc.e2;
    s3 = sc.e3; s4 = sc.e4; s5 = sc.e5;
    i0 = id.e0; i1 = id.e1; i2 = id.e2;
    i3 = id.e3; i4 = id.e4; i5 = id.e5;
    girl_no = 1;
    c56 = 56;
    for (i = 9; i > 0; i--) {
        p = (GekoRec *)(screen_w + i * c56);
        if ((p->flags << 30) < 0) {
            v = p->base + p->bonus;
            if (v >= s0) {
                s5 = s4; i5 = i4;
                s4 = s3; i4 = i3;
                s3 = s2; i3 = i2;
                s2 = s1; i2 = i1;
                s1 = s0; i1 = i0;
                s0 = p->base + p->bonus; i0 = i;
            } else if (v >= s1) {
                s5 = s4; i5 = i4;
                s4 = s3; i4 = i3;
                s3 = s2; i3 = i2;
                s2 = s1; i2 = i1;
                s1 = p->base + p->bonus; i1 = i;
            } else if (v >= s2) {
                s5 = s4; i5 = i4;
                s4 = s3; i4 = i3;
                s3 = s2; i3 = i2;
                s2 = p->base + p->bonus; i2 = i;
            } else if (v >= s3) {
                s5 = s4; i5 = i4;
                s4 = s3; i4 = i3;
                s3 = p->base + p->bonus; i3 = i;
            } else if (v >= s4) {
                s5 = s4; i5 = i4;
                s4 = p->base + p->bonus; i4 = i;
            } else if (v >= s5) {
                s5 = v; i5 = i;
            }
        }
    }
    girl_no = i5;
    sc.e0 = s0; sc.e1 = s1; sc.e2 = s2;
    sc.e3 = s3; sc.e4 = s4; sc.e5 = s5;
    id.e0 = i0; id.e1 = i1; id.e2 = i2;
    id.e3 = i3; id.e4 = i4; id.e5 = i5;
    now_girl = 13;
    draw2d3d(1, 0);
    class_switch(1);
    prim_init();
    class_clear_all(0);
    back_clear_switch(1);
    tpage_buf_clear();
    text_sys_init();
    basyo_init(geko_ranking_basyo_str);
    cw3_disp = 0;
    (*(s8 *)&class_w[4].disp_flag) = 0;
    now_serifuev = 0;
    now_serifuev_in = 0;
    bustup_ptr_init();
    geko_ptr_tbl_init();
    hizuke_init();
    message_window_init();
    yazirushi_off();
    fade_bri = 0;
    date_name_set(now_girl);
    shared_system_state_reset();
    geko_ptr_tbl_init();
    geko_ss_serifuev = geko_serifuev_tbl[78];
    geko_ss_serifuev_kind = geko_serifuev_kind_tbl[78];
    geko_sss_soundev = geko_soundev_tbl[78];
    strcpy(ba_string, geko_ranking_init_str0);
    geko_bustup_kuchi_flag = 0;
    now_event_no = 255;
    sub_sub_step_up();
    ev_flag52 = ev_flag52 + 1;
}
