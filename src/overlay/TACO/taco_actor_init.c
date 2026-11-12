/* Address: 0x801322AC */
/* Size: 364 bytes (91 insns) */
/* @SymbolSize: 352 */
#include "types.h"

extern void taco_model_link();
extern void taco_actor_reset();

extern u8 *volatile taco_actor_rec;
extern u8 taco_bonus_timer;
extern s32 taco_bonus_score;

void taco_actor_init(void) {
    s32 neg1 = -1;

    taco_model_link((void *)0x8019C800, 0, taco_actor_rec);

    *(u8 *)(taco_actor_rec + 1) = 128;
    *(u16 *)(taco_actor_rec + 108) = 1024;
    *(u16 *)(taco_actor_rec + 110) = 1024;
    *(u16 *)(taco_actor_rec + 112) = 1024;

    taco_model_link((void *)0x8019C800, 1, (void *)(taco_actor_rec + 1088));

    *(u8 *)(taco_actor_rec + 1089) = 128;
    *(u16 *)(taco_actor_rec + 1196) = 1024;
    *(u16 *)(taco_actor_rec + 1198) = 1024;
    *(u16 *)(taco_actor_rec + 1200) = 1024;
    *(u16 *)(taco_actor_rec + 122) = 0;
    *(u16 *)(taco_actor_rec + 1210) = 0;
    *(u16 *)(taco_actor_rec + 114) = (u16)neg1;
    *(u16 *)(taco_actor_rec + 1202) = (u16)neg1;
    *(u16 *)(taco_actor_rec + 130) = (u16)neg1;
    *(u16 *)(taco_actor_rec + 1218) = (u16)neg1;
    *(u16 *)(taco_actor_rec + 124) = 4;
    *(u16 *)(taco_actor_rec + 126) = 3;
    *(u16 *)(taco_actor_rec + 128) = 0;
    *(u8 *)(taco_actor_rec + 133) = 255;
    *(u8 *)(taco_actor_rec + 1221) = 128;

    taco_actor_reset();

    taco_bonus_timer = 0;
    taco_bonus_score = 0;
}
