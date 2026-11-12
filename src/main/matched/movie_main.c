/* Address: 0x80056F18 */
/* Size: 1288 bytes (322 instructions) */
/* Region: Game (0x80041000-0x80085FFF), IDO 5.2, stock pic0 */
/* @SymbolSize: 1288 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

typedef struct {
    s16 x, y, w, h;
} MvRect;

typedef struct {
    MvRect disp;
    MvRect screen;
    u8 isinter;
    u8 isrgb24;
    u8 pad0, pad1;
} MvDispEnv;

typedef struct {
    u8 data[92];
} MvDrawEnv;

typedef struct {
    u8 pos[4];
    u32 size;
    char name[16];
} MvCdFile;

typedef struct {
    u32 *vlcbuf[2];
    s32 vlcid;
    u16 *imgbuf;
    MvRect rect[2];
    s32 rectid;
    MvRect slice;
    s32 isdone;
} MvDec;

typedef struct {
    u8 loc[4];
    MvDec dec;
    s32 field_52;
    s32 pad56;
    s32 width;
    s32 height;
} MvState;

extern s32 movie_end_frame;
extern s32 movie_main_tbl0;
extern s32 movie_main_tbl1;
extern s32 activeBuff;
extern MvState *movie_state;
extern s32 vblnk_counter;
extern s32 frame_counter;
extern s32 pad_trig;
extern u8 ot_work[][20];

extern void ResetGraph(s32);
extern void GsSwapDispBuff(void);
extern void ot_buff_set(void);
extern s32 CdSearchFile(MvCdFile *, char *);
extern void CdIntToPos(s32, u8 *);
extern void strSetDefDecEnv(MvDec *, s32, s32, s32, s32);
extern void strInit(MvState *, void (*)(void));
extern void strCallback(void);
extern void strNextVlc(MvDec *);
extern void SsSetSerialAttr(s32, s32, s32);
extern void SsSetSerialVol(s32, s16, s16);
extern void SetDefDispEnv(MvDispEnv *, s32, s32, s32, s32);
extern void SetDefDrawEnv(MvDrawEnv *, s32, s32, s32, s32);
extern void PutDispEnv(MvDispEnv *);
extern void PutDrawEnv(MvDrawEnv *);
extern void DecDCTin(u32 *, s32);
extern void DecDCTout(u16 *, s32);
extern void song_set(u32);
extern void sprite_slot_disp(void);
extern void disp_string(void);
extern void GsDrawOt(u8 *);
extern void strSync(MvDec *, s32);
extern void VSync(s32);
extern void SetDispMask(s32);
extern void pad_main(void);
extern void screen_clear(void);
extern void SsSetMVol(s16, s16);
extern void DrawSync(s32);

s32 movie_main(char *name, s32 end_frame, s32 sector)
{
    s32 pad_top[2];
    MvDispEnv disp;
    MvDrawEnv draw;
    u8 pad_mid[28];
    MvCdFile file;
    s32 frame;
    s32 start;
    s32 tries;
    s32 idx;
    s32 limit;
    MvRect *r;
    s16 vol;

    movie_end_frame = end_frame;
    ResetGraph(1);
    if (activeBuff == 0) {
        GsSwapDispBuff();
        ot_buff_set();
    }
    movie_main_tbl0 = 0;
    movie_state = (MvState *)0x80180000;
    movie_state->width = 320;
    movie_state->height = 240;
    frame = 0;
    tries = 0;
    if (sector == 0) {
        while (CdSearchFile(&file, name) == 0) {
            if (tries++ >= 0x10000) {
                return 0;
            }
        }
        movie_state->loc[0] = file.pos[0];
        movie_state->loc[1] = file.pos[1];
        movie_state->loc[2] = file.pos[2];
    } else {
        CdIntToPos(sector, movie_state->loc);
    }
    strSetDefDecEnv(&movie_state->dec, 0, 0, 0, 240);
    strInit(movie_state, strCallback);
    strNextVlc(&movie_state->dec);
    SsSetSerialAttr(0, 0, 1);
    SsSetSerialVol(0, 0x7FFF, 0x7FFF);
    SetDefDispEnv(&disp, movie_state->dec.rect[1].x, movie_state->dec.rect[1].y,
                  movie_state->dec.rect[1].w, movie_state->dec.rect[1].h);
    SetDefDrawEnv(&draw, movie_state->dec.rect[1].x, movie_state->dec.rect[1].y,
                  movie_state->dec.rect[1].w, movie_state->dec.rect[1].h);
    if (movie_main_tbl1 & 1) {
        disp.isrgb24 = 1;
        disp.disp.w = disp.disp.w * 2 / 3;
    }
    PutDispEnv(&disp);
    PutDrawEnv(&draw);
    start = vblnk_counter;
    limit = end_frame - 3;
    do {
        DecDCTin(movie_state->dec.vlcbuf[movie_state->dec.vlcid],
                 (movie_main_tbl1 & 1) + 2);
        DecDCTout(movie_state->dec.imgbuf,
                  movie_state->dec.slice.w * movie_state->dec.slice.h / 2);
        strNextVlc(&movie_state->dec);
        if (!(movie_main_tbl1 & 1)) {
            ot_buff_set();
            song_set((u32)(vblnk_counter - start) / 6);
            sprite_slot_disp();
            disp_string();
            GsSwapDispBuff();
            GsDrawOt(ot_work[activeBuff]);
        }
        strSync(&movie_state->dec, 0);
        VSync(0);
        idx = movie_state->dec.rectid ? 0 : 1;
        r = &movie_state->dec.rect[idx];
        SetDefDispEnv(&disp, r->x, r->y, r->w, r->h);
        r = &movie_state->dec.rect[idx];
        SetDefDrawEnv(&draw, r->x, r->y, r->w, r->h);
        if (movie_main_tbl1 & 1) {
            disp.isrgb24 = 1;
            disp.disp.w = disp.disp.w * 2 / 3;
        }
        PutDispEnv(&disp);
        PutDrawEnv(&draw);
        SetDispMask(1);
        if (movie_main_tbl0 >= limit) {
            break;
        }
        if (frame >= end_frame - 15) {
            break;
        }
        frame_counter++;
        frame++;
        if (movie_state->field_52 == 1) {
            break;
        }
        pad_main();
    } while (!(pad_trig & 0x860));
    disp.isrgb24 = 0;
    PutDispEnv(&disp);
    SetDispMask(0);
    screen_clear();
    vol = 127;
    do {
        SsSetMVol(vol, vol);
        SsSetSerialVol(0, vol, vol);
        vol -= 10;
    } while (vol > 0);
    SetDispMask(1);
    DrawSync(0);
}
