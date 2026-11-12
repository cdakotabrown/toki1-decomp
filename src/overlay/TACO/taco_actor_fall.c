/* @SymbolStart: 0x8015AB90 */
/* @SymbolSize: 104 */
#include "types.h"

typedef struct {
    u8  pad0[120];
    s16 f120;
    u8  pad122[14];
} TacoRec;

extern TacoRec *taco_actor_rec;

extern void taco_actor_exit(void);

void
taco_actor_fall(s32 idx)
{
    TacoRec *p;
    s32 ofs;

    ofs = idx * 136;
    p = &taco_actor_rec[idx];
    p->f120 = p->f120 - 250;
    p = (TacoRec *)(ofs + (u32)taco_actor_rec);
    if (p->f120 >= -30000) {
        return;
    }
    taco_actor_exit();
}
