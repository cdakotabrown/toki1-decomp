/* Function: sprite_slot_disp + func_8004863C */
/* Address: 0x80047A00 */
/* Size: 3280 bytes (820 instructions) -- 783 for sprite_slot_disp, 36 for the */
/* Region: Game (0x80041000-0x80085FFF) */
/* @CompileFlags: -Wc,-pic1 -cckr */
/* @CompileEnv: IDO52_FORCE_WEB_SPILL=0.1 IDO52_FORCE_WEB_COLOR=11:v1,45:a1,56:t0,138:s8,120:s7,79:s6,75:s5 IDO52_UGEN_TEMP_REG=29:t2,31:t5,32:t6,33:t7,34:t8,35:t9,36:t2,37:t3,84:t4,85:t5,86:t6,87:t7,88:t8,89:t9,93:t7,94:t8,95:t9,96:t2,97:t3,98:t4 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: patch_insn_reg_field 0x214:rd:t2:t4 0x220:rs:t2:t3 0x220:rt:t3:t4 */
#include "types.h"

typedef struct {
    s32 base;
    s16 posX, posY;
    u16 dimX, dimY;
    s16 palette;
    u8  texU, texV;
    s16 tpage, clutv;
    u8  r, g, b, pad;
    s16 scaleX, scaleY, flipX, flipY;
    s32 timing;
} RD;

typedef struct {
    u32 flags;
} Hdr;

typedef union {
    s32 l;
    struct { u16 lo; s16 hi; } w;
} Fix;

typedef struct {
    u8 b0;
    u8 layer;
    u8 f2;
    u8 f3;
    u8 f4;
    u8 f5;
    u8 rate;
    u8 bright;
    s16 tick;
    s16 pad0a;
    u32 *cels;
    s16 **anims;
    s16 pad14;
    s16 anim;
    s16 frame;
    s16 sx;
    s16 sy;
    s16 pad1e;
    s32 timing;
    Fix x;
    Fix y;
    s32 vx;
    s32 vy;
    Hdr *hdr;
    s32 base;
    s32 pad3c;
    u8 r, g, b;
    u8 clut;
} Slot;

typedef struct {
    s16 ox, oy, cel, tp, attr, w, h;
} Cel;

typedef struct {
    u8 pad[4248];
    s32 count;
    u8 pad2[396];
    s32 tab[64];
} Save;

extern Save screen_w;
extern u32 frame_counter;
extern u8 now_step, now_sub_step, now_sub_sub_step, step_mode;
extern char sprite_slot_disp_str0[];
extern s32 activeBuff;
extern Slot class_w[];
extern u8 ot_work[];

extern void printf();
extern void GsSortSprite(RD *, u8 *, u16);

