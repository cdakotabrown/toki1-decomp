#ifndef PSYQ_LIBGTE_MIN_H
#define PSYQ_LIBGTE_MIN_H

typedef struct {
    short m[3][3];
    long  t[3];
} MATRIX;

typedef struct {
    long vx, vy;
    long vz, pad;
} VECTOR;

typedef struct {
    short vx, vy;
    short vz, pad;
} SVECTOR;

extern MATRIX *RotMatrix(SVECTOR *r, MATRIX *m);
extern MATRIX *TransMatrix(MATRIX *m, VECTOR *v);
extern MATRIX *ScaleMatrix(MATRIX *m, VECTOR *v);
extern void    SetRotMatrix(MATRIX *m);
extern void    SetTransMatrix(MATRIX *m);

#endif
