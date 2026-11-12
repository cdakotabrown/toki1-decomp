/* Address: 0x80137640 */
/* Size: 560 bytes (140 insns) */
/* @PostProc: swap_branch_operands --offsets 0x4C,0x18C,0x1AC */
/* @PostProc: patch_insn_reg_field 0x94:rt:v1:v0 0xa0:rd:a0:a2 0xa4:rs:v1:v0 0xa4:rt:v1:v0 0xa8:rt:a1:a0 0xac:rs:v1:v0 0xb0:rt:a2:a1 0xb4:rs:v1:v0 0xbc:rs:v1:v0 0xc8:rs:v1:v0 0xd0:rs:v1:v0 0xd8:rs:v1:v0 0xe4:rs:v1:v0 0xf0:rs:v1:v0 0x118:rt:a1:a0 0x11c:rt:a2:a1 0x134:rs:a1:a0 0x138:rs:a2:a1 0x13c:rd:t8:t9 0x140:rd:t3:t5 0x144:rt:t8:t9 0x148:rt:t3:t5 0x154:rs:a1:a0 0x154:rt:a1:a0 0x164:rt:a0:a2 0x168:rt:t5:t7 0x170:rs:t5:t7 0x174:rs:a0:a2 0x174:rt:a0:a2 0x178:rs:a0:a2 0x17c:rs:a2:a1 0x17c:rt:a2:a1 0x180:rt:t6:t8 0x184:rs:t6:t8 0x184:rt:t6:t8 0x18c:rt:t6:t8 0x190:rt:t7:t9 0x194:rs:t7:t9 0x194:rt:t7:t9 0x198:rt:t8:t3 0x19c:rt:t7:t9 0x1a4:rs:t8:t3 0x1a4:rt:t8:t3 0x1ac:rt:t8:t3 0x1c0:rt:t9:t4 0x1cc:rt:t9:t4 0x1d8:rt:t9:t4 0x1dc:rt:t3:t5 0x1e0:rs:t3:t5 0x1e0:rt:t3:t5 0x1e8:rt:t3:t5 0x1f8:rt:t4:t6 0x200:rt:t4:t6 0x208:rt:t5:t7 0x20c:rt:t5:t7 0x214:rt:t6:t8 0x21c:rt:t6:t8 */
#include "types.h"
#include "tokimeki/taiiku_kari_run.h"

typedef struct {
    u8  pad00[38];
    s16 f38;
    u8  pad40[2];
    s16 f42;
    u8  pad44[24];
} Rec68;

extern Rec68 class_w[];
extern u8  date2_menu_select_tbl1;
extern s32 pad_trig;
extern s32 taiiku_konami_mode;
extern s32 load_done_flag;
extern u8  taiiku_kari_gseat;
extern u8  taiiku_kari_girl;
extern TAIIKU_KARI_RUN taiiku_kari_run[];

extern void Yubi();
extern void device_cmd_send();

void taiiku_ouen_cue(void)
{
    s32 v;
    s32 t;
    s32 i;
    s32 x;
    s32 y;
    s32 lim;
    s16 lo;
    s16 hi;

    Yubi(106);
    if (date2_menu_select_tbl1 != 0 && taiiku_konami_mode == 255) {
        if (load_done_flag == 1) {
            device_cmd_send(1550);
        } else {
            device_cmd_send(1294);
        }
        taiiku_konami_mode = 1;
        return;
    }
    if ((pad_trig & 0x20) == 0) {
        return;
    }
    if (taiiku_kari_run[0].tick < 41U) {
        return;
    }
    i = 0;
    v = class_w[106].f42;
    x = -160;
    y = -128;
    if (v < 0 && v >= -48) {
        t = 0;
    } else if (v < -48 && v >= -64) {
        t = 10;
    } else if (v < -64 && v >= -80) {
        t = 20;
    } else {
        t = 128;
    }
    lim = class_w[106].f38 + 11;
    for (i = 0; i < 10; i++) {
        lo = (s16)x;
        hi = (s16)y;
        if (t == 10U) {
            lo = (s16)(x + 16);
            hi = (s16)(y + 16);
        }
        if (lo < lim && lim < hi) {
            t = (t + i) & 0xFF;
            break;
        }
        x += 32;
        y += 32;
    }
    if (taiiku_konami_mode == 1 && t == taiiku_kari_gseat) {
        if (load_done_flag == 1) {
            device_cmd_send(1548);
        } else {
            device_cmd_send(1292);
        }
        taiiku_kari_run[0].cue = 1;
    } else if (t == taiiku_kari_run[0].seat) {
        device_cmd_send(1537);
        taiiku_kari_run[0].cue = 1;
        taiiku_konami_mode = 255;
        taiiku_kari_girl = 255;
    } else {
        device_cmd_send(1536);
    }
    taiiku_kari_run[0].tick = 0;
}
