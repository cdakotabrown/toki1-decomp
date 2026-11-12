/* Address: 0x80142BCC */
/* Size: 3656 bytes (914 instructions) */
/* @SymbolSize: 3656  (jr table: @RodataBase: 0x80144A98) */
/* @CompileFlags: -Wo,-loopunroll,0 -Wc,-pic0 */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=22:v1 IDO52_CONST_NOINREG_VALUES=19,7 IDO52_LDA_NOINREG_CLASSES=1:14:14,1:15:15 IDO52_AS1_XBB_VOLATILE_OK=all IDO52_COPYPROP_VAR=-28 IDO52_AS1_FORCE_LA_PULL=15 IDO52_UGEN_TEMP_REG=120:t6,121:t7,122:t9,123:t8,124:t7,125:t9,126:t8,127:t6,128:t9 */
/* @PostProc: swap_instructions --offset1 0x294 --offset2 0x29c */
/* @PostProc: swap_instructions --offset1 0x2a0 --offset2 0x2a8 */
/* @PostProc: swap_instructions --offset1 0x2c0 --offset2 0x2c4 */
/* @PostProc: swap_instructions --offset1 0x2c4 --offset2 0x2c8 */
/* @PostProc: swap_instructions --offset1 0x2c8 --offset2 0x2cc */
/* @PostProc: swap_instructions --offset1 0x99c --offset2 0x9a0 */
/* @PostProc: swap_instructions --offset1 0x9cc --offset2 0x9d0 */
/* @PostProc: swap_instructions --offset1 0x9d0 --offset2 0x9d4 */
/* @PostProc: swap_instructions --offset1 0x9d4 --offset2 0x9d8 */
/* @PostProc: swap_instructions --offset1 0x9d8 --offset2 0x9dc */
/* @PostProc: swap_instructions --offset1 0x9dc --offset2 0x9e0 */
/* @PostProc: swap_instructions --offset1 0x9e0 --offset2 0x9e4 */
/* @PostProc: swap_instructions --offset1 0x9e4 --offset2 0x9e8 */
/* @PostProc: swap_instructions --offset1 0x9e8 --offset2 0x9ec */
/* @PostProc: patch_insn_reg_field 0x20:rt:v1:v0 0x70:rs:v1:v0 0x7c:rs:v1:v0 0x88:rs:v1:v0 0x94:rs:v1:v0 0xa0:rs:v1:v0 0xac:rs:v1:v0 0x254:rd:v0:v1 0x25c:rs:v1:v0 0x288:rt:v0:v1 0x290:rt:v0:v1 0x2b4:rt:t6:a3 0x2b8:rs:t6:a3 0x2b8:rt:t6:a3 0x2c0:rd:v1:t6 0x2c0:rs:t8:a3 0x2c0:rt:t6:t8 0x2c4:rs:v1:t6 0x330:rt:t3:t4 0x334:rt:a1:a2 0x338:rt:a2:a3 0x340:rs:t3:t4 0x344:rs:a2:a3 0x344:rt:a2:a3 0x348:rs:a1:a2 0x350:rs:a1:a2 0x358:rs:a1:a2 0x364:rs:a1:a2 0x36c:rs:a1:a2 0x380:rs:a1:a2 0x38c:rs:a1:a2 0x3ac:rs:t3:t4 0x3b4:rs:a1:a2 0x3b8:rs:a1:a2 0x3cc:rs:a1:a2 0x3d4:rs:a1:a2 0x3e8:rs:a1:a2 0x3f0:rs:a1:a2 0x404:rs:a1:a2 0x410:rs:a1:a2 0x430:rs:t3:t4 0x438:rs:a1:a2 0x43c:rs:a1:a2 0x450:rs:a1:a2 0x458:rs:a1:a2 0x46c:rs:a1:a2 0x474:rs:a1:a2 0x488:rs:a1:a2 0x494:rs:a1:a2 0x4b8:rd:t0:t3 0x4d0:rt:a3:t0 0x4d4:rd:t6:t7 0x4d4:rs:v1:a3 0x4d4:rt:a2:v1 0x4d8:rs:t6:t7 0x4d8:rt:v0:t9 0x4dc:rt:t7:t6 0x4e0:rd:t4:a1 0x4e0:rt:t7:t6 0x4e4:rs:v0:t0 0x4e4:rt:a3:t9 0x4e8:rs:t4:a1 0x4f8:rs:t3:t4 0x504:rs:t3:t4 0x508:rd:t9:t8 0x50c:rs:t9:t8 0x50c:rt:t9:t8 0x514:rs:a1:a2 0x514:rt:t9:t8 0x524:rt:t8:t6 0x528:rs:t8:t6 0x528:rt:t8:t6 0x530:rd:t6:t7 0x530:rs:t8:a3 0x530:rt:a2:t6 0x534:rs:t6:t7 0x534:rt:t7:t9 0x53c:rs:t7:t9 0x53c:rt:t9:t8 0x540:rs:t9:t8 0x54c:rt:t8:t6 0x550:rs:t8:t6 0x550:rt:t8:t6 0x558:rd:t6:t7 0x558:rs:t8:a3 0x558:rt:a2:t6 0x55c:rs:t6:t7 0x55c:rt:t7:t9 0x564:rs:t7:t9 0x564:rt:t9:t8 0x568:rs:t9:t8 0x574:rt:t8:t6 0x578:rs:t8:t6 0x578:rt:t8:t6 0x580:rd:t6:t7 0x580:rs:t8:a3 0x580:rt:a2:t6 0x584:rs:t6:t7 0x584:rt:t7:t9 0x58c:rs:t7:t9 0x58c:rt:t9:t8 0x590:rs:t9:t8 0x59c:rt:t8:t6 0x5a0:rs:t8:t6 0x5a0:rt:t8:t6 0x5a8:rd:t6:t7 0x5a8:rt:t8:t6 0x5ac:rd:t6:t7 0x5ac:rs:t6:t7 0x5ac:rt:t8:t6 0x5b0:rd:t6:t7 0x5b0:rt:t6:t7 0x5b4:rd:t7:t9 0x5b4:rs:t6:t7 0x5b8:rd:t9:t8 0x5b8:rt:t7:t9 0x5bc:rs:t9:t8 0x5bc:rt:t9:t8 0x5c4:rs:t9:t8 0x5d0:rt:t8:t6 0x5d4:rs:t8:t6 0x5d4:rt:t8:t6 0x5dc:rd:t6:t7 0x5dc:rt:t8:t6 0x5e0:rd:t6:t7 0x5e0:rs:t6:t7 0x5e0:rt:t8:t6 0x5e4:rd:t6:t7 0x5e4:rt:t6:t7 0x5e8:rd:t7:t9 0x5e8:rs:t6:t7 0x5ec:rd:t9:t8 0x5ec:rt:t7:t9 0x5f0:rs:t9:t8 0x5f0:rt:t9:t8 0x5f8:rs:t9:t8 0x608:rs:t3:t4 0x60c:rs:a1:a2 0x614:rs:a1:a2 0x61c:rt:t8:t6 0x620:rs:t8:t6 0x620:rt:t8:t6 0x628:rd:t6:t7 0x628:rt:t8:t6 0x62c:rd:t6:t7 0x62c:rs:t6:t7 0x62c:rt:t8:t6 0x630:rd:t6:t7 0x630:rt:t6:t7 0x634:rd:t7:t9 0x634:rs:t6:a3 0x634:rt:a2:t7 0x638:rs:t7:t9 0x638:rt:t9:t8 0x640:rs:t9:t8 0x648:rs:t3:t4 0x64c:rs:a1:a2 0x654:rs:a1:a2 0x65c:rt:t8:t6 0x660:rs:t8:t6 0x660:rt:t8:t6 0x668:rd:t6:t7 0x668:rt:t8:t6 0x66c:rd:t6:t7 0x66c:rs:t6:t7 0x66c:rt:t8:t6 0x670:rd:t6:t7 0x670:rt:t6:t7 0x674:rd:t7:t9 0x674:rs:t6:a3 0x674:rt:a2:t7 0x678:rs:t7:t9 0x678:rt:t9:t8 0x680:rs:t9:t8 0x688:rs:t3:t4 0x68c:rs:a1:a2 0x694:rs:a1:a2 0x69c:rt:t8:t6 0x6a0:rs:t8:t6 0x6a0:rt:t8:t6 0x6a8:rd:t6:t7 0x6a8:rt:t8:t6 0x6ac:rd:t6:t7 0x6ac:rs:t6:t7 0x6ac:rt:t8:t6 0x6b0:rd:t6:t7 0x6b0:rt:t6:t7 0x6b4:rd:t7:t9 0x6b4:rs:t6:a3 0x6b4:rt:a2:t7 0x6b8:rs:t7:t9 0x6b8:rt:t9:t8 0x6c0:rs:t9:t8 0x6e8:rs:a1:a2 0x6f0:rs:a1:a2 0x700:rs:t3:t4 0x704:rs:a1:a2 0x720:rt:t8:t6 0x724:rt:t9:t8 0x728:rd:t6:t7 0x728:rt:t8:t6 0x72c:rd:t7:t9 0x72c:rs:t6:a3 0x72c:rt:a2:t7 0x730:rd:t8:t6 0x730:rs:t7:t9 0x730:rt:t9:t8 0x734:rs:t8:t6 0x734:rt:t6:t7 0x73c:rs:t6:t7 0x744:rs:a1:a2 0x75c:rt:t7:t9 0x760:rt:t6:t7 0x764:rd:t9:t8 0x764:rt:t7:t9 0x768:rd:t8:t6 0x768:rs:t9:a3 0x768:rt:a2:t8 0x76c:rd:t7:t9 0x76c:rs:t8:t6 0x76c:rt:t6:t7 0x770:rs:t7:t9 0x770:rt:t9:t8 0x778:rs:t9:t8 0x780:rs:a1:a2 0x790:rs:t6:a3 0x790:rt:a2:t6 0x7c0:rs:t8:t6 0x7c0:rt:t6:t8 0x7cc:rt:t8:t6 0x7d0:rs:t8:t6 0x7d0:rt:t8:t6 0x7d4:rt:t6:t8 0x7d8:rs:a3:t0 0x7d8:rt:t8:t6 0x7e8:rs:a1:a2 0x7e8:rt:a3:t0 0x7f0:rs:a1:a2 0x7f8:rs:a1:a2 0x800:rs:a1:a2 0x820:rs:t6:t8 0x820:rt:t6:t8 0x828:rs:t6:t8 0x868:rt:t8:t6 0x878:rs:t8:t6 0x878:rt:t8:t6 0x880:rs:t8:t6 0x884:rt:t6:t8 0x890:rs:a1:a2 0x890:rt:a3:t0 0x898:rs:a1:a2 0x8a0:rs:a1:a2 0x8a8:rs:a1:a2 0x8b0:rs:t6:t8 0x8b0:rt:t6:t8 0x8b8:rs:t6:t8 0x8c8:rs:a1:a2 0x8c8:rt:a3:t0 0x8d0:rs:a1:a2 0x8d8:rs:a1:a2 0x8e0:rs:a1:a2 0x91c:rd:t8:t6 0x920:rd:t8:t6 0x920:rs:t8:t6 0x924:rd:t8:t6 0x924:rt:t8:t6 0x928:rt:t6:t8 0x92c:rd:t8:t6 0x92c:rs:t8:t6 0x930:rs:a1:a2 0x930:rt:a3:t0 0x934:rs:t8:t6 0x934:rt:t6:t8 0x938:rs:a1:a2 0x940:rs:a1:a2 0x948:rs:a1:a2 0x968:rt:t8:t6 0x96c:rs:t4:a1 0x96c:rt:a3:t0 0x970:rd:t0:t3 0x970:rs:t8:t3 0x970:rt:t0:t6 0x974:rs:t0:t3 0x974:rt:t6:t8 0x97c:rd:t0:t3 0x97c:rs:t6:t8 0x994:rd:t6:t7 0x994:rs:v1:a3 0x994:rt:a2:v1 0x9a0:rt:a1:a2 0x9a8:rt:t0:t3 0x9b4:rt:t0:t3 0x9bc:rt:t0:t3 0x9d4:rs:t0:t3 0x9b8:rt:a1:a2 0x9c0:rt:a3:t0 0x9e0:rd:t0:t3 0x9e4:rt:t3:a3 0x9f0:rd:a2:a1 0x9f4:rs:a2:a1 0x9f4:rt:t8:t6 0x9f8:rd:a2:a1 0x9f8:rs:t8:t6 0x9fc:rd:t6:t8 0xa00:rs:t6:t8 0xa00:rt:t6:t8 0xa04:rs:v1:t4 0xa04:rt:t4:v1 0xa08:rs:t6:t8 0xa18:rd:t8:t6 0xa1c:rd:t8:t6 0xa1c:rs:t8:t6 0xa20:rd:t8:t6 0xa20:rt:t8:t6 0xa24:rt:t6:t8 0xa28:rd:t8:t6 0xa28:rs:t8:t6 0xa2c:rs:a1:a2 0xa2c:rt:a3:t0 0xa30:rs:t8:t6 0xa30:rt:t6:t8 0xa34:rs:a1:a2 0xa3c:rs:a1:a2 0xa44:rs:a1:a2 0xa64:rt:t8:t6 0xa6c:rd:t0:t3 0xa6c:rs:t8:t3 0xa6c:rt:t0:t6 0xa70:rs:t0:t3 0xa70:rt:t6:t8 0xa74:rs:t6:t8 0xa74:rt:a2:a1 0xa7c:rd:t0:t3 0xa7c:rs:t6:t8 0xa84:rt:t3:a3 0xa9c:rd:t6:t8 0xaa0:rt:t8:t6 0xaa4:rs:t3:a3 0xaa4:rt:t8:t6 0xaa8:rt:t6:t8 0xab0:rs:a1:a2 0xab0:rt:a3:t0 0xab4:rt:t6:t8 0xab8:rs:a1:a2 0xabc:rt:t9:t7 0xac0:rs:a1:a2 0xac4:rt:t9:t7 0xac8:rs:a1:a2 0xacc:rt:t9:t7 0xad0:rt:t9:t7 0xad4:rt:t8:t9 0xad8:rs:t8:t9 0xad8:rt:t8:t9 0xae0:rs:t8:t9 0xb04:rt:t7:t8 0xb0c:rt:t7:t8 0xb10:rt:t9:t7 0xb18:rt:t9:t7 0xb6c:rt:t8:t9 0xb70:rs:t8:t9 0xb70:rt:t8:t9 0xb78:rs:t8:t9 0xb80:rd:t8:t9 0xb80:rt:t8:t9 0xb88:rt:t8:t9 0xb8c:rt:t8:t9 0xb94:rs:t8:t9 0xba8:rt:t7:t8 0xbac:rs:t7:t8 0xbac:rt:t7:t8 0xbb8:rt:t9:t7 0xbbc:rs:t9:t7 0xbbc:rt:t9:t7 0xbc0:rt:t7:t8 0xbcc:rt:t9:t7 0xbd0:rt:t8:t9 0xbd4:rs:t8:t9 0xbd4:rt:t8:t9 0xbe4:rt:t8:t9 0xbec:rt:t7:t8 0xbf0:rs:t7:t8 0xbf0:rt:t7:t8 0xc00:rt:t7:t8 0xc04:rt:t9:t7 0xc08:rs:t9:t7 0xc08:rt:t9:t7 0xc10:rt:t8:t9 0xc14:rs:t8:t9 0xc14:rt:t8:t9 0xc18:rt:t9:t7 0xc28:rt:t8:t9 0xc38:rt:t7:t8 0xc3c:rs:t7:t8 0xc3c:rt:t7:t8 0xc44:rt:t9:t7 0xc48:rs:t9:t7 0xc48:rt:t9:t7 0xc4c:rt:t7:t8 0xc54:rt:t8:t9 0xc58:rs:t8:t9 0xc58:rt:t8:t9 0xc5c:rt:t9:t7 0xc68:rt:t8:t9 0xc78:rt:t7:t8 0xc7c:rs:t7:t8 0xc7c:rt:t7:t8 0xc88:rt:t9:t7 0xc8c:rs:t9:t7 0xc8c:rt:t9:t7 0xc90:rt:t7:t8 0xc9c:rt:t9:t7 0xca0:rt:t8:t9 0xca4:rs:t8:t9 0xca4:rt:t8:t9 0xcb4:rt:t8:t9 0xcbc:rt:t7:t8 0xcc0:rs:t7:t8 0xcc0:rt:t7:t8 0xcd0:rt:t7:t8 0xcd4:rt:t9:t7 0xcd8:rs:t9:t7 0xcd8:rt:t9:t7 0xce0:rt:t8:t9 0xce4:rs:t8:t9 0xce4:rt:t8:t9 0xce8:rt:t9:t7 0xcfc:rt:t8:t9 0xd28:rt:t8:t6 0xd2c:rs:t8:t6 0xd2c:rt:t8:t6 0xd38:rt:t6:t8 0xd3c:rs:t6:t8 0xd3c:rt:t6:t8 0xd40:rt:t8:t6 0xd4c:rt:t6:t8 0xd6c:rt:t8:t6 0xd70:rs:t8:t6 0xd70:rt:t8:t6 0xd80:rt:t8:t6 0xd84:rt:t6:t8 0xd88:rs:t6:t8 0xd88:rt:t6:t8 0xd98:rt:t6:t8 0xdb8:rt:t8:t6 0xdbc:rs:t8:t6 0xdbc:rt:t8:t6 0xdc4:rt:t6:t8 0xdc8:rs:t6:t8 0xdc8:rt:t6:t8 0xdcc:rt:t8:t6 0xddc:rt:t6:t8 0xdf8:rt:t8:t6 0xdfc:rs:t8:t6 0xdfc:rt:t8:t6 0xe08:rt:t6:t8 0xe0c:rs:t6:t8 0xe0c:rt:t6:t8 0xe10:rt:t8:t6 0xe18:rt:t6:t8 0xe2c:rt:t8:t6 0xe34:rt:t8:t6 */
#include "types.h"

