/* @SymbolSize: 1220 */
/* @PostProc: patch_stack_complete --old 256 --new 272 */
/* @PostProc: remap_stack_slots --map '72:256,52:68' */
/* @PostProc: patch_insn_reg_field 0x184:rt:v0:v1 0x188:rs:v0:v1 0x188:rt:v0:v1 0x190:rs:v0:v1 0x194:rs:v0:v1 0x1a0:rd:v0:v1 0x1ac:rt:v0:v1 0x1c0:rt:v0:v1 0x1c8:rs:v0:v1 0x214:rt:v0:v1 0x218:rs:v0:v1 0x218:rt:v0:v1 0x220:rt:v0:v1 0x2a0:rt:t4:t7 0x2a4:rd:v0:t4 0x2a8:rs:v0:t4 0x2a8:rt:v0:t6 0x2b0:rs:v0:t6 0x2b4:rt:v0:t6 0x2c0:rt:t6:t9 0x2c4:rs:t6:t9 0x2c4:rt:t6:t9 0x2c8:rs:t4:t7 0x2c8:rt:t4:t7 0x2cc:rt:t0:t1 0x2d0:rd:t7:t8 0x2d0:rt:t6:t9 0x2d4:rs:t0:t1 0x2d4:rt:t0:t1 0x2d8:rd:t9:t0 0x2d8:rs:t4:t7 0x2d8:rt:t7:t8 0x2dc:rs:t9:t0 0x2dc:rt:t8:a0 0x2e0:rd:t1:t2 0x2e0:rt:t0:t1 0x2e4:rs:t8:a0 0x2e4:rt:t1:t2 0x30c:rt:t2:t3 0x350:rt:t2:t3 0x390:rt:t2:t3 0x3d0:rt:t2:t3 0x410:rt:t2:t3 0x41c:rt:t2:t3 0x424:rs:t2:t3 0x43c:rt:t3:t5 0x444:rs:t3:t5 0x45c:rt:t5:t4 0x464:rs:t5:t4 0x49c:rt:t4:t9 0x4a4:rs:t4:t9 */
#include "types.h"

extern u8 now_sub_sub_step;

extern u8 name_ent_sei_ask_str[];
extern u8 name_ent_mei_ask_str[];
extern u8 name_ent_adana_ask_str[];
extern u8 name_ent_desc_disp_str0[];
extern u8 *name_ent_now_ask;
extern s32 name_ent_yomi_no;
extern s32 name_ent_tab_no;
extern s32 name_ent_kanji_row;
extern s32 name_ent_kanji_len;
extern u8 *name_ent_yomi_tbl[];
extern u8 **name_ent_kanji_txt;
extern u16 *name_ent_kanji_cnt;

extern void printf();
extern s32 strlen(u8 *);
extern void strcpy(u8 *, u8 *);
extern void strncpy();
extern void name_ent_caption_disp(u8 *);
extern void text_sys_init(void);
extern void set_kanji_string(s32, s32, s32, u8 *, s32);
extern void name_ent_grid9_disp(void);

void name_ent_desc_disp(s32 arg0)
{
    u8 b0[35];
    u8 b1[35];
    u8 b2[35];
    u8 b3[35];
    u8 b4[35];
    u8 *p;
    s32 i;
    s32 n;
    s32 m;

    switch (arg0) {
    case 0:
        if (now_sub_sub_step == 1) {
            name_ent_now_ask = name_ent_sei_ask_str;
        }
        if (now_sub_sub_step == 3) {
            name_ent_now_ask = name_ent_mei_ask_str;
        }
        if (now_sub_sub_step == 5) {
            name_ent_now_ask = name_ent_adana_ask_str;
        }
        break;
    case 1:
        name_ent_now_ask = name_ent_sei_ask_str;
        break;
    case 2:
        name_ent_now_ask = name_ent_mei_ask_str;
        break;
    case 3:
        name_ent_now_ask = name_ent_adana_ask_str;
        break;
    }

    for (i = 0; i < 35; i++) {
        b0[i] = 0;
        b1[i] = 0;
        b2[i] = 0;
        b3[i] = 0;
        b4[i] = 0;
    }

    if (name_ent_yomi_no < 0 || name_ent_yomi_no >= 45) {
        name_ent_yomi_no = 0;
    }
    if (name_ent_tab_no != 2) {
        name_ent_yomi_no = name_ent_yomi_no;
        if (name_ent_yomi_no != 38) {
            goto main;
        }
    }
    printf(name_ent_desc_disp_str0);
    name_ent_caption_disp(name_ent_now_ask);
    goto tail;

main:
    text_sys_init();
    set_kanji_string(-152, -104, 10, name_ent_now_ask, 0);
    set_kanji_string(-120, -48, 10, name_ent_yomi_tbl[name_ent_yomi_no], 0);
    name_ent_kanji_len = strlen(name_ent_kanji_txt[name_ent_yomi_no]);
    if ((u32)name_ent_kanji_cnt[name_ent_yomi_no] < (u32)(name_ent_kanji_row * 16 + 64)) {
        m = ((u32)name_ent_kanji_cnt[name_ent_yomi_no] >> 4) - 4;
        name_ent_kanji_row = m;
        if (m < 0) {
            name_ent_kanji_row = 0;
        }
    }
    p = name_ent_kanji_txt[name_ent_yomi_no] + name_ent_kanji_row * 32;
    if (strlen(p) < 32) {
        strcpy(b0, p);
        goto done;
    }
    strncpy(b0, p, 32);
    if (strlen(p + 32) < 32) {
        strcpy(b1, p + 32);
        goto done;
    }
    strncpy(b1, p + 32, 32);
    if (strlen(p + 64) < 32) {
        strcpy(b2, p + 64);
        goto done;
    }
    strncpy(b2, p + 64, 32);
    if (strlen(p + 96) < 32) {
        strcpy(b3, p + 96);
        goto done;
    }
    strncpy(b3, p + 96, 32);
    if (strlen(p + 128) < 32) {
        strcpy(b4, p + 128);
        goto done;
    }
    strncpy(b4, p + 128);
done:
    if (b0[0] != 0) {
        set_kanji_string(-120, -32, 10, b0, 0);
    }
    if (b1[0] != 0) {
        set_kanji_string(-120, -16, 10, b1, 0);
    }
    if (b2[0] != 0) {
        set_kanji_string(-120, 0, 10, b2, 0);
    }
    if (b3[0] != 0) {
        set_kanji_string(-120, 16, 10, b3, 0);
    }
    if (b4[0] != 0) {
        set_kanji_string(-120, 32, 10, b4, 0);
    }
tail:
    name_ent_grid9_disp();
}