void sprite_slot_disp()
{
    Save *sv;
    RD rd;
    s32 fi;
    s32 ai;
    s16 *vd;
    Cel *cp;
    Cel *c;
    s16 *aB;
    s32 n;
    s32 w, h, x, y;
    s32 cnt, tv;
    s32 i;
    u16 pal;
    u8 bri;

    sv = &screen_w;
    bri = get_brightness();
    search_tpage(0x7FFF, i, 0);
    for (i = 0; i <= sv->count; i++) {
        if (class_w[i].f3 & 0x80) {
        if ((class_w[i].f3 & 0x40) != 0x40
            || frame_counter % (class_w[i].rate + 1) != 0) {
        if (!(class_w[i].f2 & 0x80)) {
        if (!(class_w[i].hdr->flags & 0x80000000)
            || !(class_w[i].cels[0] & 0x80000000)) {
            printf(sprite_slot_disp_str0, i, now_step, now_sub_step,
                          now_sub_sub_step, step_mode);
            continue;
        }
        if (class_w[i].f2 & 0x40) {
            cnt = class_w[i].frame;
            vd = (s16 *)class_w[i].cels[cnt];
        } else {
            ai = class_w[i].anim;
            fi = class_w[i].frame;
            vd = class_w[i].anims[ai];
            cnt = vd[fi * 2 + 1];
            vd = (s16 *)class_w[i].cels[cnt];
        }
        cnt = vd[0];
        w = vd[1];
        h = vd[2];
        x = class_w[i].x.w.hi;
        y = class_w[i].y.w.hi;
        if (cnt != 0) {
            cp = (Cel *)(vd + 4);
            for (n = cnt - 1; n >= 0; n--) {
                if ((class_w[i].f3 & 0x8) && n >= 4) {
                    n = 3;
                }
                c = &cp[n];
                rd.base = class_w[i].base;
                if (!(class_w[i].f3 & 0x1)) {
                    if (c->attr & 0x2) {
                        rd.posX = x + w;
                        rd.scaleX = c->w - w + c->ox;
                        rd.flipX = -4096;
                    } else {
                        rd.posX = x + w;
                        rd.scaleX = w - c->ox;
                        rd.flipX = 4096;
                    }
                    if (c->attr & 0x1) {
                        rd.posY = y + h;
                        rd.scaleY = c->h - h + c->oy;
                        rd.flipY = -4096;
                    } else {
                        rd.posY = y + h;
                        rd.scaleY = h - c->oy;
                        rd.flipY = 4096;
                    }
                } else {
                    if (c->attr & 0x2) {
                        rd.posX = x + w;
                        rd.scaleX = c->w - w + c->ox;
                        rd.flipX = 4096;
                    } else {
                        rd.posX = x + w;
                        rd.scaleX = w - c->ox;
                        rd.flipX = -4096;
                    }
                    if (c->attr & 0x1) {
                        rd.posY = y + h;
                        rd.scaleY = c->h - h + c->oy;
                        rd.flipY = 4096;
                    } else {
                        rd.posY = y + h;
                        rd.scaleY = h - c->oy;
                        rd.flipY = 4096;
                    }
                }
                if (class_w[i].f3 & 0x4) {
                    rd.posX -= w;
                    rd.posY -= h;
                }
                if (class_w[i].sx != 4096 && class_w[i].sy != 4096) {
                    rd.dimX = c->w + 1;
                    rd.dimY = c->h + 1;
                } else {
                    rd.dimX = c->w;
                    rd.dimY = c->h;
                }
                if ((class_w[i].f4 & 0x8) == 0x8) {
                    switch (class_w[i].base & 0x03000000) {
                    case 0:
                        pal = search_tpage8(c->cel, i, 2);
                        break;
                    case 0x01000000:
                        pal = search_tpage8(c->cel, i, 1);
                        break;
                    }
                    rd.palette = pal & 0x1F;
                    tv = c->cel & 0xFF;
                    rd.texU = (tv & 0xF) << 3;
                    rd.texV = ((tv >> 4) << 3) + ((pal & 0x20) << 2);
                } else {
                    switch (class_w[i].base & 0x03000000) {
                    case 0:
                        pal = search_tpage(c->cel, i, 2);
                        break;
                    case 0x01000000:
                        pal = search_tpage(c->cel, i, 1);
                        break;
                    }
                    rd.palette = pal & 0x1F;
                    tv = c->cel & 0x3F;
                    rd.texU = (tv & 0x7) << 4;
                    rd.texV = ((tv >> 3) << 4) + ((pal & 0x20) << 2);
                }
                if (class_w[i].clut != 0) {
                    rd.tpage = 0;
                    switch (class_w[i].clut & 0x60) {
                    case 0x20:
                        rd.tpage = 256;
                        break;
                    case 0x40:
                        rd.tpage = 288;
                        break;
                    default:
                        if (class_w[i].f5 & 0x80) {
                        switch (class_w[i].f5 & 0x7F) {
                        case 1:
                            rd.tpage = ((c->tp & 0xF0) >> 4) << 4;
                            break;
                        case 2:
                            rd.tpage = ((c->tp & 0xE0) >> 4) << 4;
                            break;
                        case 4:
                            rd.tpage = ((c->tp & 0xC0) >> 4) << 4;
                            break;
                        case 8:
                            rd.tpage = ((c->tp & 0x80) >> 4) << 4;
                            break;
                        case 0:
                        case 32:
                        case 64:
                            break;
                        }
                        }
                        break;
                    }
                    rd.clutv = (class_w[i].clut & 0x1F) + 480;
                } else {
                    switch (class_w[i].f5 & 0x7F) {
                    case 1:
                        rd.tpage = ((c->tp & 0xF0) >> 4) << 4;
                        break;
                    case 2:
                        rd.tpage = ((c->tp & 0xE0) >> 4) << 4;
                        break;
                    case 4:
                        rd.tpage = ((c->tp & 0xC0) >> 4) << 4;
                        break;
                    case 8:
                        rd.tpage = ((c->tp & 0x80) >> 4) << 4;
                        break;
                    case 0:
                    case 32:
                    case 64:
                    default:
                        rd.tpage = 0;
                        break;
                    }
                    if (class_w[i].f5 & 0x80) {
                        rd.clutv = ((c->tp & 0xF00) >> 8) + 496;
                    } else {
                        rd.clutv = ((c->tp & 0xF00) >> 8) + 480;
                    }
                }
                if (class_w[i].f3 & 0x20) {
                    rd.r = bri;
                    rd.g = bri;
                    rd.b = bri;
                } else if (class_w[i].r || class_w[i].g || class_w[i].b) {
                    rd.r = class_w[i].r;
                    rd.g = class_w[i].g;
                    rd.b = class_w[i].b;
                } else {
                    rd.r = class_w[i].bright;
                    rd.g = class_w[i].bright;
                    rd.b = class_w[i].bright;
                }
                rd.timing = class_w[i].timing;
                if (class_w[i].f3 & 0x8) {
                    if (rd.timing != 0 || rd.flipX != 4096 || rd.flipY != 4096) {
                        rd.dimX--;
                        rd.dimY--;
                    }
                }
                if (class_w[i].sx != 4096 || class_w[i].sy != 4096) {
                    rd.flipX = class_w[i].sx * (rd.flipX / 4096);
                    rd.flipY = class_w[i].sy * (rd.flipY / 4096);
                }
                if (class_w[i].layer == 0) {
                    GsSortSprite(&rd, &ot_work[activeBuff * 20], 0);
                } else {
                    GsSortSprite(&rd, &ot_work[activeBuff * 20], class_w[i].layer - 1);
                }
            }
        }
        if (!(class_w[i].f2 & 0x40) && (class_w[i].f2 & 0x1) == 1) {
            if ((class_w[i].rate & 0x80) && (class_w[i].rate & 0x7F)) {
                class_w[i].tick += class_w[i].rate & 0x7F;
            } else if (class_w[i].rate != 0) {
                if (frame_counter % class_w[i].rate == 0) {
                    class_w[i].tick++;
                }
            } else {
            }
            aB = class_w[i].anims[ai];
            if (class_w[i].tick >= aB[fi * 2 + 2]) {
                if ((class_w[i].f2 & 0x10) == 0x10) {
                    class_w[i].frame++;
                    class_w[i].tick = 0;
                    class_w[i].f2 &= ~0x20;
                } else if ((class_w[i].f2 & 0x20) == 0x20) {
                    class_w[i].frame--;
                    class_w[i].tick = 0;
                    if (class_w[i].frame <= 0) {
                        class_w[i].f2 &= 0xFFDE;
                    }
                } else {
                    class_w[i].frame++;
                    class_w[i].tick = 0;
                }
            }
            if (class_w[i].frame >= aB[0]) {
                if ((class_w[i].f2 & 0x2) == 0x2) {
                    class_w[i].frame = 0;
                    class_w[i].f2 &= ~0x3;
                } else if ((class_w[i].f2 & 0x4) == 0x4) {
                    class_w[i].f2 &= 0xFFFA;
                    class_w[i].frame--;
                } else if ((class_w[i].f2 & 0x8) == 0x8) {
                    class_w[i].frame = 0;
                    class_w[i].f2 &= 0xFFFC;
                    class_w[i].f3 &= ~0x80;
                } else if ((class_w[i].f2 & 0x10) == 0x10) {
                    class_w[i].f2 &= ~0x10;
                    class_w[i].f2 |= 0x20;
                    if (class_w[i].f3 & 0x8) {
                        if (aB[0] >= 3) {
                            class_w[i].frame -= 3;
                        } else {
                            class_w[i].frame = 0;
                        }
                    } else {
                        if (aB[0] >= 2) {
                            class_w[i].frame -= 1;
                        } else {
                            class_w[i].frame = 0;
                        }
                    }
                } else {
                    class_w[i].frame = 0;
                }
            }
        }
        if (class_w[i].f3 & 0x10) {
            if (class_w[i].vx != 0) {
                class_w[i].x.l += class_w[i].vx;
            }
            if (class_w[i].vy != 0) {
                class_w[i].y.l += class_w[i].vy;
            }
        }
        }
        }
        }
    }
}

void func_8004863C()
{
    s32 i;

    for (i = 0; i < 64; i++) {
        if ((i & 0xF) < 5) {
            screen_w.tab[i] = 0;
        } else {
            screen_w.tab[i] = -1;
        }
    }
}
