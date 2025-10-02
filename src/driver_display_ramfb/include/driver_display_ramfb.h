#pragma once
#include "shared.h"

#define DISPLAY_DRIVER_RAMFB_FORMAT_XRGB32 875713112

typedef struct _display_driver_ramfb_QemuRamFBCfg {
    u64 address;
    u32 fourcc;
    u32 flags;
    u32 width;
    u32 height;
    u32 stride;
} display_driver_ramfb_QemuRamFBCfg;

bool driver_display_ramfb_init(display_driver_ramfb_QemuRamFBCfg *cfg);
