/* @PostProc: patch_insn_reg_field 0x20:rt:v1:a3 0x24:rs:v1:a3 0x24:rt:v1:a3 0x2c:rt:v1:a3 0xa4:rt:v1:a3 0xa8:rs:v1:a3 0xa8:rt:v1:a3 0xac:rt:v0:a0 0xb0:rt:a0:a2 0xb4:rt:v1:a3 0xbc:rd:v0:a2 0xbc:rs:v0:a2 0xbc:rt:v1:a3 0xc0:rs:v0:a2 0xd0:rt:v1:a3 0xd4:rs:v1:a3 0xd4:rt:v1:a3 0xdc:rt:v1:a3 0xf0:rd:t0:t1 0xf0:rt:v1:a3 0xf4:rt:t1:t2 0xf8:rs:t1:t2 0xf8:rt:t1:t2 0xfc:rd:a3:v1 0xfc:rs:t0:t1 0xfc:rt:t1:t2 0x100:rs:a3:v1 0x104:rs:a3:v1 0x118:rt:v1:a3 0x11c:rs:v1:a3 0x11c:rt:v1:a3 0x124:rt:v1:a3 */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:6 */
/* name_ent_serifu_set - NAME_ENT overlay, Address: 0x80144A80, Size: 328 bytes */
#include "types.h"

typedef struct {
    s32 a0;
    s32 a1;
} PairEntry;

extern void k_sub_reset();
extern void gnsx();
extern void sndisp();
extern void set_XAsector();
extern void device_cmd_send();
extern void k_sub_disp_start();

extern volatile u8 name_ent_serifu_no;
extern volatile u8 name_ent_serifu_girl_tbl[];
extern volatile u8 p_sei[];
extern volatile u8 name_ent_serifu_set_str0[];
extern volatile u8 name_ent_serifu_set_str1[];
extern void *date_name_set_tbl0[];
extern void *name_ent_serifu_tbl[];
extern PairEntry name_ent_serifu_xa_tbl[];
extern volatile u8 now_girl;
extern volatile s32 name_ent_voice_on;

void name_ent_serifu_set(void)
{
    s32 idx;
    s32 val;
    PairEntry *entry;

    k_sub_reset();

    idx = name_ent_serifu_no;
    val = name_ent_serifu_girl_tbl[idx];
    if ((val & 0x0F) == 15) {
        gnsx(p_sei);
    } else if (val == 30) {
        gnsx(name_ent_serifu_set_str0);
    } else if (val == 46) {
        gnsx(name_ent_serifu_set_str1);
    } else {
        gnsx(date_name_set_tbl0[val]);
    }

    idx = name_ent_serifu_no;
    val = name_ent_serifu_girl_tbl[idx];
    sndisp(name_ent_serifu_tbl[idx], 0, val);

    idx = name_ent_serifu_no;
    val = name_ent_serifu_girl_tbl[idx];
    if ((val & 0x0F) != 15) {
        entry = &name_ent_serifu_xa_tbl[idx];
        set_XAsector(entry->a0, entry->a1, val);
        device_cmd_send(768);
        idx = name_ent_serifu_no;
        val = name_ent_serifu_girl_tbl[idx];
    }

    now_girl = val;
    name_ent_voice_on = 0;
    k_sub_disp_start(1);
}
