/* name_ent_hizuke_rand - NAME_ENT overlay, Address: 0x80137898, Size: 696 bytes */
/* @PostProc: patch_insn_reg_field 0x15c:rt:t6:v1 0x168:rt:t6:v1 0x16c:rt:t7:t6 0x170:rs:t7:t6 0x170:rt:t7:t6 0x178:rs:t7:t6 0x194:rd:t8:t7 0x198:rt:t8:t7 0x1c8:rt:t9:t8 0x1cc:rs:t9:t8 0x1d0:rt:t0:t9 0x1d4:rs:t0:t9 0x1d4:rt:t0:t9 0x1dc:rs:t0:t9 0x200:rd:t1:t0 0x204:rt:t1:t0 0x220:rt:t2:t1 0x224:rs:t2:t1 0x23c:rt:t3:t2 0x240:rs:t3:t2 0x258:rt:t4:t3 0x25c:rs:t4:t3 0x274:rt:t5:t4 0x278:rs:t5:t4 0x298:rt:t6:t5 0x2a4:rs:t6:t5 0x2ac:rt:t7:v1 0x2b4:rt:t7:v1 */
#include "types.h"

extern u8 name_ent_p_mon;
extern volatile u8 name_ent_p_day;
extern u8 name_ent_p_blood;
extern u8 name_ent_fuji_mon;
extern volatile u8 name_ent_fuji_day;
extern u8 name_ent_fuji_blood;

extern s32 rand(void);

void name_ent_hizuke_rand(s32 mode)
{
    u32 month;
    u8 day;
    s32 random;

    if (mode != 0) {
        if (name_ent_fuji_mon == 255) {
            random = rand();
            name_ent_fuji_mon = random % 12;
        }
        day = name_ent_fuji_day;
        if (day == 255) {
            random = rand();
            day = (u8)(random % 31);
        }
        if (name_ent_fuji_blood == 255) {
            name_ent_fuji_day = day;
            random = rand();
            day = name_ent_fuji_day;
            name_ent_fuji_blood = random % 3;
        }
        month = name_ent_fuji_mon + 1;
        if (month == 2) {
            if ((u32)(day + 1) >= 29) {
                day = 27;
            }
        }
        if (month == 4) {
            if ((u32)(day + 1) >= 31) {
                day = 29;
            }
        }
        if (month == 6) {
            if ((u32)(day + 1) >= 31) {
                day = 29;
            }
        }
        if (month == 9) {
            if ((u32)(day + 1) >= 31) {
                day = 29;
            }
        }
        name_ent_fuji_day = day;
        if (month == 11) {
            name_ent_fuji_day = day;
            if ((u32)(day + 1) >= 31) {
                day = 29;
                name_ent_fuji_day = day;
            }
        }
    } else {
        if (name_ent_p_mon == 255) {
            random = rand();
            name_ent_p_mon = random % 12;
        }
        day = name_ent_p_day;
        if (day == 255) {
            random = rand();
            day = (u8)(random % 31);
        }
        if (name_ent_p_blood == 255) {
            name_ent_p_day = day;
            random = rand();
            day = name_ent_p_day;
            name_ent_p_blood = random % 3;
        }
        month = name_ent_p_mon + 1;
        if (month == 2) {
            if ((u32)(day + 1) >= 29) {
                day = 27;
            }
        }
        if (month == 4) {
            if ((u32)(day + 1) >= 31) {
                day = 29;
            }
        }
        if (month == 6) {
            if ((u32)(day + 1) >= 31) {
                day = 29;
            }
        }
        if (month == 9) {
            if ((u32)(day + 1) >= 31) {
                day = 29;
            }
        }
        name_ent_p_day = day;
        if (month == 11) {
            name_ent_p_day = day;
            if ((u32)(day + 1) >= 31) {
                day = 29;
                name_ent_p_day = day;
            }
        }
    }
}
