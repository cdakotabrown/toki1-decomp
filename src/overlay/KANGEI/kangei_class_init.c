/* @CompileFlags: -Wc,-pic0 */
/* @SymbolSize: 1256 */
/* Address: 0x801376A4 */
/* Size: 1256 bytes (314 insns)   BYTE-PERFECT, zero PostProc */
#include "types.h"

typedef struct {
    u8 b[220];
} BlkA;

typedef struct {
    u8 b[231];
} BlkB;

typedef struct {
    u8 b0;
    u8 b1;
    u8 b2;
    u8 b3;
    u8 b4;
    u8 b5;
    u8 b6;
    u8 b7;
    s32 w8;
    s32 w12;
    s32 w16;
    s16 h20;
    s16 h22;
    s16 h24;
    s16 h26;
    s16 h28;
    s16 h30;
    s16 h32;
    s16 h34;
    s16 h36;
    s16 h38;
    s16 h40;
    s16 h42;
    s16 h44;
    s16 h46;
    s32 w48;
    s32 w52;
    s32 w56;
    s32 w60;
    u8 b64;
    u8 b65;
    u8 b66;
    u8 b67;
} Entry;

typedef struct {
    s16 x;
    s16 y;
} Pt;

extern BlkA kangei_class_init_tbl0;
extern BlkB kangei_class_init_tbl1;

extern u8 aname[];
extern u8 mname[];

extern u16 basyo_pack;
extern u8 byouki_state;
extern s32 frame_counter;

extern s16 kangei_now_s;
extern s16 kangei_now_s_in;
extern s32 kangei_ss_s;
extern s32 kangei_ss_s_kind;
extern s32 kangei_sss_so;
extern s32 kangei_work_pal_ptr;

extern s32 kangei_work_desc_tbl[][2];

extern Pt kangei_streak_pos_wk[];
extern s16 kangei_streak_spd_wk[];

extern Entry class_w[];
extern s32 kangei_msg_res_tbl[][8];

extern void palette_upload_enqueue(s32, s32, s32, s32, s32);
extern void strcpy(void *dst, void *src);
extern void class_clear_n(s32);
extern void srand(s32);
extern s32 rand(void);
extern void sub_sub_step_up(void);

void kangei_class_init(void)
{
    s32 pT[2];
    BlkA bufA;
    BlkB bufB;
    s32 pB[2];
    s16 i;

    bufA = kangei_class_init_tbl0;
    bufB = kangei_class_init_tbl1;

    byouki_state = byouki_state & 0xFFEF;
    kangei_now_s = 13;
    kangei_now_s_in = 0;
    kangei_ss_s = kangei_msg_res_tbl[0][6];
    kangei_ss_s_kind = kangei_msg_res_tbl[1][6];
    kangei_sss_so = kangei_msg_res_tbl[2][6];

    palette_upload_enqueue(kangei_work_pal_ptr, 16, 1, 0, 0);
    palette_upload_enqueue(kangei_work_pal_ptr, 17, 1, 0, 1);

    strcpy(aname, &bufA.b[((u32)basyo_pack >> 12) * 20]);
    strcpy(mname, &bufB.b[((u32)basyo_pack >> 12) * 21]);

    class_clear_n(96);

    class_w[96].b1 = 11;
    class_w[96].b2 = 0;
    class_w[96].w56 = 0x01000000;
    class_w[96].b3 = 4;
    class_w[96].w12 = kangei_work_desc_tbl[1][0];
    class_w[96].w16 = kangei_work_desc_tbl[2][0];
    class_w[96].w52 = kangei_work_desc_tbl[0][0];
    class_w[96].h20 = (*(s16 *)&kangei_work_desc_tbl[3][0]);
    class_w[96].h22 = 0;
    class_w[96].h24 = 0;
    class_w[96].b6 = 1;
    class_w[96].b67 = 0;
    class_w[96].h38 = -160;
    class_w[96].h42 = -120;
    class_w[96].b7 = 0;
    class_w[96].b5 = 128;
    class_w[96].b4 = 0;

    srand(frame_counter);

    class_w[97] = class_w[96];
    class_w[97].b2 = 64;
    class_w[97].w12 = kangei_work_desc_tbl[1][1];
    class_w[97].w16 = kangei_work_desc_tbl[2][1];
    class_w[97].w52 = kangei_work_desc_tbl[0][1];
    class_w[97].h20 = (*(s16 *)&kangei_work_desc_tbl[3][1]);
    class_w[97].h38 = (s16)(rand() % 320 - 160);
    class_w[97].h42 = 60;

    for (i = 98; i < 116; i++) {
        class_w[i] = class_w[97];
        class_w[i].h24 = (s16)(rand() % 10);
    }

    for (i = 0; i < 10; i++) {
        kangei_streak_spd_wk[i] = (s16)(rand() % 5 + 1);
        kangei_streak_pos_wk[i].x = (s16)(rand() % 320 - 160);
        kangei_streak_pos_wk[i].y = 60;
    }

    sub_sub_step_up();
}
