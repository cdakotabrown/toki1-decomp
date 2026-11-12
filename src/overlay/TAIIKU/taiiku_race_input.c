/* @SymbolStart: 0x801440FC */
/* @SymbolSize: 956 */
/* @PostProc: swap_instructions --offset1 0x338 --offset2 0x344 */
/* @PostProc: swap_instructions --offset1 0x5c --offset2 0x64 */
/* @PostProc: swap_instructions --offset1 0x16c --offset2 0x174 */
/* @PostProc: swap_instructions --offset1 0x294 --offset2 0x29c */
/* @PostProc: patch_insn_reg_field 0x5c:rt:t0:v0 0x74:rs:t0:v0 0x74:rt:t0:v0 0x80:rs:t0:v0 0x88:rt:t1:t0 0x8c:rs:t1:t0 0x8c:rt:t1:t0 0x90:rt:t2:t1 0x94:rs:t1:t0 0xa0:rt:t2:t1 0xbc:rt:t3:t2 0xc8:rt:t3:t2 0xcc:rt:t4:t3 0xd0:rs:t4:t3 0xd0:rt:t4:t3 0xd4:rt:t5:t4 0xd8:rs:t4:t3 0xe4:rt:t5:t4 0x100:rt:t6:t5 0x108:rt:t6:t5 0x148:rt:t7:t6 0x14c:rs:t7:t6 0x154:rt:t8:t7 0x15c:rt:t8:t7 0x164:rt:t9:t8 0x168:rs:t9:t8 0x168:rt:t9:t8 0x16c:rt:t0:v0 0x170:rs:t9:t8 0x184:rs:t0:v0 0x184:rt:t0:v0 0x190:rs:t0:v0 0x198:rt:t1:t9 0x19c:rs:t1:t9 0x19c:rt:t1:t9 0x1a0:rt:t2:t0 0x1a4:rs:t1:t9 0x1b0:rt:t2:t0 0x1cc:rt:t3:t1 0x1d8:rt:t3:t1 0x1dc:rt:t4:t2 0x1e0:rs:t4:t2 0x1e0:rt:t4:t2 0x1e4:rt:t5:t3 0x1e8:rs:t4:t2 0x1f4:rt:t5:t3 0x210:rt:t6:t4 0x218:rt:t6:t4 0x240:rt:t7:t5 0x244:rs:t7:t5 0x244:rt:t7:t5 0x24c:rs:t7:t5 0x258:rt:t8:t6 0x268:rs:t8:t6 0x268:rt:t8:t6 0x270:rs:t8:t6 0x274:rt:t9:t7 0x278:rs:t9:t7 0x278:rt:t9:t7 0x280:rs:t9:t7 0x284:rt:t0:t8 0x290:rs:t0:t8 0x290:rt:t0:t8 0x294:rt:t1:v0 0x298:rs:t0:t8 0x2b0:rs:t1:v0 0x2b0:rt:t1:v0 0x2bc:rs:t1:v0 0x2dc:rt:t2:t9 0x2e8:rt:t2:t9 0x304:rt:t3:t0 0x30c:rt:t3:t0 0x320:rt:t4:t1 0x324:rs:t4:t1 0x324:rt:t4:t1 0x328:rt:t5:t2 0x32c:rs:t4:t1 0x334:rs:t5:t2 0x334:rt:t5:t2 0x33c:rs:t5:t2 0x344:rt:t6:t3 0x354:rt:t6:t3 0x368:rt:t7:t4 0x374:rt:t7:t4 0x378:rt:t8:t5 0x380:rt:t8:t5 0x384:rt:t9:t6 0x390:rt:t9:t6 0x394:rt:t0:t7 0x39c:rt:t0:t7 0x3b0:rt:t1:t8 0x3b8:rt:t1:t8 */
/* Address: 0x801440FC (head) / 0x80144104 */
/* Size: 956 bytes (239 insns) */
#include "types.h"

typedef struct {
    u8  pad00[3];
    u8  f03;
    s16 f04;
    u8  pad06[18];
    s16 f24;
    u8  pad26[42];
} Rec68;

typedef struct {
    u8  f00;
    u8  pad01;
    u8  f02;
    u8  pad03;
    s32 f04;
    u8  pad08[4];
    s32 f12;
    u8  pad16[8];
    s16 f24;
    u16 f26;
    s32 f28;
    s32 f32;
} Rec36;

extern Rec68 class_w[];
extern s32   pad_trig;
extern u8    taiiku_ninin_girl1;
static u8 pad_align[12] = {0};
Rec36 taiiku_ninin_run = {0};

