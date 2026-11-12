typedef unsigned int u32;
typedef signed short s16;
typedef unsigned char u8;

typedef struct {
    u8 pad0[0x10];
    s16 frame0_x;
    s16 frame0_y;
    u8 pad1[4];
    s16 frame1_x;
    s16 frame1_y;
    u8 pad2[4];
    u32 state;
    s16 out_x;
    s16 out_y;
    u8 pad3[4];
    u32 flag;
} AnimState;

void strSync(AnimState *obj, u32 unused) {
    register u32 counter asm("v0");
    s16 x, y;

    counter = 0x800000;

    if (obj->flag != 0) {
        obj->flag = 0;
        return;
    }

    do {
        counter = counter - 1;
        if (counter == 0) {
            obj->flag = 1;

            if (obj->state == 0) {
                obj->state = 1;
            } else {
                obj->state = 0;
            }

            {
                u8 *base = (u8*)obj + obj->state * 8 + 0x10;
                x = *(s16*)(base);
                y = *(s16*)(base + 2);
            }
            obj->out_x = x;
            obj->out_y = y;
        }
    } while (obj->flag == 0);

    obj->flag = 0;
}
