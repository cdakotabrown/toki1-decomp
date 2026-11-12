/* @PostProc: patch_insn_reg_field 0x4c4:rt:t8:t7 0x4d4:rt:t6:t8 0x4dc:rs:t6:t8 0x4dc:rt:t7:t9 0x4e0:rd:t9:t0 0x4e0:rt:t8:t9 0x4e8:rt:t9:t0 0x4ec:rt:t0:t1 0x4f0:rs:t0:t1 0x4f0:rt:t0:t1 0x4f8:rs:t0:t1 0x500:rd:t1:t2 0x504:rd:t2:t3 0x504:rt:t1:t2 0x508:rs:t2:t3 0x510:rd:t3:t4 0x518:rt:t3:t4 0x51c:rt:t5:t6 0x520:rs:t5:t6 0x520:rt:t5:t6 0x528:rs:t5:t6 0x530:rd:t6:t8 0x534:rt:t6:t8 0x538:rt:t7:t1 0x540:rs:t7:t1 0x540:rt:t8:t2 0x544:rd:t1:t3 0x544:rs:t8:t0 0x544:rt:t0:t2 0x54c:rt:t1:t3 0x550:rt:t2:t4 0x554:rs:t2:t4 0x554:rt:t2:t4 0x55c:rs:t2:t4 0x564:rd:t3:t5 0x568:rd:t4:t6 0x568:rt:t3:t5 0x56c:rs:t4:t6 0x574:rd:t5:t8 0x57c:rt:t5:t8 0x580:rt:t7:t9 0x584:rs:t7:t9 0x584:rt:t7:t9 0x588:rd:t1:t2 0x58c:rs:t7:t9 0x590:rs:t1:t2 0x590:rt:t2:t3 0x594:rd:t9:t1 0x598:rt:t9:t1 0x59c:rt:t8:t4 0x5a4:rs:t8:t4 0x5a4:rt:t0:t5 0x5a8:rd:t3:t6 0x5a8:rs:t0:t3 0x5a8:rt:t2:t5 0x5b0:rt:t3:t6 0x5b4:rt:t4:t8 0x5b8:rs:t4:t8 0x5b8:rt:t4:t8 0x5c0:rs:t4:t8 0x5c8:rd:t5:t7 0x5cc:rd:t6:t9 0x5cc:rt:t5:t7 0x5d0:rs:t6:t9 0x5d8:rd:t7:t1 0x5e0:rt:t7:t1 0x5e4:rt:t8:t2 0x5e8:rs:t8:t2 0x5e8:rt:t8:t2 0x5ec:rd:t3:t5 0x5f0:rs:t8:t2 0x5f4:rs:t3:t5 0x5f4:rt:t4:t6 0x5f8:rd:t1:t4 0x5fc:rt:t1:t4 0x600:rt:t0:t8 0x608:rs:t0:t8 0x608:rt:t2:t7 0x60c:rd:t5:t9 0x60c:rs:t2:t6 0x60c:rt:t4:t7 0x614:rt:t5:t9 0x618:rt:t6:t1 0x61c:rs:t6:t1 0x61c:rt:t6:t1 0x624:rs:t6:t1 0x62c:rd:t7:t0 0x630:rd:t9:t2 0x630:rt:t7:t0 0x634:rs:t9:t2 0x640:rt:t8:t4 0x644:rs:t8:t4 0x644:rt:t8:t4 0x64c:rs:t8:t4 0x650:rd:t1:t3 0x654:rd:t0:t5 0x654:rt:t1:t3 0x65c:rs:t0:t5 0x660:rt:t3:t8 0x664:rs:t3:t8 0x664:rt:t3:t8 0x66c:rs:t3:t8 0x674:rd:t2:t6 0x678:rd:t4:t7 0x678:rt:t2:t6 0x67c:rs:t4:t7 0x684:rd:t5:t9 0x68c:rt:t5:t9 0x690:rt:t7:t0 0x694:rs:t7:t0 0x694:rt:t7:t0 0x698:rd:t0:t3 0x69c:rs:t7:t0 0x6a0:rs:t0:t3 0x6a0:rt:t3:t5 0x6a4:rd:t9:t2 0x6a8:rt:t9:t2 0x6b4:rt:t1:t6 0x6b8:rd:t2:t7 0x6b8:rs:t1:t5 0x6b8:rt:t3:t6 0x6c0:rt:t2:t7 0x6c4:rt:t4:t9 0x6c8:rs:t4:t9 0x6c8:rt:t4:t9 0x6d0:rs:t4:t9 0x6d8:rd:t5:t1 0x6dc:rd:t6:t0 0x6dc:rt:t5:t1 0x6e0:rs:t6:t0 0x6e8:rd:t7:t2 0x6f0:rs:t7:t2 0x6f4:rt:t9:t4 0x6f8:rs:t9:t4 0x6f8:rt:t9:t4 0x6fc:rd:t2:t6 0x700:rs:t9:t4 0x704:rd:t8:t3 0x708:rt:t8:t3 0x70c:rt:t0:t7 0x714:rs:t0:t7 0x714:rt:t1:t9 0x718:rd:t4:t1 0x718:rs:t1:t6 0x718:rt:t2:t9 0x720:rt:t4:t1 0x724:rt:t5:t0 0x728:rs:t5:t0 0x728:rt:t5:t0 0x730:rs:t5:t0 0x738:rd:t6:t2 0x73c:rd:t7:t4 0x73c:rt:t6:t2 0x740:rs:t7:t4 0x748:rt:t9:t3 0x750:rs:t9:t3 0x75c:rt:t2:t6 0x764:rd:t0:t5 0x768:rt:t0:t5 0x76c:rt:t3:t9 0x774:rs:t3:t9 0x778:rd:t4:t0 0x778:rs:t1:t6 0x778:rt:t2:t1 0x780:rt:t4:t0 0x784:rt:t5:t2 0x788:rs:t5:t2 0x788:rt:t5:t2 0x790:rs:t5:t2 0x798:rd:t6:t4 0x79c:rd:t7:t3 0x79c:rt:t6:t4 0x7a0:rs:t7:t3 0x7a8:rd:t9:t8 0x7b0:rt:t9:t8 0x7b4:rt:t0:t7 0x7b8:rs:t0:t7 0x7b8:rt:t0:t7 0x7bc:rd:t4:t1 0x7c0:rs:t0:t7 0x7c4:rs:t4:t1 0x7c4:rt:t5:t0 0x7c8:rd:t3:t9 0x7cc:rt:t3:t9 0x7d0:rt:t1:t2 0x7d8:rs:t1:t2 0x7d8:rt:t2:t4 0x7dc:rd:t6:t3 0x7dc:rs:t2:t0 0x7dc:rt:t5:t4 0x7e4:rt:t6:t3 0x7e8:rt:t7:t8 0x7ec:rs:t7:t8 0x7ec:rt:t7:t8 0x7f4:rs:t7:t8 0x7fc:rd:t9:t5 0x800:rd:t8:t7 0x800:rt:t9:t5 0x804:rs:t8:t7 0x80c:rd:t0:t9 0x814:rs:t0:t9 0x818:rt:t3:t6 0x81c:rs:t3:t6 0x81c:rt:t3:t6 0x820:rd:t6:t4 0x824:rs:t3:t6 0x830:rt:t4:t3 0x838:rs:t4:t3 0x838:rt:t2:t8 0x83c:rd:t7:t5 0x83c:rs:t2:t4 0x83c:rt:t6:t8 0x844:rt:t7:t5 0x848:rt:t9:t7 0x84c:rs:t9:t7 0x84c:rt:t9:t7 0x854:rs:t9:t7 0x85c:rd:t8:t9 0x860:rd:t0:t6 0x860:rt:t8:t9 0x864:rs:t0:t6 0x86c:rd:t3:t1 0x874:rt:t3:t1 0x878:rt:t4:t0 0x87c:rs:t4:t0 0x87c:rt:t4:t0 0x880:rd:t7:t8 0x884:rs:t4:t0 0x888:rs:t7:t8 0x888:rt:t9:t5 0x88c:rd:t5:t3 0x890:rt:t5:t3 0x894:rt:t2:t7 0x89c:rs:t2:t7 0x89c:rt:t6:t9 0x8a0:rd:t8:t6 0x8a0:rs:t6:t5 0x8a8:rt:t8:t6 0x8ac:rt:t0:t1 0x8b0:rs:t0:t1 0x8b0:rt:t0:t1 0x8b8:rs:t0:t1 0x8c0:rd:t3:t2 0x8c4:rd:t1:t0 0x8c4:rt:t3:t2 0x8c8:rs:t1:t0 0x8d0:rd:t4:t3 0x8d8:rt:t4:t3 0x8dc:rt:t2:t8 0x8e0:rs:t2:t8 0x8e0:rt:t2:t8 0x8e4:rd:t8:t9 0x8e8:rs:t2:t8 0x8ec:rs:t8:t9 0x8ec:rt:t0:t6 0x8f8:rt:t6:t1 0x900:rs:t6:t1 0x900:rt:t9:t2 0x904:rd:t3:t0 0x904:rs:t9:t6 0x904:rt:t0:t2 0x908:rt:t3:t0 */
/* @SymbolSize: 2320 */
/* @RodataBase: 0x8013D154 */
/* Address: 0x8013B588, Size: 2320 bytes (580 instructions) */
#include "types.h"

