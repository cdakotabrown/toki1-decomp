/* @PostProc: patch_stack_complete --old 88 --new 96 */
/* @PostProc: remap_stack_slots --from 56 --to 60 --from 52 --to 56 --from 48 --to 52 --from 44 --to 48 --from 40 --to 44 */
/* @PostProc: patch_insn_reg_field 0xe4:rt:t1:v1 0xe8:rt:t5:t2 0xec:rd:t2:t1 0xec:rt:t1:v1 0xf0:rs:t2:t1 0xf0:rt:t5:t2 0x144:rt:t7:t5 0x148:rs:t7:t5 0x148:rt:t7:t5 0x15c:rs:t7:t5 0x15c:rt:t6:t7 0x160:rt:t6:t7 */
/* date2_xa_init -- DATE2 overlay, Address: 0x80138A90, Size: 348 bytes (87 insns) */
#include "types.h"

typedef struct { u32 w[8]; } Buf0;
typedef struct { u32 w[2]; } Buf2;

extern Buf0 date2_xa_init_tbl0;
extern Buf2 date2_xa_init_tbl1;
extern Buf2 date2_xa_init_tbl2;

extern s32 kuchi_stop;
extern u8 date2_song_no;
extern s32 serifu_no;
extern s16 date2_xa_timer;
extern u8 step_mode;

void device_cmd_send(s32 arg0);
void set_XAsector(s32 a0, s32 a1, s32 a2);

void date2_xa_init(void)
{
    Buf0 buf0;
    Buf2 buf1;
    Buf2 buf2;
    u8 cnt;
    s32 v1;
    s32 idx;
    u8 t5;

    buf0 = date2_xa_init_tbl0;
    buf1 = date2_xa_init_tbl1;
    buf2 = date2_xa_init_tbl2;

    kuchi_stop = 1;
    device_cmd_send(191);

    cnt = date2_song_no;
    if ((u32)cnt >= 2)
    {
        v1 = serifu_no;
        if (v1 != 1 && v1 != 2)
        {
            goto other;
        }
    }
    idx = serifu_no;
    set_XAsector((s32)buf0.w[idx * 2], (s32)buf0.w[idx * 2 + 1], 15);
    goto done;
other:
    if (v1 == 0)
    {
        set_XAsector((s32)buf1.w[0], (s32)buf1.w[1], 15);
    }
    else
    {
        set_XAsector((s32)buf2.w[0], (s32)buf2.w[1], 15);
    }

done:
    device_cmd_send(768);
    t5 = step_mode;
    date2_xa_timer = 0;
    step_mode = (u8)(t5 + 1);
}
