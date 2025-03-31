#include <am.h>
#include <nemu.h>
#include <stdio.h>

#define SYNC_ADDR (VGACTL_ADDR + 4)

void __am_gpu_init() {
  // int i ;
  // int w = inw(VGACTL_ADDR + 2);
  // int h = inw(VGACTL_ADDR);
  // printf("GUI: width = %d, height = %d\n", w, h);
  // uint32_t *fb = (uint32_t *) (uintptr_t) FB_ADDR;
  // for (i = 0; i < w * h; ++i) {
  //   fb[i] = i;
  // }
  // outl(SYNC_ADDR, 1);
}

void __am_gpu_config(AM_GPU_CONFIG_T *cfg) {
  *cfg = (AM_GPU_CONFIG_T) {
    .present = true, .has_accel = false,
    .width = inw(VGACTL_ADDR + 2), .height = inw(VGACTL_ADDR),
    .vmemsz = inw(VGACTL_ADDR + 2) * inw(VGACTL_ADDR) * sizeof(uint32_t)
  };
}

void __am_gpu_fbdraw(AM_GPU_FBDRAW_T *ctl) {
  int gui_w = inw(VGACTL_ADDR + 2);
  int x = ctl->x, y = ctl->y, w = ctl->w, h = ctl->h;
  uint32_t *fb = (uint32_t *) (uintptr_t) FB_ADDR;
  uint32_t *pixels = (uint32_t *) ctl->pixels;
  for (int j = 0; j < h; ++j) {
    for (int i = 0; i < w; ++i) {
      fb[(y + j) * gui_w + x + i] = pixels[j * w + i];
    }
  }
  if (ctl->sync) {
    outl(SYNC_ADDR, 1);
  }
}

void __am_gpu_status(AM_GPU_STATUS_T *status) {
  status->ready = true;
}