extern s32 prim_init(void);
extern s32 sub_sub_step_up(void);
extern s32 draw2d3d(s32, s32);
extern s32 back_clear_switch(s32);
extern s32 tpage_buf_clear();
extern s32 class_switch(s32);
extern s32 class_clear_all(s32);
extern s32 text_sys_init(void);
extern s32 k_reset(s32);
extern s32 get_g_zyotai_s(s32);
extern s32 hizuke_init(void);
extern s32 message_window_init(void);
extern s32 bustup_ptr_init(void);
extern s32 date_name_set(s32);
extern s32 yazirushi_off(void);
extern s32 shared_system_state_reset(void);
extern s32 event_log_add(s32, s32);
extern s32 rand(void);
extern void srand(s32);
extern s32 strcpy(s32, s32);
extern s32 geko_girl_ptr_init(void);
extern s32 geko_week_end_get(s32);
extern void geko_event_set(void);

extern u8 bg_change_tbl2;
extern u8 ba_string[];
extern u8 mname[];
extern u8 screen_w[];
extern u8 screen_w_b[];
extern u8 screen_bri;
extern u8 now_year;
extern u8 now_month;
extern u8 now_day;
extern u8 now_youbi;
extern s32 reserved_date;
extern u8 schedule_cursor;
extern volatile u8 now_girl;
extern u8 vblank_flag;
extern s32 vb_ready;
extern s32 frame_counter;
extern u8 fade_bri;
extern u8 geko_girl_pick_init_str0[];
extern s32 geko_serifu3_tbl[];
extern s32 geko_serifu3_kind_tbl[];
extern s32 geko_sound3_tbl[];
extern s32 geko_ss_serifu3;
extern s32 geko_ss_serifu3_kind;
extern s32 geko_sss_sound3;
extern s16 geko_now_serifu3;
extern s16 geko_now_serifu3_in;
extern u8 geko_pick_no;

