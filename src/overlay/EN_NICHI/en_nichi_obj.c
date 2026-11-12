/* en_nichi_obj - EN_NICHI overlay, Address: 0x80133A68, Size: 56 bytes (14 insns) */
#include "types.h"

extern void en_nichi_obj_out_clear();
extern void en_nichi_obj_spawn();
extern void en_nichi_obj_move_all();
extern void en_nichi_obj_disp();

void en_nichi_obj(void)
{
    en_nichi_obj_out_clear();
    en_nichi_obj_spawn(1);
    en_nichi_obj_move_all();
    en_nichi_obj_disp();
}
