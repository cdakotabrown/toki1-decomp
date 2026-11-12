/* Function: vblank_function */
/* Address: 0x80043750 */
/* Size: 920 bytes (230 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=24:s2 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: swap_instructions --offset1 0x98 --offset2 0xA4 */
/* @PostProc: swap_instructions --offset1 0xA0 --offset2 0xA4 */
/* @PostProc: swap_instructions --offset1 0x13C --offset2 0x140 */
/* @PostProc: swap_instructions --offset1 0x1A8 --offset2 0x1B0 */
/* `@CompileFlags: -Wc,-pic1` is the WRONG lever here: it also reaches 168 */
/* `multu`.  `@CompileEnv: IDO52_CONST_NOINREG_VALUES=36` alone is worth */











































































































#include "types.h"

extern s32 vblank_function_tbl0;
extern s32 vblank_function_tbl1;
extern s32 vblank_function_tbl2;
extern s32 vblank_function_tbl3;
extern u8 class_flag;
extern u8 vblank_flag;
extern u8 scene_sub_a;
extern s32 vb_ready;
extern s32 activeBuff;
extern u8 ot_work;
extern u8 scene_sub_b;
extern u8 screen_w;
extern u8 now_day;
extern u8 now_step;
extern u8 now_sub_step;
extern s32 vcount_lim;
extern u8 back_clear_r;
extern u8 back_clear_g;
extern u8 back_clear_b;
extern u8 back_clear_flag;
extern s32 Sprites;
extern s32 Bg;
extern s32 object[];
extern s32 ref_view[];

extern s32 GsGetVcount(void);
extern void set_brightness(void);
extern void sprite_slot_disp(void);
extern void srn_set(s32);
extern void GsSortSprite(s32, s32, s32);
extern void GsSortFastSprite(s32, s32, s32);
extern void disp_string(void);
extern void GsSortBg(s32, s32, s32);
extern void GsSetRefView2(s32);
extern void GsGetLws(s32, s32 *, s32 *);
extern void GsSetLightMatrix(s32 *);
extern void GsGetLs(s32, s32 *);
extern void GsSetLsMatrix(s32 *);
extern void GsSortObject4(s32, s32, s32, s32);
extern void object3d_sort(void);
extern void func_8009A53C(s32, s32);
extern void image_upload_queue_flush(s32);
extern void palette_load_vram(s32);
extern void DrawSync(s32);
extern void VSync(s32);
extern void pad_main(void);
extern void ResetGraph(s32);
extern void GsClearVcount(void);
extern void GsSwapDispBuff(void);
extern void GsSortClear(s32, s32, s32, s32);
extern void GsDrawOt(s32);

void vblank_function(s32 arg0)
{
    s32 pad0[6];
    s32 sp70[8];
    s32 sp50[8];
    s32 i;
    u8 *endp;
    u8 *ptr;
    s32 val;
    s32 baseAddr;
    s32 mulVal;
    (void)&pad0[0];

#line 188
    vblank_function_tbl1 = GsGetVcount();
    set_brightness();

    if (class_flag == 1) {
        sprite_slot_disp();
    }

    if (vblank_flag != 0) {
        srn_set(0);
    }

    if (scene_sub_a != 0 && vb_ready != 0) {
#line 5
        baseAddr = (s32)&Sprites;
#line 10
        endp = &now_day;
#line 5
        ptr = &screen_w;
        do {
            val = ptr[4261];
            if (val & 0x80) {
                GsSortSprite(baseAddr, (s32)&ot_work + (activeBuff * 20), (u8)(val - 1) & 0x7F);
            } else {
                GsSortFastSprite(baseAddr, (s32)&ot_work + (activeBuff * 20), (u8)(val - 1) & 0x7F);
            }
            ptr++;
            baseAddr += 0x24;
        } while ((u32)ptr < (u32)endp);

        disp_string();

        if (vb_ready != 0) {
            i = 0;
            endp = &now_day;
        ptr = &screen_w;
            do {
                val = ptr[4332];
                if ((u32)val < 128u) {
                    mulVal = activeBuff * 20;
                    GsSortBg((s32)&Bg + (i * 0x24), (s32)&ot_work + mulVal, (u16)(val - 1));
                }
                i++;
                ptr++;
            } while (i != 2);
        }
    }

    if (scene_sub_b != 0) {
        GsSetRefView2((s32)ref_view);
        ptr = (u8 *)object;
        do {
            GsGetLws(*(s32 *)(ptr + 4), &sp50, &sp70);
            GsSetLightMatrix(&sp50);
            GsGetLs(*(s32 *)(ptr + 4), &sp70);
            GsSetLsMatrix(&sp70);
            mulVal = activeBuff * 20;
            GsSortObject4((s32)ptr, (s32)&ot_work + mulVal, 6, (s32)0x1F800000);
            ptr += 16;
        } while (ptr != (u8 *)ref_view);
        object3d_sort();
    }

    if (arg0 != 0) {
        vblank_function_tbl2 = GsGetVcount();
        image_upload_queue_flush(0);
        palette_load_vram(0);
        DrawSync(0);
        vblank_function_tbl3 = GsGetVcount();

        if (now_step == 0x90 && (u32)now_sub_step >= 10u) {
            VSync(2);
        } else {
            if (GsGetVcount() < vcount_lim) {
                VSync(0);
            }
        }

        vblank_function_tbl0 = GsGetVcount();
        pad_main();
    }

    ResetGraph(1);
    GsClearVcount();
    GsSwapDispBuff();

    if (back_clear_flag != 0) {
        mulVal = activeBuff * 20;
        GsSortClear(back_clear_r, back_clear_g, back_clear_b, (s32)&ot_work + mulVal);
    }

    mulVal = activeBuff * 20;
    GsDrawOt((s32)&ot_work + mulVal);
}
