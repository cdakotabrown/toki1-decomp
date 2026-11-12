#ifndef TOKIMEKI_LIBGPU_H
#define TOKIMEKI_LIBGPU_H

typedef unsigned char u_char;
typedef unsigned short u_short;
typedef unsigned long u_long;
typedef signed char s_char;
typedef signed short s_short;
typedef signed long s_long;

typedef struct {
    short x, y, w, h;
} RECT;

typedef struct {
    u_long tag;
    u_long code[15];
} DR_ENV;

typedef struct {
    RECT clip;
    short ofs[2];
    RECT tw;
    u_short tpage;
    u_char dtd;
    u_char dfe;
    u_char isbg;
    u_char r0, g0, b0;
    DR_ENV dr_env;
    int _tokimeki_pad;
} DRAWENV;

#ifdef __STDC_VERSION__
#if __STDC_VERSION__ >= 201112L
_Static_assert(sizeof(DRAWENV) == 96, "DRAWENV must be 96 bytes for Tokimeki Memorial");
#endif
#endif

typedef struct {
    RECT disp;
    RECT screen;
    u_char isinter;
    u_char isrgb24;
    u_char pad0;
    u_char pad1;
} DISPENV;

extern void GetDrawEnv(DRAWENV *env);
extern void PutDrawEnv(DRAWENV *env);
extern void GetDispEnv(DISPENV *env);
extern void PutDispEnv(DISPENV *env);
extern void SetDefDrawEnv(DRAWENV *env, int x, int y, int w, int h);
extern void SetDefDispEnv(DISPENV *env, int x, int y, int w, int h);
extern void ClearImage(RECT *rect, u_char r, u_char g, u_char b);
extern void LoadImage(RECT *rect, u_long *data);
extern void StoreImage(RECT *rect, u_long *data);
extern void MoveImage(RECT *rect, int x, int y);
extern void DrawSync(int mode);
extern int VSync(int mode);

#endif
