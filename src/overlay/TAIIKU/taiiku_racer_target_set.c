/* @CompileFlags: -Wc,-pic0 */
/* @CompileEnv: IDO52_NO_LOOP_TEST_REPLACEMENT=occ:1 */
/* @PostProc: patch_insn_reg_field 0x18c:rd:v1:a2 0x190:rs:v1:a2 0x190:rt:v1:a2 0x194:rs:v1:a2 0x19c:rd:v1:a2 0x1d8:rd:v1:a2 0x1dc:rs:v1:a2 0x1dc:rt:v1:a2 0x1e0:rs:v1:a2 0x1e4:rd:v1:a2 0x1f4:rs:v1:a2 0x214:rt:at:s1 0x218:rt:s1:at 0x230:rt:v1:a2 0x26c:rd:v1:t6 0x270:rs:v1:t6 0x270:rt:t6:t1 0x274:rt:t6:t1 0x278:rt:v1:t6 0x2ac:rd:v1:t4 0x2b0:rs:v1:t4 0x2b0:rt:t1:t7 0x2b4:rt:t1:t7 0x2b8:rt:v1:t4 0x2ec:rt:t4:t8 0x2f0:rt:t7:t5 0x2f4:rs:t7:t5 0x2f4:rt:t7:t5 0x2f8:rs:t4:t8 0x2f8:rt:t4:t8 0x2fc:rd:t8:t9 0x2fc:rt:t7:t5 0x300:rd:t5:t0 0x300:rt:t8:t9 0x304:rs:t5:t0 0x304:rt:t9:t3 0x30c:rs:t4:t8 0x30c:rt:t9:t3 0x328:rt:t3:t6 0x32c:rt:t6:t4 0x330:rs:t6:t4 0x330:rt:t6:t4 0x338:rd:t1:t7 0x338:rt:t6:t4 0x33c:rd:t7:t5 0x33c:rt:t1:t7 0x340:rd:t0:t2 0x344:rt:t0:t2 0x348:rs:t3:t6 0x348:rt:t3:t6 0x34c:rs:t7:t5 0x34c:rt:t8:t9 0x350:rs:t3:t6 0x358:rd:t2:t1 0x35c:rd:a0:v1 0x35c:rt:t2:t1 0x360:rs:a0:v1 0x360:rt:t8:t9 0x364:rs:a0:v1 0x364:rt:t5:t0 0x36c:rd:t4:t8 0x36c:rt:t5:t0 0x370:rt:t4:t8 0x374:rs:a0:v1 0x374:rt:v1:a0 0x37c:rd:t9:t3 0x37c:rt:v1:a0 0x380:rd:t0:t2 0x380:rt:t9:t3 0x384:rd:t3:t6 0x384:rt:t0:t2 0x388:rs:v1:a0 0x388:rt:t2:t1 0x38c:rd:t6:t4 0x38c:rs:t3:t6 0x38c:rt:t2:t1 0x390:rs:t6:t4 0x390:rt:t1:t7 0x398:rt:t1:t7 0x3a8:rd:t7:t5 0x3ac:rd:t7:t5 0x3ac:rs:t7:t5 0x3b0:rt:t8:t9 0x3b4:rs:t8:t9 0x3b4:rt:t8:t9 0x3b8:rd:t7:t5 0x3b8:rt:t7:t5 0x3bc:rs:t7:t5 0x3bc:rt:t8:t9 0x3c0:rt:t5:t0 0x3c4:rd:t0:t2 0x3c8:rd:t4:t8 0x3c8:rt:t5:t0 0x3cc:rd:t4:t8 0x3cc:rs:t4:t8 0x3cc:rt:t5:t0 0x3d0:rd:t4:t8 0x3d0:rt:t4:t8 0x3d4:rd:t0:t2 0x3d4:rs:t0:t2 0x3d8:rd:t0:t2 0x3d8:rt:t0:t2 0x3dc:rd:t9:t3 0x3dc:rt:t4:t8 0x3e8:rs:t0:t2 0x3e8:rt:t3:t6 0x3ec:rs:t9:t3 0x3f0:rs:t3:t6 0x3f0:rt:t8:t9 0x3fc:rt:t2:t1 0x400:rt:t5:t0 0x404:rs:t2:t1 0x408:rd:t6:t4 0x40c:rd:t1:t7 0x40c:rt:t6:t4 0x410:rs:t1:t7 0x410:rt:t7:t5 0x418:rt:t7:t5 0x420:rs:t5:t0 0x420:rt:t5:t0 0x428:rs:t5:t0 0x42c:rd:t4:t8 0x430:rd:t9:t3 0x430:rt:t4:t8 0x434:rs:t9:t3 0x434:rt:t0:t2 0x43c:rt:t0:t2 0x474:rd:t3:t6 0x478:rs:t3:t6 0x478:rt:t8:t9 0x47c:rt:t3:t6 0x480:rs:t8:t9 0x484:rt:t1:t7 0x488:rd:t2:t1 0x48c:rd:t6:t4 0x48c:rt:t2:t1 0x490:rs:t6:t4 0x494:rs:t1:t7 0x498:rd:t7:t5 0x49c:rd:t5:t0 0x49c:rt:t7:t5 0x4a0:rs:t5:t0 0x4a0:rt:t4:t8 0x4a8:rt:t4:t8 0x4b0:rt:t9:t3 0x4b4:rs:t9:t3 0x4b4:rt:t9:t3 0x4bc:rs:t9:t3 0x4c0:rd:t0:t2 0x4c4:rd:t3:t6 0x4c4:rt:t0:t2 0x4c8:rs:t3:t6 0x4c8:rt:t8:t9 0x4d0:rt:t8:t9 0x50c:rt:t2:t1 0x514:rs:t2:t1 0x520:rd:t6:t4 0x524:rd:t1:t7 0x524:rt:t6:t4 0x528:rs:t1:t7 0x528:rt:t7:t5 0x530:rd:t5:t0 0x530:rt:t7:t5 0x538:rt:t5:t0 0x53c:rt:t4:a2 0x540:rs:t4:a2 0x540:rt:t4:a2 0x548:rs:t4:a2 0x570:rd:t9:t8 0x574:rd:t0:t3 0x574:rt:t9:t8 0x578:rs:t0:t3 0x578:rt:v1:v0 0x580:rd:t3:t2 0x580:rt:v1:v0 0x584:rd:t8:t6 0x584:rt:t3:t2 0x588:rd:t2:t9 0x588:rt:t8:t6 0x58c:rs:v1:v0 0x58c:rt:t6:t1 0x590:rd:t1:t4 0x590:rs:t2:t9 0x590:rt:t6:t1 0x594:rs:t1:t4 0x5ac:rd:t4:t0 0x5b0:rd:t9:t8 0x5b0:rt:t4:t0 0x5b4:rs:t9:t8 0x5bc:rd:t0:t3 0x5c0:rd:t3:t2 0x5c0:rt:t0:t3 0x5c4:rd:t8:t6 0x5c4:rt:t3:t2 0x5c8:rt:t2:t9 0x5cc:rd:t6:t1 0x5cc:rs:t8:t6 0x5cc:rt:t2:t9 0x5d0:rs:t6:t1 0x5d0:rt:t1:t4 0x5d8:rt:t1:t4 0x5dc:rt:t1:t4 0x5e0:rs:t1:t4 0x5e0:rt:t1:t4 0x5ec:rd:t4:t0 0x5f0:rs:t4:t0 0x5f0:rt:v1:a0 0x5f4:rs:t1:t4 0x5f8:rd:t9:t8 0x5f8:rt:v1:a0 0x5fc:rd:t0:t3 0x5fc:rt:t9:t8 0x600:rd:t3:t2 0x600:rt:t0:t3 0x604:rs:v1:a0 0x604:rt:t8:t6 0x608:rd:t2:t9 0x608:rs:t3:t2 0x608:rt:t8:t6 0x60c:rs:t2:t9 0x60c:rt:t6:t1 0x614:rt:t6:t1 0x624:rt:v1:a1 0x62c:rd:t4:t0 0x62c:rt:v1:a1 0x630:rd:t9:t8 0x630:rt:t4:t0 0x634:rd:t0:t3 0x634:rt:t9:t8 0x638:rs:v1:a1 0x638:rt:t3:t2 0x63c:rd:t8:t6 0x63c:rs:t0:t3 0x63c:rt:t3:t2 0x640:rs:t8:t6 0x640:rt:t2:t9 0x650:rt:t2:t9 */
/* Address: 0x80136080 */
/* Size: 1604 bytes (401 insns) */
#include "types.h"

