/* @CompileFlags: -Wo,-loopunroll,0 -Wc,-pic1 */
/* @CompileEnv: IDO52_COPYPROP_VAR=-12 IDO52_AS1_NO_PASS1_PROP=13 */
/* @PostProc: swap_instructions --offset1 0x80 --offset2 0x88 */
/* @SymbolStart: 0x801484B4 */
/* @SymbolSize: 392 */
/* Address: 0x801484BC */
/* Size: 448 bytes (112 insns incl. the absorbed 8-byte cross-prologue */
#include "types.h"
#include "tokimeki/libgs_types.h"

extern s32 step_counter;
extern volatile s32 step_counter_v;
extern s32 taco_obj_3_attribute[];
extern s32 taco_obj_31_attribute[];
extern LIBGS_DOBJ2 taco_obj[];

extern void goto_sub_step(s32 a);
extern void sd_cmd_limit_send(s32 a, s32 b);
extern void taco_tile_load(s32 a, s32 b);
extern void taco_spark_group_step(s32 a);
extern void taco_band_disp(s32 a, s32 b, u8 c, u8 d);

void taco_maina_band_fade_step(void)
{
    s32 pad;
    s32 pad2;
    s32 snap;
    s32 cur;
    s32 fill;
    s32 *p;
    s32 *end;
    s32 kraw;
    s32 k;

    cur = step_counter_v;
    snap = cur;
    if (cur < 120) {
        if ((cur % 60) == 0) {
            sd_cmd_limit_send(0, 1288);
        }
        taco_spark_group_step(0);
    }
    if (snap == 100) {
        fill = (s32)0x80000000;
        (*(s32 *)&taco_obj[1].attribute) = fill;
        end = (s32 *)taco_obj_31_attribute;
        p = (s32 *)taco_obj_3_attribute;
        (*(s32 *)&taco_obj[2].attribute) = fill;
        do {
            p += 16;
            p[-16] = fill;
            p[-12] = fill;
            p[-8] = fill;
            p[-4] = fill;
        } while (p != end);
    }
    if (snap < 120) {
        taco_band_disp(1, 224, 224, 224);
    } else if ((snap >= 120) && (snap < 176)) {
        kraw = 704 - snap * 4;
        k = kraw & 0xFF;
        taco_band_disp(1, k, k | 0, k);
    } else if ((snap >= 176) && (snap < 240)) {
        kraw = snap * 4 - 704;
        k = kraw & 0xFF;
        taco_band_disp(2, k, k | 0, k);
    } else if (snap == 240) {
        taco_tile_load(0x80162000, 14);
        taco_tile_load(0x80172000, 12);
        goto_sub_step(21);
    }
    step_counter = step_counter + 1;
}
