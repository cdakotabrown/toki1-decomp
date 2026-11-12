/* @CompileEnv: IDO52_AS1_NO_LA_PULL=10 */
/* @SymbolSize: 368 */
/* @PostProc: patch_stack_complete --old 248 --new 264 */
/* @PostProc: remap_stack_slots --from 36 --to 44 */
/* @PostProc: swap_instructions --offset1 0xe0 --offset2 0xe4 */
/* @PostProc: patch_insn_reg_field 0xe4:rt:v1:a0 0xfc:rt:t9:a3 0x10c:rt:a0:a3 0x110:rs:t9:a3 0x110:rt:t9:a3 0x118:rs:t9:a3 0x11c:rt:a0:a3 0x124:rt:a0:a3 0x128:rt:a0:a3 0x130:rt:a0:a3 0x138:rt:a0:a3 0x13c:rs:v1:a0 0x140:rs:v1:a0 0x140:rt:v1:a0 0x144:rd:t5:t4 0x144:rt:v1:a0 0x148:rd:v1:a0 0x148:rt:t5:t4 0x14c:rs:v1:a0 0x150:rd:t4:t9 0x154:rt:t4:t9 0x158:rt:a0:a3 0x15c:rt:a0:a3 0x164:rt:a0:a3 0x16c:rt:a0:a3 */
/* Address: 0x80155034 */
/* Size: 368 bytes (92 instructions) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[53];
} JumpTable;

extern JumpTable date_girl1_main5_tbl;
extern u8  now_sub_sub_step;
extern s32 pad_trig;
extern s32 frame_counter;
extern u8  date_ev0c_bri_flag;
extern u8  fade_bri;
extern u8  cal_cw_bright;
extern u8  cw97_bright;
extern u8  Sprites[];

void date_girl1_mode5_dispatch(void)
{
    JumpTable table;
    s32 val;
    s16 i;
    u8 *p;

    table = date_girl1_main5_tbl;

    table.ptrs[now_sub_sub_step](128);

    if (now_sub_sub_step == 14) {
        if ((pad_trig & 0xFF9FFF9F) != 0) {
            date_ev0c_bri_flag = (u8)(1 - date_ev0c_bri_flag);
        }
        if ((u32)frame_counter % 3U == 0) {
            if (date_ev0c_bri_flag != 0) {
                val = 128;
            } else if (fade_bri == 128) {
                val = 96;
            } else {
                val = 128;
            }
            cal_cw_bright = (u8)val;
            cw97_bright = (u8)val;
            i = 58;
            do {
                p = Sprites + (u32)i * 36U;
                p[22] = (u8)val;
                p[21] = (u8)val;
                p[20] = (u8)val;
                i = i + 1;
            } while (i < 64);
            fade_bri = (u8)val;
        }
    }
}