#define EE760 ((s32)screen_w_b)

typedef struct { char b[209]; } T209;
typedef struct { char b[24]; } T24;
typedef struct { char b[132]; } T132;
typedef struct { char b[21]; } T21;
typedef struct { char b[627]; } T627;
typedef struct { char b[37]; } T37;

extern T209 geko_girl_pick_init_tbl0;
extern T24 geko_girl_pick_init_tbl1;
extern T132 geko_girl_pick_init_tbl2;
extern T132 geko_girl_pick_init_tbl3;
extern T21 geko_girl_pick_init_tbl4;
extern T21 geko_girl_pick_init_tbl5;
extern T627 geko_girl_pick_init_tbl6;
extern T37 geko_girl_pick_init_tbl7;
extern T21 geko_girl_pick_init_tbl8;

void geko_girl_pick_init(void)
{

    s32 flag;
    volatile u8 s1363;
    volatile u8 s1362;
    volatile u8 s1361;
    volatile u8 s1360;
    u8 *p1;
    s32 t4;
    s32 a2;
    s32 ladr;
    u32 bval;
    u32 wval;
    u8 *pg;
    s32 ee;
    u8 *mark;
    s16 v1;
    volatile u8 s1321;
    u8 t9;
    volatile s16 lad1318;
    u8 t3;
    T209 c1108;
    u8 marks[21];
    u32 elig;
    T24 c1056;
    T132 c924;
    T132 c792;
    T21 c768;
    T21 c744;
    T627 c116;
    T37 c76;
    T21 c52;
    u32 lim;
    u32 win;
    u8 *p52;
    u8 *pw;

    c1108 = geko_girl_pick_init_tbl0;
    c1056 = geko_girl_pick_init_tbl1;
    c924 = geko_girl_pick_init_tbl2;
    c792 = geko_girl_pick_init_tbl3;
    c768 = geko_girl_pick_init_tbl4;
    c744 = geko_girl_pick_init_tbl5;
    c116 = geko_girl_pick_init_tbl6;
    c76 = geko_girl_pick_init_tbl7;
    c52 = geko_girl_pick_init_tbl8;

    if (now_year == 96) {
        *(s16 *)(c1056.b + 2) = 29;
    }

    {
        flag = 0;
        if ((u32)*(s16 *)(c1056.b + (s32)now_month * 2 - 2) < (u32)((s32)now_day - (s32)now_youbi + 6)) {
            flag = 1;
        }
        s1362 = (u8)flag;
        s1360 = (u8)geko_week_end_get(0);
        s1361 = (u8)geko_week_end_get(1);
        {
            flag = geko_week_end_get(2);
            t9 = schedule_cursor;
            s1363 = (u8)flag;
            {
                p1 = screen_w + (s32)t9 * 4;
                if (p1) {
                }
                now_girl = p1[1693];
            }
            draw2d3d(1, 0);
        }
    }

    class_switch(1);
    prim_init();
    class_clear_all(0);
    back_clear_switch(1);
    tpage_buf_clear();
    text_sys_init();
    strcpy((s32)ba_string, (s32)geko_girl_pick_init_str0);
    event_log_add(9, 0);

    {
        t4 = s1360;
        a2 = s1361;

        ee = EE760;
        if (ee) {
        }
        if (t4 == 95) {
            if (!((u32)a2 < 4U) && (u32)a2 < 6U) {
                lad1318 = 0;
            } else if (!((u32)a2 < 6U) && (u32)a2 < 9U) {
                lad1318 = 1;
            } else if (!((u32)a2 < 9U) && (u32)a2 < 12U) {
                lad1318 = 2;
            } else {
                lad1318 = 3;
            }
        } else if (t4 == 96) {
            if (a2 != 0 && (u32)a2 < 3U) {
                lad1318 = 3;
            } else if (!((u32)a2 < 3U) && (u32)a2 < 6U) {
                lad1318 = 4;
            } else if (!((u32)a2 < 6U) && (u32)a2 < 9U) {
                lad1318 = 5;
            } else if (!((u32)a2 < 9U) && (u32)a2 < 12U) {
                lad1318 = 6;
            } else {
                lad1318 = 7;
            }
        } else if (t4 == 97) {
            if (a2 != 0 && (u32)a2 < 3U) {
                lad1318 = 7;
            } else if (!((u32)a2 < 3U) && (u32)a2 < 6U) {
                lad1318 = 8;
            } else if (!((u32)a2 < 6U) && (u32)a2 < 9U) {
                lad1318 = 9;
            } else if (!((u32)a2 < 9U) && (u32)a2 < 12U) {
                lad1318 = 10;
            } else {
                lad1318 = 11;
            }
        } else {
            lad1318 = 11;
        }

        ladr = lad1318;
        t3 = 0;
        v1 = 0;
        do {
            mark = &marks[v1];
            elig = *(u8 *)(ee + v1 + 288);
            *mark = 0;
            if (elig != 1U) goto L_next;

            bval = *(u8 *)(c768.b + v1);
            if ((u32)t4 < bval) goto L_next;
            if (t4 == bval) {
                if ((u32)a2 < (u32)*(u8 *)(c744.b + v1)) goto L_next;
            }

            if (v1 == 14) {
                if (*(u8 *)(ee + now_girl + 1868) & 1) goto L_next;
            }
            if (v1 == 15) {
                if (*(u8 *)(ee + now_girl + 1868) & 2) goto L_next;
            }
            if (v1 == 7) {
                if (*(u8 *)(ee + now_girl + 1868) & 4) goto L_next;
            }

            if (v1 == 14) {
                if (*(u8 *)(c924.b + (s32)now_girl * 12 + ladr) == 0) goto L_next;
            }
            if (v1 == 15) {
                if (*(u8 *)(c792.b + (s32)now_girl * 12 + ladr) == 0) goto L_next;
            }
            if (v1 == 7) {
                if (t4 == 97 && !((u32)a2 < 9U) && (u32)a2 < 12U) {
                    if (*(u8 *)(ee + (s32)now_girl * 56 + 476) != 0) goto L_next;
                }
                if (t4 == 97 && !((u32)a2 < 3U) && (u32)a2 < 6U) {
                    if (*(u8 *)(ee + (s32)now_girl * 56 + 477) != 0) goto L_next;
                }
                if (t4 == 95 && !((u32)a2 < 9U) && (u32)a2 < 12U) {
                    if (*(u8 *)(ee + (s32)now_girl * 56 + 478) != 0) goto L_next;
                }
            }

            if (v1 == 0) goto L_next;
            if (v1 == 1) goto L_next;
            if (v1 == 18) goto L_next;
            if (v1 == 13) {
                if (a2 == 2) goto L_next;
                if (a2 == 12) goto L_next;
            }
            if (v1 == 14 && t4 == 95) {
                if ((u32)a2 < 6U) goto L_next;
            }

            if (v1 == 16 || v1 == 17) {
                if (*(u8 *)(ee + ((s32)s1362 << 6) + s1363 + 68) != 2) {
                    if (a2 != 7) goto L_next;
                }
            }
            if (v1 == 19 || v1 == 20) {
                if (*(u8 *)(ee + ((s32)s1362 << 6) + s1363 + 68) != 2) {
                    if (a2 != 12) goto L_next;
                }
            }

            if (v1 == *(u8 *)(c76.b + *(u8 *)(ee + now_girl + 1836))) goto L_next;

            bval = v1;
            {
                elig = reserved_date;
                if (now_girl == (elig << 3) >> 28) {
                    if (bval == (elig & 0x1F)) goto L_next;
                }
            }

            if (1U == now_girl) {
                if (v1 == 13) {
                    if (a2 == 1U) goto L_next;
                    if (a2 == 3U) goto L_next;
                    if (a2 == 6U) goto L_next;
                    if (a2 == 11U) goto L_next;
                }
                if (v1 == 16) goto L_next;
                if (v1 == 17) goto L_next;
                if (v1 == 20) goto L_next;
            }
            if (now_girl == 2) {
                if (v1 == 20) goto L_next;
            }
            if (3U == now_girl) {
                if (v1 == 16) goto L_next;
                if (v1 == 17) goto L_next;
            }
            if (now_girl == 4) {
                if (v1 == 7) goto L_next;
            }
            if (now_girl == 5) {
                if (v1 == 13) {
                    if (a2 == 1U) goto L_next;
                    if (a2 == 3U) goto L_next;
                    if (a2 == 6U) goto L_next;
                    if (a2 == 11U) goto L_next;
                }
            }
            if (now_girl == 9) {
                if (v1 == 13) {
                    if (a2 == 1U) goto L_next;
                    if (a2 == 3U) goto L_next;
                    if (a2 == 6U) goto L_next;
                    if (a2 == 11U) goto L_next;
                }
                if (v1 == 20) goto L_next;
            }
            if (now_girl == 10) {
                if (v1 == 7) goto L_next;
                if (v1 == 11) goto L_next;
            }

            wval = *(u8 *)(c52.b + v1);
            pg = (u8 *)(c1108.b + (s32)now_girl * 19);
            if (a2 == 1U || a2 == 3U || a2 == 6U || a2 == 11U) {
                if (v1 == 13) {
                    wval = (u8)(wval + 1);
                }
            }
            t3 = t3 + *(pg + wval);
            *mark = 1;
            if (t3) {
            }
            ladr = lad1318;

        L_next:
            v1 = (s16)(v1 + 1);
        } while (v1 < 21);

        s1361 = (u8)a2;
        s1321 = (u8)t3;
        srand(frame_counter);
        {
            lim = (u32)rand();
            t3 = s1321;
            a2 = s1361;
            lim = (u8)(lim % (u32)t3);
            t3 = 0;
            win = 2;
            v1 = 0;
            p52 = (u8 *)c52.b;
            if (p52) {
            }
            do {
                if (marks[v1] == 0) goto L_next2;
                wval = *(p52 + v1);
                pg = (u8 *)(c1108.b + (s32)now_girl * 19);
                if (a2 == 1U || a2 == 3U || a2 == 6U || a2 == 11U) {
                    if (v1 == 13) {
                        wval = (u8)(wval + 1);
                    }
                }
                t3 = t3 + *(pg + wval);
                if ((u32)t3 < lim) goto L_next2;
                win = (u8)v1;
                goto L_found;
            L_next2:
                v1 = (s16)(v1 + 1);
            } while (v1 < 21);

        L_found:
            {
                pw = (u8 *)c52.b + win;
                geko_pick_no = *pw;
                if (a2 == 1U || a2 == 3U || a2 == 6U || a2 == 11U) {
                    if (pw == (u8 *)c52.b + 13) {
                        geko_pick_no = (u8)(geko_pick_no + 1);
                    }
                }
            }
        }
    }

    geko_event_set();
    k_reset(1);
    vblank_flag = 0;
    vb_ready = 1;
    shared_system_state_reset();
    screen_bri = 128;
    fade_bri = 0;
    bg_change_tbl2 = 0;
    bustup_ptr_init();
    hizuke_init();
    message_window_init();
    yazirushi_off();
    geko_now_serifu3 = 0;
    geko_now_serifu3_in = 0;
    date_name_set(now_girl);
    geko_girl_ptr_init();

    switch (now_girl) {
    case 0:
        geko_ss_serifu3 = geko_serifu3_tbl[1];
        geko_ss_serifu3_kind = geko_serifu3_kind_tbl[1];
        geko_sss_sound3 = geko_sound3_tbl[1];
        break;
    case 1:
        geko_ss_serifu3 = geko_serifu3_tbl[5];
        geko_ss_serifu3_kind = geko_serifu3_kind_tbl[5];
        geko_sss_sound3 = geko_sound3_tbl[5];
        break;
    case 2:
        geko_ss_serifu3 = geko_serifu3_tbl[2];
        geko_ss_serifu3_kind = geko_serifu3_kind_tbl[2];
        geko_sss_sound3 = geko_sound3_tbl[2];
        break;
    case 3:
        geko_ss_serifu3 = geko_serifu3_tbl[3];
        geko_ss_serifu3_kind = geko_serifu3_kind_tbl[3];
        geko_sss_sound3 = geko_sound3_tbl[3];
        break;
    case 4:
        geko_ss_serifu3 = geko_serifu3_tbl[10];
        geko_ss_serifu3_kind = geko_serifu3_kind_tbl[10];
        geko_sss_sound3 = geko_sound3_tbl[10];
        break;
    case 5:
        geko_ss_serifu3 = geko_serifu3_tbl[6];
        geko_ss_serifu3_kind = geko_serifu3_kind_tbl[6];
        geko_sss_sound3 = geko_sound3_tbl[6];
        break;
    case 6:
        geko_ss_serifu3 = geko_serifu3_tbl[8];
        geko_ss_serifu3_kind = geko_serifu3_kind_tbl[8];
        geko_sss_sound3 = geko_sound3_tbl[8];
        if ((u32)(get_g_zyotai_s(now_girl) & 0x7F) < 2U) {
            geko_ss_serifu3 = geko_serifu3_tbl[7];
            geko_ss_serifu3_kind = geko_serifu3_kind_tbl[7];
            geko_sss_sound3 = geko_sound3_tbl[7];
        }
        break;
    case 7:
        geko_ss_serifu3 = geko_serifu3_tbl[4];
        geko_ss_serifu3_kind = geko_serifu3_kind_tbl[4];
        geko_sss_sound3 = geko_sound3_tbl[4];
        break;
    case 8:
        geko_ss_serifu3 = geko_serifu3_tbl[0];
        geko_ss_serifu3_kind = geko_serifu3_kind_tbl[0];
        geko_sss_sound3 = geko_sound3_tbl[0];
        break;
    case 9:
        geko_ss_serifu3 = geko_serifu3_tbl[9];
        geko_ss_serifu3_kind = geko_serifu3_kind_tbl[9];
        geko_sss_sound3 = geko_sound3_tbl[9];
        break;
    case 10:
        geko_ss_serifu3 = geko_serifu3_tbl[12];
        geko_ss_serifu3_kind = geko_serifu3_kind_tbl[12];
        geko_sss_sound3 = geko_sound3_tbl[12];
        break;
    }

    strcpy((s32)mname, (s32)(c116.b + (s32)geko_pick_no * 33));
    sub_sub_step_up();
}