extern void menu_check();
extern void menu_cursor_move();
extern void menu_bar_show();
extern void debug_string_show();
extern void debug_number_show();
extern s32  option_value_step();
extern void goto_sub_sub_step();

extern s16 cursor_cw_x;
extern s16 cursor_cw_y;
extern s32 option_menu3_lbl_tbl[];
extern volatile u8 option_menu3_girl_no;
extern u8  screen_w[][56];
extern u32 pad_data;
extern u8  now_icon;

#define REC   screen_w[option_menu3_girl_no]
#define WORD  (*(u32 *)&REC[456])

void option_menu3_main1(void)
{
    s32 pos;
    s32 *p;
    s32 r;
    u8 *q;

    menu_check(0, cursor_cw_x, cursor_cw_y);
    menu_cursor_move(0);
    menu_bar_show(0);

    p = option_menu3_lbl_tbl;
    for (pos = -96; pos != 96; pos += 12) {
        debug_string_show(*p, -32, pos);
        p++;
    }

    debug_number_show(option_menu3_girl_no, 32, -96);
    debug_number_show(*(s16 *)&REC[446], 32, -84);
    debug_number_show(*(s16 *)&REC[450], 32, -72);
    debug_number_show(*(s16 *)&REC[454], 32, -60);
    debug_number_show(WORD & 1, 32, -48);
    debug_number_show((WORD << 30) >> 31, 32, -36);
    debug_number_show((WORD << 29) >> 31, 32, -24);
    debug_number_show((WORD << 28) >> 31, 32, -12);
    debug_number_show(REC[459], 32, 0);
    debug_number_show((WORD << 25) >> 31, 32, 12);
    debug_number_show((u32)REC[456] >> 7, 32, 24);
    debug_number_show(REC[457] & 1, 32, 36);
    debug_number_show((WORD << 20) >> 29, 32, 48);
    debug_number_show((u32)REC[458] >> 4, 32, 60);
    debug_number_show((WORD << 19) >> 31, 32, 72);
    debug_number_show((WORD << 18) >> 31, 32, 84);

    if ((pad_data & 0x90) != 0) {
        switch ((u32)now_icon) {
        case 0:
            option_menu3_girl_no = option_value_step(option_menu3_girl_no, 11, 0);
            goto_sub_sub_step(0);
            break;
        case 1:
            *(s16 *)&REC[446] = option_value_step(*(s16 *)&REC[446], 127, 0);
            break;
        case 2:
            *(s16 *)&REC[450] = option_value_step(*(s16 *)&REC[450], 127, 0);
            break;
        case 3:
            *(s16 *)&REC[454] = option_value_step(*(s16 *)&REC[454], 127, 0);
            break;
        case 4:
            r = option_value_step(WORD & 1, 1, 0);
            q = REC;
            q[456] = (q[456] & ~1) | (r & 1);
            break;
        case 5:
            r = option_value_step((WORD << 30) >> 31, 1, 0);
            q = REC;
            q[456] = (q[456] & ~2) | ((r << 1) & 2);
            break;
        case 6:
            r = option_value_step((WORD << 29) >> 31, 1, 0);
            q = REC;
            q[456] = (q[456] & ~4) | ((r << 2) & 4);
            break;
        case 7:
            r = option_value_step((WORD << 28) >> 31, 1, 0);
            q = REC;
            q[456] = (q[456] & ~8) | ((r << 3) & 8);
            break;
        case 8:
            REC[459] = option_value_step(REC[459], 999, 0);
            break;
        case 9:
            r = option_value_step((WORD << 25) >> 31, 1, 0);
            q = REC;
            q[456] = (q[456] & ~0x40) | ((r << 6) & 0x40);
            break;
        case 10:
            r = option_value_step((u32)REC[456] >> 7, 1, 0);
            q = REC;
            q[456] = (q[456] & ~0x80) | ((r << 7) & 0x80);
            break;
        case 11:
            r = option_value_step(REC[457] & 1, 1, 0);
            q = REC;
            q[457] = (q[457] & ~1) | (r & 1);
            break;
        case 12:
            r = option_value_step((WORD << 20) >> 29, 7, 0);
            q = REC;
            q[457] = (q[457] & ~0xE) | ((r << 1) & 0xE);
            break;
        case 13:
            r = option_value_step((u32)REC[458] >> 4, 11, 0);
            q = REC;
            q[458] = (q[458] & ~0xF0) | ((r << 4) & 0xF0);
            break;
        case 14:
            r = option_value_step((WORD << 19) >> 31, 1, 0);
            q = REC;
            q[457] = (q[457] & ~0x10) | ((r << 4) & 0x10);
            break;
        case 15:
            r = option_value_step((WORD << 18) >> 31, 1, 0);
            q = REC;
            q[457] = (q[457] & ~0x20) | ((r << 5) & 0x20);
            break;
        }
    }
}
