/* @SymbolSize: 688 */
/* @CompileFlags: -cckr */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"

typedef struct {
    s32 v[10];
} PadSeq;

extern PadSeq rpg_bat_pad_seq_poll_tbl0;
extern PadSeq rpg_bat_pad_seq_poll_tbl1;
extern s32 rpg_bat_mode;
typedef struct {
    s32 pad;
    s32 prev;
    s32 curB;
    s32 curA;
} PadState;
extern PadState rpg_bat_pad_seq_work;
extern RPG_BAT_UNIT rpg_bat_unit0;
extern RPG_BAT_UNIT rpg_bat_unit1;

void rpg_bat_pad_seq_poll()
{
    PadSeq seqA;
    PadSeq seqB;
    s32 pad;

    seqA = rpg_bat_pad_seq_poll_tbl0;
    seqB = rpg_bat_pad_seq_poll_tbl1;
    pad = pad_get();
    rpg_bat_pad_seq_work.pad = pad;
    if (pad == rpg_bat_pad_seq_work.prev) {
        return;
    }
    rpg_bat_pad_seq_work.prev = pad;
    if ((pad & 0xFFFF) == 0) {
        return;
    }
    if (pad == seqA.v[rpg_bat_pad_seq_work.curA]) {
        rpg_bat_pad_seq_work.curA++;
    } else {
        rpg_bat_pad_seq_work.curA = 0;
    }
    if (pad == seqB.v[rpg_bat_pad_seq_work.curB]) {
        rpg_bat_pad_seq_work.curB++;
    } else {
        rpg_bat_pad_seq_work.curB = 0;
    }
    if ((rpg_bat_pad_seq_work.curA | 0) == 10) {
        rpg_bat_pad_seq_work.curA = 0;
        switch (rpg_bat_mode) {
        case 0x8000000:
            rpg_bat_unit1.hp = 1;
        case 0x10000:
        case 0x20000:
        case 0x40000:
            if (rpg_bat_unit0.hp >= 4) {
                rpg_bat_unit0.hp_max = 5730;
                rpg_bat_unit0.hp = 5730;
            }
            break;
        case 0x100000:
            if (rpg_bat_unit0.hp > 0 && rpg_bat_unit0.hp < 10) {
                rpg_bat_unit0.hp_max = 5729;
                rpg_bat_unit0.hp = 5729;
            }
        case 0x80000000:
            if (rpg_bat_unit0.hp >= 2 && rpg_bat_unit0.hp < 10) {
                rpg_bat_unit0.hp_max = 5729;
                rpg_bat_unit0.hp = 5729;
            }
            if (rpg_bat_unit0.hp >= 10) {
                rpg_bat_unit0.hp++;
                rpg_bat_unit0.hp_max++;
                if (rpg_bat_unit0.hp_max >= 10000) {
                    rpg_bat_unit0.hp_max = rpg_bat_unit0.hp = 9999;
                }
            }
            break;
        }
    }
    if ((rpg_bat_pad_seq_work.curB | 0) == 10) {
        rpg_bat_pad_seq_work.curB = 0;
        switch (rpg_bat_mode) {
        case 0x8000000:
        case 0x10000:
        case 0x20000:
        case 0x40000:
            break;
        case 0x100000:
            rpg_bat_unit0.hp = 1;
            break;
        case 0x80000000:
            rpg_bat_unit0.hp = 2;
            break;
        }
    }
}
