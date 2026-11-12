/* @CompileFlags: -Wc,-pic0 */
/* @RodataBase: 0x8015DCA0 */
/* @SymbolStart: 0x80154960 */
/* @SymbolSize: 1532 */
/* @PostProc: patch_insn_reg_field 0x40c:rs:v1:v0 0x408:rt:v1:v0 0x40c:rt:v1:v0 0x414:rs:v1:v0 0x414:rt:v1:v0 0x418:rt:v1:v0 0x41c:rs:v1:v0 0x5cc:rt:v1:v0 0x5d0:rs:v1:v0 0x5d0:rt:v1:v0 0x5d8:rs:v1:v0 0x5d8:rt:v1:v0 0x5dc:rt:v1:v0 0x5e0:rs:v1:v0 */
/* @PostProc: remap_stack_slots --map '32:48' */
/* Address: 0x80154968 */
/* Size: 1532 bytes / 383 insns over the windowed symbol (@SymbolStart */
#include "types.h"

typedef struct {
    u8 pad0[4];
    u8 f4;
    u8 f5;
    u8 f6;
    u8 f7;
    s16 x0;
    s16 y0;
    s16 x1;
    s16 y1;
    s16 x2;
    s16 y2;
    s16 x3;
    s16 y3;
} TacoQuad;

extern s16 screen_w[];
extern u8 ot_buff0[];
extern volatile s32 activeBuff;
extern u8 *taco_actor_rec;
extern s32 taco_spawn_slot;
extern volatile s32 taco_phase_cnt;
extern s32 taco_phase_no;

extern void sub_sub_step_up(void);
extern void *prim_alloc(s32, s32);
extern s32 dtd_on_tpage(s32, s32, s32, s32, s32);
extern void AddPrim(u8 *, void *);
extern void SetSemiTrans(void *, s32);
extern void SetPolyF4(void *);
extern void taco_spawn_rand(void);
extern void taco_column_spawn(void);
extern void taco_scatter_spawn(void);
extern s32 taco_spawn_up(void);
extern void taco_spawn_group(s32);
extern void taco_phase_next(void);
extern void taco_actor_pool_clear(void);
extern void taco_actor_main(void);
extern void taco_actor_clear(void);
extern s32 taco_actor_count_owned(s32);

void taco_phase_main(void)
{
    s32 v0;
    s32 v1;
    s32 v2;
    s32 v3;
    s32 v4;
    TacoQuad *p;

    v0 = taco_phase_cnt + 1;
    taco_phase_cnt = v0;
    taco_actor_main();

    switch ((u32)taco_phase_no) {
    case 0:
        v0 = taco_phase_cnt;
        if (v0 < 2100) {
            taco_phase_cnt = v0;
            if (v0 % 7 == 0) {
                taco_spawn_rand();
            }
        }
        v1 = taco_phase_cnt;
        if (v1 < 1200) {
            taco_phase_cnt = v1;
            if (v1 % 50 == 0) {
                taco_spawn_group(0);
            }
        }
        v2 = taco_phase_cnt;
        if (v2 >= 601) {
            taco_phase_cnt = v2;
            if (v2 < 1800) {
                if (taco_actor_count_owned(4) == 0) {
                    taco_column_spawn();
                }
            }
        }
        v3 = taco_phase_cnt;
        if (v3 >= 1801) {
            taco_phase_next();
        }
        break;

    case 1:
        taco_spawn_slot = taco_spawn_up();
        taco_phase_next();
        break;

    case 2:
        v0 = taco_phase_cnt;
        if (v0 < 600) {
            taco_phase_cnt = v0;
            if (v0 % 7 == 0) {
                taco_spawn_rand();
            }
        }
        v1 = taco_phase_cnt;
        if (v1 < 300) {
            if (taco_actor_count_owned(4) == 0) {
                taco_column_spawn();
            }
        }
        v2 = taco_phase_cnt;
        if (v2 >= 481) {
            taco_phase_cnt = v2;
            if (v2 % 110 == 0) {
                taco_spawn_group(1);
            }
        }
        v3 = taco_phase_cnt;
        if (v3 >= 301) {
            taco_phase_cnt = v3;
            if (v3 % 120 == 0) {
                taco_scatter_spawn();
            }
        }
        if (taco_actor_rec[taco_spawn_slot * 136 + 3] == 2) {
            taco_phase_next();
        }
        break;

    case 3:
        taco_actor_clear();
        taco_phase_next();
        break;

    case 4:
        p = (TacoQuad *)prim_alloc(24, activeBuff);
        SetPolyF4(p);
        SetSemiTrans(p, 1);
        p->f4 = taco_phase_cnt;
        p->f5 = taco_phase_cnt;
        p->f6 = taco_phase_cnt;
        p->x0 = screen_w[activeBuff + 10] - 256;
        p->y0 = screen_w[activeBuff + 12] - 120;
        p->x1 = screen_w[activeBuff + 10] + 256;
        p->y1 = screen_w[activeBuff + 12] - 120;
        p->x2 = screen_w[activeBuff + 10] - 256;
        p->y2 = screen_w[activeBuff + 12] + 120;
        p->x3 = screen_w[activeBuff + 10] + 256;
        p->y3 = screen_w[activeBuff + 12] + 120;
        AddPrim(&ot_buff0[activeBuff * 1024], p);
        dtd_on_tpage(0, 0, 0, 1, 0);
        v4 = taco_phase_cnt + 2;
        taco_phase_cnt = v4;
        if (v4 >= 128) {
            taco_phase_next();
        }
        break;

    case 5:
        p = (TacoQuad *)prim_alloc(24, activeBuff);
        SetPolyF4(p);
        SetSemiTrans(p, 1);
        p->f4 = 128 - taco_phase_cnt;
        p->f5 = 128 - taco_phase_cnt;
        p->f6 = 128 - taco_phase_cnt;
        p->x0 = screen_w[activeBuff + 10] - 256;
        p->y0 = screen_w[activeBuff + 12] - 120;
        p->x1 = screen_w[activeBuff + 10] + 256;
        p->y1 = screen_w[activeBuff + 12] - 120;
        p->x2 = screen_w[activeBuff + 10] - 256;
        p->y2 = screen_w[activeBuff + 12] + 120;
        p->x3 = screen_w[activeBuff + 10] + 256;
        p->y3 = screen_w[activeBuff + 12] + 120;
        AddPrim(&ot_buff0[activeBuff * 1024], p);
        dtd_on_tpage(0, 0, 0, 1, 0);
        v4 = taco_phase_cnt + 3;
        taco_phase_cnt = v4;
        if (v4 >= 128) {
            taco_actor_pool_clear();
            sub_sub_step_up();
        }
        break;
    }
}
