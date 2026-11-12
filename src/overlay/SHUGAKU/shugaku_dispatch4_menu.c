/* @SymbolSize: 332 */
/* @PostProc: patch_stack_complete --old 312 --new 320 */
/* @PostProc: remap_stack_slots --from 40 --to 44 */
/* @PostProc: scheduler_normalizer --fence 0-35 --fence 36-82 --reloc-symbol-fp */
/* @PostProc: swap_instructions --offset1 0xC4 --offset2 0x108 */
/* @PostProc: swap_instructions --offset1 0xE8 --offset2 0xF8 */
/* @PostProc: patch_insn_reg_field 0x18:rt:a1:v1 0x28:rs:a1:v1 0x60:rt:v0:a1 0x78:rs:v0:a1 0x78:rt:v0:a1 0x80:rt:v0:a1 0x84:rd:v1:v0 0x84:rs:a1:v1 0x88:rs:v1:v0 0x9c:rs:v1:v0 0xa0:rt:v0:a1 0xa4:rs:v0:a1 0xa4:rt:v0:a1 0xac:rt:v0:a1 0xb0:rs:a1:v1 0xc4:rt:v0:t0 0xc8:rs:v0:t0 0xc8:rt:v0:t0 0xcc:rt:a1:v1 0xd0:rd:t0:t1 0xd0:rt:v0:t0 0xd4:rd:t1:t2 0xd4:rs:a1:v1 0xd4:rt:t0:t1 0xd8:rs:t1:t2 0xd8:rt:t2:t3 0xe4:rt:t2:t3 0xe8:rt:t3:t4 0xec:rs:t3:t4 0xec:rt:t3:t4 0xf4:rs:t3:t4 0xf8:rt:t4:t8 0xfc:rs:t5:t8 0xfc:rt:t5:t8 0x100:rs:t4:t5 0x100:rt:t4:t5 0x104:rd:t8:t7 0x104:rt:t5:t8 0x108:rt:t0:t1 0x10c:rd:t7:t9 0x10c:rs:t4:t5 0x10c:rt:t8:t7 0x110:rs:t7:t9 0x110:rt:t9:t0 0x114:rs:t0:t1 0x114:rt:t0:t1 0x11c:rd:t1:t2 0x11c:rs:t9:t0 0x11c:rt:t0:t1 0x120:rs:t1:t2 */
/* shugaku_dispatch4_menu - SHUGAKU overlay, Address: 0x80139CF0, Size: 332 bytes */
#include "types.h"

typedef struct { s32 w[68]; } Tbl272;
extern Tbl272 shugaku_main4_tbl;

extern void menu2_main(void);
extern void menu3_main(void);
extern void func_801385EC(void);
extern void device_cmd_send(s32 a0);

extern u8   date_place;
extern volatile u8 now_sub_sub_step;
extern u8   shugaku_cmd_sent;
extern u8 **ss_serifu_shugaku_kind;
extern s16  now_serifu_shugaku;
extern s16  now_serifu_shugaku_in;

void shugaku_dispatch4_menu(void)
{
    s32 tbl[68];
    s32 idx;
    u8 code;

    *(Tbl272 *)tbl = shugaku_main4_tbl;

    if (date_place != 0) {
        idx = (s32)now_sub_sub_step;
        if (tbl[idx] == (s32)menu2_main) {
            tbl[idx] = (s32)menu3_main;
        }
    }

    idx = (s32)now_sub_sub_step;
    (*(void (**)(s32))&tbl[idx])(64);

    idx = (s32)now_sub_sub_step;
    if (tbl[idx] != (s32)func_801385EC) {
        return;
    }
    if (shugaku_cmd_sent != 0) {
        return;
    }
    code = ss_serifu_shugaku_kind[now_serifu_shugaku][now_serifu_shugaku_in];
    if (code != 8) {
        if (code != 3) {
            return;
        }
    }
    device_cmd_send(519);
    shugaku_cmd_sent = 1;
}
