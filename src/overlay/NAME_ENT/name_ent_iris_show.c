/* @SymbolStart: 0x8013B780 */
/* @SymbolSize: 396 */
/* name_ent_iris_show - NAME_ENT overlay, Address: 0x8013B788, Size: 388 bytes (97 insns) */
#include "types.h"

extern u8 now_sub_step;
extern u8 now_sub_sub_step;

extern s32 name_ent_iris_size;
extern s32 name_ent_iris_x;
extern s32 name_ent_iris_y;
extern s32 name_ent_iris_vx;
extern s32 name_ent_iris_vy;

extern s32 rand(void);
extern void iris_show(s32 a0, s32 a1, s32 a2, s32 a3,
                          s32 arg4, s32 arg5, s32 arg6, s32 arg7);

void name_ent_iris_show(void)
{
    if (now_sub_step != 0 || now_sub_sub_step != 0) {
        if (name_ent_iris_size < 16385) {
            name_ent_iris_size = (name_ent_iris_size * 16 + 112) / 15;
            name_ent_iris_x = name_ent_iris_x + name_ent_iris_vx;
            name_ent_iris_y = name_ent_iris_y + name_ent_iris_vy;
            if (name_ent_iris_x >= 161) {
                name_ent_iris_vx = -(rand() & 0xF);
            }
            if (name_ent_iris_y >= 121) {
                name_ent_iris_vy = -(rand() & 0xF);
            }
            if (name_ent_iris_x < -160) {
                name_ent_iris_vx = rand() & 0xF;
            }
            if (name_ent_iris_y < -120) {
                name_ent_iris_vy = rand() & 0xF;
            }
        }
    }
    iris_show(26, 0, 503, name_ent_iris_size, name_ent_iris_x, name_ent_iris_y,
                  0x01DF7FCF, 2);
}
