/* @SymbolStart: 0x8013A034 */
/* @SymbolSize: 740 */
/* Address: 0x8013A0FC (true start 0x8013A034) */
#include "types.h"
#include "tokimeki/sprite.h"

extern void device_cmd_send();
extern void sub_sub_step_up();

typedef struct {
    u32 f0;
    s16 f4;
    s16 f6;
    s16 f8;
    s16 f10;
    s16 f12;
    u8 f14;
    u8 f15;
    s16 f16;
    s16 f18;
    u8 f20;
    u8 f21;
    u8 f22;
    s16 f24;
    s16 f26;
    s16 f28;
    s16 f30;
    u32 f32;
} EndRec;


extern u8 sprite_state4;
extern u8 sprite_state5;
extern u8 sprite_state6;
extern u8 sprite_state7;
extern u8 sprite_state8;
extern u8 sprite_state9;
extern SPRITE_REC Sprites[];

void ending_bs_sprite_init(void)
{
    EndRec *src;
    EndRec *dst;

    ((EndRec *)&Sprites[4].attribute)[0].f0 = 0x02000000;
    ((EndRec *)&Sprites[4].attribute)[0].f8 = 192;
    ((EndRec *)&Sprites[4].attribute)[0].f10 = 128;
    ((EndRec *)&Sprites[4].attribute)[0].f14 = 0;
    ((EndRec *)&Sprites[4].attribute)[0].f15 = 56;
    ((EndRec *)&Sprites[4].attribute)[0].f16 = 0;
    ((EndRec *)&Sprites[4].attribute)[0].f12 = 21;
    ((EndRec *)&Sprites[4].attribute)[0].f18 = 497;
    ((EndRec *)&Sprites[4].attribute)[0].f20 = 0;
    ((EndRec *)&Sprites[4].attribute)[0].f21 = 0;
    ((EndRec *)&Sprites[4].attribute)[0].f22 = 0;
    ((EndRec *)&Sprites[4].attribute)[0].f24 = 0;
    ((EndRec *)&Sprites[4].attribute)[0].f26 = 0;
    ((EndRec *)&Sprites[4].attribute)[0].f28 = 4096;
    ((EndRec *)&Sprites[4].attribute)[0].f30 = 4096;
    ((EndRec *)&Sprites[4].attribute)[0].f32 = 0;
    ((EndRec *)&Sprites[4].attribute)[0].f4 = -160;
    ((EndRec *)&Sprites[4].attribute)[0].f6 = -64;

    src = &((EndRec *)&Sprites[4].attribute)[0];
    dst = &((EndRec *)&Sprites[4].attribute)[1];

    sprite_state4 = 13;
    *dst = *src;
    ((EndRec *)&Sprites[4].attribute)[1].f8 = 128;
    ((EndRec *)&Sprites[4].attribute)[1].f12 = 24;
    ((EndRec *)&Sprites[4].attribute)[1].f4 = 32;

    sprite_state5 = 13;
    ((EndRec *)&Sprites[4].attribute)[2] = *src;
    ((EndRec *)&Sprites[4].attribute)[2].f15 = 0;
    ((EndRec *)&Sprites[4].attribute)[2].f10 = 56;
    ((EndRec *)&Sprites[4].attribute)[2].f6 = -120;

    sprite_state6 = 1;
    ((EndRec *)&Sprites[4].attribute)[3] = *dst;
    ((EndRec *)&Sprites[4].attribute)[3].f15 = 0;
    ((EndRec *)&Sprites[4].attribute)[3].f10 = 56;
    ((EndRec *)&Sprites[4].attribute)[3].f6 = -120;

    sprite_state7 = 1;
    ((EndRec *)&Sprites[4].attribute)[4] = *src;
    ((EndRec *)&Sprites[4].attribute)[4].f6 = 64;
    ((EndRec *)&Sprites[4].attribute)[4].f15 = 184;
    ((EndRec *)&Sprites[4].attribute)[4].f10 = 56;

    sprite_state8 = 1;
    ((EndRec *)&Sprites[4].attribute)[5] = *dst;
    ((EndRec *)&Sprites[4].attribute)[5].f6 = 64;
    ((EndRec *)&Sprites[4].attribute)[5].f15 = 184;
    ((EndRec *)&Sprites[4].attribute)[5].f10 = 56;

    sprite_state9 = 1;
    device_cmd_send(513);
    sub_sub_step_up();
}
