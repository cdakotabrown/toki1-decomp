/* @SymbolStart: 0x8013B368 */
/* @SymbolSize: 836 */
/* name_ent_status_disp - NAME_ENT overlay, Address: 0x8013B370 (true entry 0x8013B368, */
/* Size: 836 bytes (209 insns) counting the head. */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* @CompileEnv: IDO52_CONST_NOINREG_VALUES=3840,13 IDO52_FORM_SITE=lda:0:97:0 IDO52_NO_LOOP_TEST_REPLACEMENT=occ:1 IDO52_COPYPROP_SITE=lod:2,lod:7 */
/* @PostProc: swap_instructions --offset1 0x30 --offset2 0x34 */
/* @PostProc: swap_instructions --offset1 0x68 --offset2 0x6c */
/* @PostProc: swap_instructions --offset1 0x70 --offset2 0x74 */
/* @PostProc: swap_instructions --offset1 0x7c --offset2 0x80 */
/* @PostProc: swap_instructions --offset1 0x1c0 --offset2 0x1c4 */
/* @PostProc: swap_instructions --offset1 0x1f8 --offset2 0x1fc */
/* @PostProc: swap_instructions --offset1 0x200 --offset2 0x204 */
/* @PostProc: swap_instructions --offset1 0x20c --offset2 0x210 */
/* @PostProc: patch_insn_reg_field 0x54:rt:v1:a1 0x58:rs:v1:a1 0x58:rt:v1:a1 0x60:rs:v1:a1 0x60:rt:v1:a2 0x64:rt:v1:a2 0x68:rt:v1:a2 0x74:rt:a1:a2 0x78:rs:a1:a2 0x78:rt:a1:a2 0xa0:rt:a1:a2 0xa8:rt:v0:v1 0xac:rs:v0:v1 0x1a0:rt:a0:t0 0x1a4:rs:a0:t0 0x1e4:rt:v1:a1 0x1e8:rs:v1:a1 0x1e8:rt:v1:a1 0x1f0:rs:v1:a1 0x1f0:rt:v1:a2 0x1f4:rt:v1:a2 0x1f8:rt:v1:a2 0x204:rt:a1:a2 0x208:rs:a1:a2 0x208:rt:a1:a2 0x230:rt:a1:a2 0x238:rt:v0:v1 0x23c:rs:v0:v1 0x2dc:rt:t9:t0 0x2e0:rs:t9:t0 0x2e0:rt:t9:t0 0x2e8:rd:t8:t9 0x2e8:rt:t9:t0 0x2ec:rd:t8:t9 0x2ec:rs:t8:t9 0x2ec:rt:t9:t0 0x2f0:rd:t8:t9 0x2f0:rt:t8:t9 0x2f4:rt:t8:t9 0x308:rt:t1:t0 0x30c:rs:t1:t0 0x30c:rt:t1:t0 0x314:rd:t2:t8 0x314:rt:t1:t0 0x318:rd:t2:t8 0x318:rs:t2:t8 0x318:rt:t1:t0 0x31c:rd:t2:t8 0x31c:rt:t2:t8 0x320:rt:t2:t8 */
#include "types.h"
#include "tokimeki/class_w.h"

extern void name_num_disp(s32, s32, s32);

extern u8  card_sys_present;
extern u8  album_work[];
extern u8  last_album;
extern u8  cw96_active[];
extern s8  now_icon;
extern u8  name_buf0[];
extern u8  name_buf1[];
extern u8  name_buf2[];
extern u8  album_slot;
extern u8  now_album;
extern u8  album_chr[];
extern CLASS_W class_w[];

void name_ent_status_disp(void)
{
    s32 i;
    s32 unused0;
    s32 unused1;
    s32 unused2;
    s32 unused3;
    s32 count;
    s32 sel;
    s32 cur;
    s32 k;
    u8 *cp;
    u8 *p;
    u8 *pend;
    u8 *psel;

    if (card_sys_present != 0) {
        count = 0;
        cp = album_work;
        do {
            if (cp[20] != 0) {
                count++;
            }
            cp++;
        } while (cp < album_work + 13);

        sel = last_album - 1;
        psel = &cw96_active[sel * 68];
        pend = &cw96_active[14 * 68];
        p = cw96_active;
        do {
            if (p == psel) {
                p[339] = 17;
            } else {
                p[339] = 0;
            }
            p += 68;
        } while (p != pend);

        cur = now_icon;
        if (cur != -1) {
            class_w[119].disp_flag |= 0x80;
            class_w[119].x.half[1] = (s16)((cur / 7) * 31 - 130);
            class_w[119].y.half[1] = (s16)((cur % 7) * 16 - 11);
        }
        name_num_disp(name_buf0[last_album], 48, -96);
        name_num_disp(name_buf1[last_album], 80, -96);
        name_num_disp(name_buf2[last_album], 112, -96);
        name_num_disp(count, 90, -29);
    } else {
        k = album_slot;
        count = 0;
        i = 0;
        cp = &album_chr[k * 3840];
        do {
            if (cp[1672] != 0) {
                count++;
            }
            i++;
            cp++;
        } while (i < 13);

        sel = now_album - 1;
        psel = &cw96_active[sel * 68];
        pend = &cw96_active[14 * 68];
        p = cw96_active;
        do {
            if (p == psel) {
                p[339] = 17;
            } else {
                p[339] = 0;
            }
            p += 68;
        } while (p != pend);

        cur = now_icon;
        if (cur != -1) {
            class_w[119].disp_flag |= 0x80;
            class_w[119].x.half[1] = (s16)((cur / 7) * 31 - 130);
            class_w[119].y.half[1] = (s16)((cur % 7) * 16 - 11);
        }
        name_num_disp(album_chr[k * 3840 + 517], 48, -96);
        name_num_disp(album_chr[album_slot * 3840 + 518], 80, -96);
        name_num_disp(album_chr[album_slot * 3840 + 519], 112, -96);
        name_num_disp(count, 90, -29);
    }
}
