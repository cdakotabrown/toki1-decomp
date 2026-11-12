/* name_ent_param_set - NAME_ENT overlay, Address: 0x8013E898, Size: 240 bytes (60 insns) */
#include "types.h"

extern u8 name_ent_keep_sei[];
extern u8 name_ent_keep_mei[];
extern u8 name_ent_keep_adana[];
extern u8 p_sei[];
extern u8 p_mei[];
extern u8 p_adana[];
extern u32 p_birthday;
extern u32 girl0_birth;
extern u8 name_ent_keep_p_mon;
extern u8 name_ent_keep_p_day;
extern u8 name_ent_keep_p_blood;
extern u8 name_ent_keep_fuji_mon;
extern u8 name_ent_keep_fuji_day;
extern u8 name_ent_keep_fuji_blood;
extern u8 name_ent_entry_mode;

extern void memcpy();

void name_ent_param_set(void)
{
    u32 v0, v1;
    s32 tmp;

    memcpy(name_ent_keep_sei, p_sei, 8);
    memcpy(name_ent_keep_mei, p_mei, 8);
    memcpy(name_ent_keep_adana, p_adana, 12);

    v0 = p_birthday;
    name_ent_keep_p_mon = (v0 & 0x0F) - 1;
    name_ent_keep_p_day = ((v0 << 23) >> 27) - 1;
    tmp = ((v0 << 21) >> 30) + 3;
    name_ent_keep_p_blood = tmp & 0x03;

    v1 = girl0_birth;
    name_ent_keep_fuji_mon = (v1 & 0x0F) - 1;
    name_ent_keep_fuji_day = ((v1 << 23) >> 27) - 1;
    tmp = ((v1 << 21) >> 30) + 3;
    name_ent_keep_fuji_blood = tmp & 0x03;

    name_ent_entry_mode = 1;
}
