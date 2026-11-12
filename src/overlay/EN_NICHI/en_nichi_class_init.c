/* en_nichi_class_init - EN_NICHI overlay, Address: 0x80132450, Size: 780 bytes (195 insns) */
/* @CompileEnv: IDO52_NO_LOOP_TEST_REPLACEMENT=1 */
/* @PostProc: swap_instructions --offset1 0x1d4 --offset2 0x1d8 */
/* @PostProc: swap_instructions --offset1 0x1d0 --offset2 0x1d4 */
/* @PostProc: swap_instructions --offset1 0x1e4 --offset2 0x1e8 */
/* @PostProc: swap_instructions --offset1 0x1f0 --offset2 0x1f4 */
#include "types.h"
#include "tokimeki/class_w.h"

extern void load_tpage_buf_lock();
extern void class_clear_n();

extern u8 class_w[];
extern s16 cursor_cw_x;
extern s16 cursor_cw_y;
extern u32 *en_nichi_intro_tpage_ptr;
extern u32 en_nichi_intro_cels_ptr;
extern u32 en_nichi_intro_anim_ptr;


void en_nichi_class_init(void)
{
    s32 s1;
    u8 *s0;
    u8 *iter;
    u8 *end;

    load_tpage_buf_lock(5, en_nichi_intro_tpage_ptr[0], 1);
    load_tpage_buf_lock(37, en_nichi_intro_tpage_ptr[1], 1);
    load_tpage_buf_lock(6, en_nichi_intro_tpage_ptr[2], 1);
    load_tpage_buf_lock(7, (s32)0x801BA000U, 1);
    load_tpage_buf_lock(8, (s32)0x801BA000U, 1);
    load_tpage_buf_lock(9, (s32)0x801BA000U, 1);
    load_tpage_buf_lock(39, (s32)0x801BA000U, 1);
    load_tpage_buf_lock(40, (s32)0x801BA000U, 1);
    load_tpage_buf_lock(41, (s32)0x801BA000U, 1);

    s0 = class_w;
    for (s1 = 0; s1 < 64; s1++) {
        class_clear_n(96 + s1);
        s0[6529] = 7;
        *(u32 *)(s0 + 6584) = 0x01000000U;
        s0[6535] = 0;
        s0[6530] = 64;
        s0[6531] = 4;
        s0[6534] = 1;
        *(s16 *)(s0 + 6566) = -160;
        *(s16 *)(s0 + 6570) = -120;
        s0[6533] = 0;
        s0[6595] = 17;
        s0[6532] = 0;
        *(u32 *)(s0 + 6540) = en_nichi_intro_cels_ptr;
        *(u32 *)(s0 + 6544) = en_nichi_intro_anim_ptr;
        *(u32 *)(s0 + 6580) = (u32)en_nichi_intro_tpage_ptr;
        s0 += 68;
    }

    ((CLASS_W *)class_w)[154].disp_flag = 128;
    ((CLASS_W *)class_w)[154].frame = 34;
    ((CLASS_W *)class_w)[153].disp_flag = 128;
    ((CLASS_W *)class_w)[153].frame = 32;
    ((CLASS_W *)class_w)[152].prio = 3;
    ((CLASS_W *)class_w)[152].disp_flag = 132;
    ((CLASS_W *)class_w)[152].frame = 31;
    ((CLASS_W *)class_w)[152].x.half[1] = cursor_cw_x;
    ((CLASS_W *)class_w)[152].y.half[1] = cursor_cw_y;

    end = class_w + 7888;
    s0 = class_w + 6528;
    do {
        s0 += 272;
        *(u8 *)(s0 - 203) = 3;
        *(s16 *)(s0 - 180) = 46;
        *(u8 *)(s0 - 135) = 3;
        *(s16 *)(s0 - 112) = 46;
        *(u8 *)(s0 - 67) = 3;
        *(s16 *)(s0 - 44) = 46;
        *(u8 *)(s0 - 271) = 3;
        *(s16 *)(s0 - 248) = 46;
    } while (s0 != end);

    ((CLASS_W *)class_w)[155].prio = 2;
    ((CLASS_W *)class_w)[155].frame = 45;
    ((CLASS_W *)class_w)[155].attribute = 0x51000000U;
    ((CLASS_W *)class_w)[155].clut = 30;
    ((CLASS_W *)class_w)[144].prio = 2;
    ((CLASS_W *)class_w)[144].x.half[1] = 16;
    ((CLASS_W *)class_w)[144].y.half[1] = (s16)-8;
    ((CLASS_W *)class_w)[145].prio = 2;
    ((CLASS_W *)class_w)[145].x.half[1] = 0;
    ((CLASS_W *)class_w)[145].y.half[1] = (s16)-8;
    ((CLASS_W *)class_w)[146].prio = 2;
    ((CLASS_W *)class_w)[146].x.half[1] = 16;
    ((CLASS_W *)class_w)[146].y.half[1] = 24;
    ((CLASS_W *)class_w)[147].prio = 2;
    ((CLASS_W *)class_w)[147].x.half[1] = 0;
    ((CLASS_W *)class_w)[147].y.half[1] = 24;
}
