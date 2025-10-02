#pragma once

#include "shared.h"

#define FW_CFG_ADDRESS ((u8 *)0x09020000)
#define FW_SELECTOR_REGISTER ((u16 *)(FW_CFG_ADDRESS + 8))
#define FW_DATA_REGISTER ((u64 *)(FW_CFG_ADDRESS + 0))
#define FW_DMA_REGISTER ((u64 *)(FW_CFG_ADDRESS + 16))

typedef struct _QemuFwCfgDmaAccess {
    u32 control;
    u32 length;
    u64 address;
} __attribute__((packed)) QemuFwCfgDmaAccess;

typedef struct _QemuFwCfgFile {
    u32 size;
    u16 select;
    u16 reserved;
    u8 name[56];
} __attribute__((packed)) QemuFwCfgFile;

typedef struct _QemuFwCfgFiles {
    u32 count;
    QemuFwCfgFile entries[];
} __attribute__((packed)) QemuFwCfgFiles;

enum {
    QEMU_CFG_DMA_CTL_ERROR = 0x01,
    QEMU_CFG_DMA_CTL_READ = 0x02,
    QEMU_CFG_DMA_CTL_SKIP = 0x04,
    QEMU_CFG_DMA_CTL_SELECT = 0x08,
    QEMU_CFG_DMA_CTL_WRITE = 0x10,
};
