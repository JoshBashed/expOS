#include "driver_display_ramfb.h"
#include "driver_display_ramfb_internal.h"
#include "logging.h"
#include <string.h>

void _write_fwcfg_selector(u16 selector) { *FW_SELECTOR_REGISTER = bswap16(selector); }

u64 _read_fwcfg_data() { return *FW_DATA_REGISTER; }

void _transfer_fwcfg_dma(void *address, u32 length, u32 control) {
    QemuFwCfgDmaAccess dma = {0};
    dma.control = bswap32(control);
    dma.address = bswap64((u64)address);
    dma.length = bswap32(length);

    *FW_DMA_REGISTER = bswap64((u64)&dma);

    LOG_DEBUG("driver_display_ramfb: waiting for dma completion");
    while (bswap64(dma.control) & ~QEMU_CFG_DMA_CTL_ERROR) {
    }
    LOG_DEBUG("driver_display_ramfb: dma completed");
}

void _read_fwcfg_dma_data(void *buffer, u32 selector, u32 length) {
    u32 control = (selector << 16) | QEMU_CFG_DMA_CTL_SELECT | QEMU_CFG_DMA_CTL_READ;
    _transfer_fwcfg_dma(buffer, length, control);
}

void _write_fwcfg_dma_data(void *buffer, u32 selector, u32 length) {
    u32 control = (selector << 16) | QEMU_CFG_DMA_CTL_SELECT | QEMU_CFG_DMA_CTL_WRITE;
    _transfer_fwcfg_dma(buffer, length, control);
}

bool _find_fwcfg_file(shared_StaticString name, QemuFwCfgFile *out) {
    u32 entries = 0;
    _read_fwcfg_dma_data(&entries, 0x19, sizeof(entries));
    entries = bswap32(entries);

    u64 directory_size = sizeof(QemuFwCfgFiles) + (entries * sizeof(QemuFwCfgFile));
    QemuFwCfgFiles *directory = __builtin_alloca(directory_size);
    _read_fwcfg_dma_data(directory, 0x19, directory_size);
    for (u32 i = 0; i < entries; i++) {
        QemuFwCfgFile *entry = &directory->entries[i];
        shared_StaticString entry_name = shared_StaticString_from_cstr((const char *)entry->name);
        if (!shared_StaticString_eq(entry_name, name))
            continue;

        entry->select = bswap16(entry->select);
        entry->size = bswap32(entry->size);
        *out = *entry;
        return true;
    }

    return false;
}

bool driver_display_ramfb_init(display_driver_ramfb_QemuRamFBCfg *cfg) {
    static u8 initialized = 255;
    if (initialized != 255)
        return initialized;

    // So the user of the api doesn't need to
    LOG_DEBUG("driver_display_ramfb: byte-swapping cfg");
    cfg->address = bswap64(cfg->address);
    cfg->fourcc = bswap32(cfg->fourcc);
    cfg->flags = bswap32(cfg->flags);
    cfg->width = bswap32(cfg->width);
    cfg->height = bswap32(cfg->height);
    cfg->stride = bswap32(cfg->stride);
    LOG_DEBUG("driver_display_ramfb: cfg byte-swapped");

    QemuFwCfgFile ramfb_file = {0};
    LOG_DEBUG("driver_display_ramfb: looking for etc/ramfb");
    LOG_DEBUG("driver_display_ramfb: searching for etc/ramfb");
    if (!_find_fwcfg_file(STATIC_STRING("etc/ramfb"), &ramfb_file)) {
        LOG_ERROR("driver_display_ramfb: etc/ramfb not found in fw_cfg");
        initialized = false;
        return false;
    }

    _write_fwcfg_dma_data(cfg, ramfb_file.select, sizeof(display_driver_ramfb_QemuRamFBCfg));

    initialized = true;
    return true;
}
