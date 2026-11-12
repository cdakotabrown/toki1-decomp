/* @SymbolStart: 0x8013A960 */
/* @SymbolSize: 228 */
#include "types.h"

typedef struct { u8 flag; u8 pad[19]; } Ent;

typedef struct {
    u8  b0;
    u8  pad001[65];
    Ent e[24];
    u8  pad222[182];
    s16 h728[4];
} Serve;

extern Serve *tt_boss_wk;

extern s32 tt_cmd_once();
extern void tt_cell0_hit(void);
extern void tt_cell1_hit(void);
extern void tt_cell2_hit(void);
extern void tt_cell3_hit(void);
extern s32 tt_hit_check(void);

void tt_cell_disp()
{
    Serve *p;
    s32 i;

    p = tt_boss_wk;
    if (p->b0 == 0) {
        return;
    }
    for (i = 0; i < 24; i++) {
        p->e[i].flag = 0;
    }
    if (p->h728[0] >= 0) {
        tt_cell0_hit();
    }
    if (p->h728[1] >= 0) {
        tt_cell1_hit();
    }
    if (p->h728[2] >= 0) {
        tt_cell2_hit();
    }
    if (p->h728[3] >= 0) {
        tt_cell3_hit();
    }
    if (tt_hit_check() != 0) {
        tt_cmd_once(1288);
    }
}
