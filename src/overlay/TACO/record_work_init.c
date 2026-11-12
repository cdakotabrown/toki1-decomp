/* @SymbolStart: 0x801345E4 */
/* @SymbolSize: 376 */
/* Address: 0x801345EC (true entry 0x801345E4, cross-prologue hoisted load) */
/* Size: 376 bytes (94 insns) counting the two hoisted words */
#include "types.h"

typedef union {
    volatile struct {
        u32 w0 : 1;
        u32 w1 : 1;
        u32 w2 : 1;
        u32 w3 : 2;
    } w;
    volatile struct {
        u16 g0 : 12;
        u16 g12 : 4;
    } h;
    volatile struct {
        u8 z0 : 1;
        u8 z1 : 7;
        u8 e0 : 4;
    } b;
} TacoFlags;

typedef struct {
    u8 pad[64];
    TacoFlags fl;
} TacoBlk;

extern void strncpy(u8 *, u8 *, s32);
extern void strcpy(u8 *, u8 *);

extern TacoBlk album_work;
extern u32 taco_rec;
extern u8 taco_rec_name0_0;
extern u8 taco_rec_name1_0;
extern u8 taco_rec_name2_0;
extern u32 taco_rec_score0;
extern u32 taco_rec_score1;
extern u32 taco_rec_score2;

extern u8 record_work_init_str0;
extern u8 record_work_init_str1;
extern u8 record_work_init_str2;
extern u8 taco_rensya_flag;
extern u8 taco_turn_btn;
extern u8 taco_shot_btn;
extern u8 taco_burst_btn;
extern s32 taco_rank_score[];
extern u8 taco_rank_name[][4];

void record_work_init(void)
{
    TacoBlk *p;

    if (taco_rec & 1) {
        p = &album_work;
        (*(u32 *)&taco_rank_score[0]) = taco_rec_score0;
        (*(u32 *)&taco_rank_score[1]) = taco_rec_score1;
        (*(u32 *)&taco_rank_score[2]) = taco_rec_score2;
        taco_rensya_flag = p->fl.w.w2;
        taco_turn_btn = p->fl.w.w3;
        taco_shot_btn = p->fl.h.g12;
        taco_burst_btn = p->fl.b.e0;
        strncpy(&taco_rank_name[0][0], &taco_rec_name0_0, 3);
        strncpy(&taco_rank_name[1][0], &taco_rec_name1_0, 3);
        strncpy(&taco_rank_name[2][0], &taco_rec_name2_0, 3);
    } else {
        p = &album_work;
        p->fl.b.z0 = 1;
        (*(u32 *)&taco_rank_score[0]) = 0x2EA694;
        (*(u32 *)&taco_rank_score[1]) = 0x1F6454;
        (*(u32 *)&taco_rank_score[2]) = 0x102214;
        strcpy(&taco_rank_name[0][0], &record_work_init_str0);
        strcpy(&taco_rank_name[1][0], &record_work_init_str1);
        strcpy(&taco_rank_name[2][0], &record_work_init_str2);
    }
}
