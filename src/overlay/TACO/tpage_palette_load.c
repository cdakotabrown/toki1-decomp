/* Address: 0x801342DC */
/* Size: 388 bytes (97 insns) */
#include "types.h"

extern void func_8004863C(void);
extern void load_image_rect(s32, s32, s32, s32, s32);
extern void load_tpage_buf_lock(s32, s32, s32);
extern void palette_upload_enqueue(s32, s32, s32, s32, s32);
extern void DrawSync(s32);
void tpage_palette_load(void)
{
  s32 i;
  s32 *p;
  func_8004863C();
  load_image_rect(864, 384, 32, 128, 0x80180000);
  load_tpage_buf_lock(63, 0x80180000, 2);
  load_tpage_buf_lock(61, 0x80182000, 2);
  load_tpage_buf_lock(29, 0x80184000, 2);
  load_tpage_buf_lock(28, 0x80186000, 2);
  load_tpage_buf_lock(60, 0x80188000, 2);
  palette_upload_enqueue(0x8018A000, 15, 1, 1, 0);
  palette_upload_enqueue(0x8018A000, 0, 1, 1, 0);
  load_image_rect(864, 256, 32, 128, 0x8018A200);
  palette_upload_enqueue(0x8018A000, 1, 1, 3, 0);
  load_tpage_buf_lock(31, 0x8018C200, 1);
  p = (s32 *) 0x80190200;
  for (i = 0; i < 16; i++)
  {
    p[128] &= 0x7FFF0000;
  }

  load_image_rect(256, 504, 256, 8, (s32) p);
  DrawSync(0);
}
