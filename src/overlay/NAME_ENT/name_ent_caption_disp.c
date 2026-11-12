/* @PostProc: patch_stack_complete --old 288 --new 304 */
/* @PostProc: remap_stack_slots --map '56:72,68:292,72:76,73:77,74:78,75:79,107:111,108:112,109:113,110:114,111:115,144:148,145:149,146:150,147:151,180:184,181:185,182:186,183:187,216:220,217:221,218:222,219:223,252:256,253:257,254:258,255:259' */
/* @PostProc: patch_insn_reg_field 0x140:rt:t1:t3 0x144:rd:v0:t1 0x148:rs:v0:t1 0x148:rt:v0:t2 0x150:rs:v0:t2 0x154:rt:v0:t2 0x160:rs:t1:t3 0x160:rt:t1:t3 0x168:rs:t1:t3 0x174:rt:t2:t4 0x178:rs:t2:t4 0x178:rt:t2:t4 0x180:rt:t4:t5 0x184:rs:t4:t5 0x184:rt:t4:t5 0x18c:rs:t2:t4 0x18c:rt:t3:a0 0x190:rd:t5:t6 0x190:rt:t4:t5 0x194:rs:t3:a0 0x194:rt:t5:t6 0x1bc:rt:t6:t7 0x200:rt:t6:t7 0x240:rt:t6:t7 0x280:rt:t6:t7 0x2c0:rt:t6:t7 0x300:rt:t6:t7 0x30c:rt:t6:t7 0x314:rs:t6:t7 0x32c:rt:t7:t8 0x334:rs:t7:t8 0x34c:rt:t8:t9 0x354:rs:t8:t9 0x36c:rt:t9:t1 0x374:rs:t9:t1 0x38c:rt:t1:t2 0x394:rs:t1:t2 0x3ac:rt:t2:t3 0x3b4:rs:t2:t3 */
#include "types.h"

extern s32 name_ent_kanji_row;
extern s32 name_ent_kanji_len;
extern u16 *name_ent_kanji_cnt;
extern u8 **name_ent_kanji_txt;

extern s32 strlen(u8 *);
extern void strcpy(u8 *, u8 *);
extern void strncpy();
extern void text_sys_init(void);
extern void set_kanji_string(s32, s32, s32, u8 *, s32);

void name_ent_caption_disp(u8 *caption)
{
    u8 b0[35];
    u8 b1[35];
    u8 b2[35];
    u8 b3[35];
    u8 b4[35];
    u8 b5[35];
    u8 *p;
    s32 i;
    s32 m;

    for (i = 0; i < 35; i++) {
        b0[i] = 0;
        b1[i] = 0;
        b2[i] = 0;
        b3[i] = 0;
        b4[i] = 0;
        b5[i] = 0;
    }

    text_sys_init();
    set_kanji_string(-152, -104, 10, caption, 0);

    if ((u32)name_ent_kanji_cnt[38] < (u32)(name_ent_kanji_row * 16 + 80)) {
        m = ((u32)name_ent_kanji_cnt[38] >> 4) - 5;
        name_ent_kanji_row = m;
        if (m < 0) {
            name_ent_kanji_row = 0;
        }
    }

    name_ent_kanji_len = strlen(name_ent_kanji_txt[38]);
    p = name_ent_kanji_txt[38] + name_ent_kanji_row * 32;

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
    if (strlen(p + 160) < 32) {
        strcpy(b5, p + 160);
        goto done;
    }
    strncpy(b5, p + 160);
done:
    if (b0[0] != 0) {
        set_kanji_string(-120, -48, 10, b0, 0);
    }
    if (b1[0] != 0) {
        set_kanji_string(-120, -32, 10, b1, 0);
    }
    if (b2[0] != 0) {
        set_kanji_string(-120, -16, 10, b2, 0);
    }
    if (b3[0] != 0) {
        set_kanji_string(-120, 0, 10, b3, 0);
    }
    if (b4[0] != 0) {
        set_kanji_string(-120, 16, 10, b4, 0);
    }
    if (b5[0] != 0) {
        set_kanji_string(-120, 32, 10, b5, 0);
    }
}
