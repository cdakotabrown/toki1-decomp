/* @SymbolStart: 0x80133738 */
/* @SymbolSize: 308 */
/* Address: 0x80133740 */
/* Size: 300 bytes (75 insns) */
#include "types.h"

extern u8 now_sub_step;

extern u8 now_sub_sub_step;
extern u8 taco_load_no;
extern s32 pad_trig;
extern s32 frame_counter;
extern s32 vblnk_counter;
extern s16 *taco_actor_rec;
extern s32 taco_xa_tbl[];

extern s32 cdq_idle_check(void);
extern s32 get_CD_State(void);
extern s32 rand(void);
extern void set_XAsector(s32 a0, s32 a1, s32 a2);
extern void device_cmd_send(s32 a0);

void xa_rand_set(void)
{
    s32 arg0;
    s32 index;
    s32 *entry;

    arg0 = now_sub_step;
    index = 3;

    if ((arg0 & 1) == 0) {
        return;
    }

    if (now_sub_sub_step != 1) {
        return;
    }
    if (taco_load_no != 0) {
        return;
    }
    if ((pad_trig & 0x10) == 0) {
        return;
    }
    if ((frame_counter & 0xFF) == 0) {
        return;
    }
    if (cdq_idle_check() != 1) {
        return;
    }
    if ((get_CD_State() & 2) == 0) {
        return;
    }

    if (taco_actor_rec[604] < taco_actor_rec[60]) {
        index = (rand() & 1) + 2;
    } else if ((vblnk_counter & 1) != 0) {
        index = rand() % 6;
    }

    entry = &taco_xa_tbl[index * 2];
    set_XAsector(entry[0], entry[1], 8);
    device_cmd_send(768);
}