typedef struct {
    u8  pad00[2];
    u8  f02;
    u8  pad03[33];
} Rec36;

typedef struct {
    u8  pad00[22];
    s16 f22;
    u8  pad24[43];
    u8  f67;
} Rec68;

typedef struct { s32 a[11]; } Tbl44;
typedef struct { s16 a[11]; } Tbl22;
typedef struct { u8  a[11][8]; } Tbl88;

extern Rec68 class_w[];
extern u8    taiiku_kari_gseat;
extern u8    taiiku_kari_girl;
extern s16   taiiku_kari_card;
extern Rec36 taiiku_kari_run[];
extern Tbl44 taiiku_kari_card_girl;
extern Tbl22 taiiku_kari_card_frm;
extern Tbl22 taiiku_kari_girl_frm;
extern Tbl88 taiiku_kari_frm_attr;

extern s32 rand(void);
extern s32 get_top_girl(void);
extern s32 get_g_zyotai_low(s32);

void taiiku_racer_target_set(void)
{
    s32 unused[3];
    Tbl44 ta;
    Tbl22 tb;
    Tbl22 tc;
    Tbl88 td;
    s32 i;
    s32 j;
    s32 v;
    u8 x;
    u8 c;

    ta = taiiku_kari_card_girl;
    tb = taiiku_kari_card_frm;
    tc = taiiku_kari_girl_frm;
    td = taiiku_kari_frm_attr;

    v = (rand() % 20 + 10) & 0xFF;
    if (v == 19) {
        taiiku_kari_run[0].f02 = v;
        do {
            v = (rand() % 20 + 10) & 0xFF;
            taiiku_kari_run[0].f02 = v;
        } while (v == 19);
    }
    class_w[v + 46].f22 = tb.a[taiiku_kari_card];
    taiiku_kari_run[0].f02 = v;

    for (i = 0; i < 3; i++) {
        x = (u8)(rand() % 30);
        taiiku_kari_run[i + 1].f02 = x;
        while (x == 19) {
            x = (u8)(rand() % 30);
            taiiku_kari_run[i + 1].f02 = x;
        }
    }

    c = (u8)get_top_girl();
    taiiku_kari_girl = c;
    if ((u32)get_g_zyotai_low(c) < 2) {
        if (taiiku_kari_girl != ta.a[taiiku_kari_card]) {
            taiiku_kari_gseat = (u8)(rand() % 10);
            class_w[taiiku_kari_gseat + 46].f22 = tc.a[taiiku_kari_girl];
            class_w[115].f22 = class_w[taiiku_kari_gseat + 46].f22 >> 3;
            v = class_w[taiiku_kari_gseat + 46].f22;
            class_w[115].f67 = td.a[v >> 3][v & 7];
        }
    }

    for (i = 0; i < 3; i++) {
        for (j = 1; j <= i + 1; j++) {
            while (class_w[taiiku_kari_run[i + 1].f02 + 46].f22 ==
                       class_w[taiiku_kari_run[i - j + 1].f02 + 46].f22 ||
                   class_w[taiiku_kari_run[i + 1].f02 + 46].f22 ==
                       class_w[taiiku_kari_gseat + 46].f22) {
                x = (u8)(rand() % 30);
                taiiku_kari_run[i + 1].f02 = x;
            }
        }
    }

    for (i = 0; i < 4; i++) {
        class_w[107 + i].f22 =
            class_w[taiiku_kari_run[i].f02 + 46].f22 >> 3;
    }

    for (i = 0; i < 4; i++) {
        v = class_w[taiiku_kari_run[i].f02 + 46].f22;
        class_w[107 + i].f67 = td.a[v >> 3][v & 7];
    }
}
