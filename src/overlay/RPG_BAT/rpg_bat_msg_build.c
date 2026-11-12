/* @SymbolSize: 516 */
/* @RodataBase: 0x8015E3B0 */
/* @CompileFlags: -Wo,-loopunroll,0 -Wc,-pic0 */
/* rpg_bat_msg_build - RPG_BAT overlay, Address: 0x80159B40, Size: 516 bytes */
/* kosiki_talk.  Windowed with @SymbolSize: 516. */
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

extern StrSet  rpg_bat_msg_build_tbl0;
extern StrSet  rpg_bat_msg_build_tbl1;
extern FlagSet rpg_bat_msg_build_tbl2;

extern u8  *rpg_bat_msg_tbl4[];
extern u8   rpg_bat_msg_build_str0[];
extern u8   rpg_bat_msg_build_str1[];
extern s32 rpg_bat_seq_work[];

void rpg_bat_msg_build(void)
{
    StrSet   setA;
    StrSet   setB;
    FlagArea flags;
    s32 cnt;
    s32 i;
    s32 n;

    setA = rpg_bat_msg_build_tbl0;
    setB = rpg_bat_msg_build_tbl1;
    flags.set = rpg_bat_msg_build_tbl2;
    cnt = 0;
    if (get_rand_mod(5) == 0) {
        n = 3;
    } else {
        n = 2;
    }
    for (i = 0; i < n; i++) {
        switch ((u32)get_rand_mod(10)) {
        case 0:
        case 1:
        case 2:
        case 3:
            flags.set.v[0] = 1;
            break;
        case 4:
        case 5:
        case 6:
            flags.set.v[1] = 1;
            break;
        case 7:
        case 8:
            flags.set.v[2] = 1;
            break;
        case 9:
            flags.set.v[3] = 1;
            break;
        }
    }
    for (i = 0; i < 4; i++) {
        if (flags.set.v[i] == 1) {
            strcpy(rpg_bat_msg_tbl4[cnt + 3], rpg_bat_msg_build_str0);
            strcat(rpg_bat_msg_tbl4[cnt + 3], setA.v[i]);
            cnt++;
        }
    }
    strcpy(rpg_bat_msg_tbl4[cnt + 3], rpg_bat_msg_build_str1);
    strcat(rpg_bat_msg_tbl4[cnt + 3], setB.v[get_rand_mod(4)]);
    rpg_bat_seq_work[40] = cnt + 4;
}
