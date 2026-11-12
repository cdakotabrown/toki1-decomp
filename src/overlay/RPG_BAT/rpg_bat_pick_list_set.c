/* @SymbolSize: 544 */
/* @CompileFlags: -Wo,-loopunroll,0 -Wc,-pic0 */
/* @PostProc: swap_instructions --offset1 0x120 --offset2 0x124 */
/* @PostProc: patch_insn_reg_field 0x170:rt:v0:v1 0x174:rs:v0:v1 0x174:rt:v0:v1 0x17c:rs:v0:v1 0x184:rs:v0:v1 0x190:rs:v0:v1 */
/* rpg_bat_pick_list_set - RPG_BAT overlay, Address: 0x80152384, Size: 544 bytes */
/* rpg_bat_disp_init.  Windowed with @SymbolSize: 544. */
#include "types.h"

extern s32  get_rand_mod(s32);
extern void strcpy(u8 *, u8 *);
extern void strcat(u8 *, u8 *);

typedef struct {
    u8 *v[4];
} StrSet;

typedef struct {
    s32 v[4];
} FlagSet;

typedef struct {
    FlagSet set;
    s32     spare[3];
} FlagArea;

extern StrSet  rpg_bat_pick_list_set_tbl0;
extern StrSet  rpg_bat_pick_list_set_tbl1;
extern StrSet  rpg_bat_pick_list_set_tbl2;
extern FlagSet rpg_bat_pick_list_set_tbl3;

extern u8  *rpg_bat_msg_tbl4[];
extern u8   rpg_bat_pick_list_set_str0[];
extern s32  rpg_bat_mode;
extern s32 rpg_bat_seq_work[];

void rpg_bat_pick_list_set(void)
{
    StrSet  setA;
    StrSet  setB;
    StrSet  setC;
    FlagArea flags;
    s32 cnt;
    s32 i;
    s32 n;
    s32 sel;

    setA = rpg_bat_pick_list_set_tbl0;
    setB = rpg_bat_pick_list_set_tbl1;
    setC = rpg_bat_pick_list_set_tbl2;
    flags.set = rpg_bat_pick_list_set_tbl3;
    cnt = 0;
    if (get_rand_mod(5) == 0) {
        n = 3;
    } else {
        n = 2;
    }
    for (i = 0; i < n; i++) {
        flags.set.v[get_rand_mod(4)] = 1;
    }
    for (i = 0; i < 4; i++) {
        if (flags.set.v[i] == 1) {
            strcpy(rpg_bat_msg_tbl4[cnt + 4], rpg_bat_pick_list_set_str0);
            sel = rpg_bat_mode;
            switch (sel) {
            case 0x10000:
                strcat(rpg_bat_msg_tbl4[cnt + 4], setA.v[i]);
                break;
            case 0x20000:
                strcat(rpg_bat_msg_tbl4[cnt + 4], setB.v[i]);
                break;
            case 0x40000:
                strcat(rpg_bat_msg_tbl4[cnt + 4], setC.v[i]);
                break;
            }
            cnt++;
        }
    }
    rpg_bat_seq_work[40] = cnt + 4;
}