extern void device_cmd_send(s32);

void taiiku_race_input(void)
{
    u32 a = taiiku_ninin_run.f00;
    s32 d;

    if (a >= 2 && a < 6) {
        d = pad_trig;
        if (d & 5) {
            taiiku_ninin_run.f32 = 1;
        }
        if (d & 10) {
            if (taiiku_ninin_run.f28 != 0) {
                taiiku_ninin_run.f28 = 0;
                taiiku_ninin_run.f26 = 0;
                taiiku_ninin_run.f02 = 0;
                taiiku_ninin_run.f12 = 0;
                if (taiiku_ninin_girl1 != 0) {
                    if (taiiku_ninin_run.f32 != 0) {
                        class_w[96].f24 = 17;
                    } else {
                        device_cmd_send(6149);
                        device_cmd_send(6148);
                        device_cmd_send(2054);
                        class_w[96].f24 = 16;
                    }
                } else {
                    if (taiiku_ninin_run.f32 != 0) {
                        class_w[96].f24 = 8;
                    } else {
                        device_cmd_send(6146);
                        device_cmd_send(6145);
                        device_cmd_send(2051);
                        class_w[96].f24 = 7;
                    }
                }
                taiiku_ninin_run.f32 = 0;
                class_w[100].f03 = 0;
                device_cmd_send(1290);
                a = taiiku_ninin_run.f00;
            }
        }
    }

    if (a >= 6 && a < 10) {
        d = pad_trig;
        if (d & 10) {
            taiiku_ninin_run.f32 = 1;
        }
        if (d & 5) {
            if (taiiku_ninin_run.f28 != 0) {
                taiiku_ninin_run.f28 = 0;
                taiiku_ninin_run.f26 = 0;
                taiiku_ninin_run.f02 = 0;
                taiiku_ninin_run.f12 = 0;
                if (taiiku_ninin_girl1 != 0) {
                    if (taiiku_ninin_run.f32 != 0) {
                        class_w[96].f24 = 17;
                    } else {
                        device_cmd_send(6149);
                        device_cmd_send(6148);
                        device_cmd_send(2054);
                        class_w[96].f24 = 16;
                    }
                } else {
                    if (taiiku_ninin_run.f32 != 0) {
                        class_w[96].f24 = 8;
                    } else {
                        device_cmd_send(6146);
                        device_cmd_send(6145);
                        device_cmd_send(2051);
                        class_w[96].f24 = 7;
                    }
                }
                taiiku_ninin_run.f32 = 0;
                class_w[100].f03 = 0;
                device_cmd_send(1290);
                a = taiiku_ninin_run.f00;
            }
        }
    }

    if (taiiku_ninin_run.f04 == 0) {
        if (a == 5 || a == 9) {
            if (taiiku_ninin_run.f02 == 1) {
                if (taiiku_ninin_run.f32 != 0) {
                    taiiku_ninin_run.f32 = 0;
                } else if (taiiku_ninin_run.f28 != 0) {
                    taiiku_ninin_run.f32 = 0;
                    taiiku_ninin_run.f28 = 0;
                    taiiku_ninin_run.f26 = 0;
                    taiiku_ninin_run.f02 = 0;
                    taiiku_ninin_run.f12 = 0;
                    if (taiiku_ninin_girl1 != 0) {
                        device_cmd_send(6149);
                        device_cmd_send(6148);
                        device_cmd_send(2054);
                        class_w[96].f24 = 15;
                    } else {
                        device_cmd_send(6146);
                        device_cmd_send(6145);
                        device_cmd_send(2051);
                        class_w[96].f24 = 6;
                    }
                    class_w[100].f03 = 0;
                    device_cmd_send(1290);
                }
            }
        }
    }

    if (taiiku_ninin_run.f28 == 0) {
        if (taiiku_ninin_run.f26 >= 30U) {
            taiiku_ninin_run.f32 = 0;
            taiiku_ninin_run.f28 = 1;
            taiiku_ninin_run.f24 = 0;
            taiiku_ninin_run.f00 = 0;
            if (taiiku_ninin_girl1 != 0) {
                class_w[96].f24 = 14;
            } else {
                class_w[96].f24 = 5;
            }
            class_w[100].f03 = 164;
            if (taiiku_ninin_girl1 == 0) {
                class_w[100].f24 = 45;
                device_cmd_send(2048);
            } else {
                class_w[100].f24 = 57;
            }
        }
    }
}
