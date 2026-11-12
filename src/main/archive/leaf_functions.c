typedef unsigned int u32;
typedef signed int s32;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned char u8;
typedef signed char s8;

extern void SD_CmdPut(u16 arg);

int device_cmd_send(u32 arg) {
    SD_CmdPut((u16)arg);
    return 1;
}

int k_sub_reset(int arg0) {
    return arg0 + 1;
}
