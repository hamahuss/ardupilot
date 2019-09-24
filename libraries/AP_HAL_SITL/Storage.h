#pragma once

#include <AP_HAL/AP_HAL.h>
#include "AP_HAL_SITL_Namespace.h"

class HALSITL::EEPROMStorage : public AP_HAL::Storage {
public:
    EEPROMStorage() {
        _eeprom_fd = -1;
    }
    void init() {}
    void read_block(void *dst, uint16_t src, size_t n);
    void write_block(uint16_t dst, const void* src, size_t n);

private:
<<<<<<< HEAD
    int _eeprom_fd;
    void _eeprom_open(void);
=======
    volatile bool _initialised;
    void _storage_create(void);
    void _storage_open(void);
    void _save_backup(void);
    void _mark_dirty(uint16_t loc, uint16_t length);
    uint8_t _buffer[HAL_STORAGE_SIZE] __attribute__((aligned(4)));
    Bitmask<STORAGE_NUM_LINES> _dirty_mask;

#if STORAGE_USE_FLASH
    bool _flash_write_data(uint8_t sector, uint32_t offset, const uint8_t *data, uint16_t length);
    bool _flash_read_data(uint8_t sector, uint32_t offset, uint8_t *data, uint16_t length);
    bool _flash_erase_sector(uint8_t sector);
    bool _flash_erase_ok(void);
#endif

    bool _flash_failed;
    uint32_t _last_re_init_ms;
    uint32_t _last_empty_ms;

#if STORAGE_USE_FLASH
    AP_FlashStorage _flash{_buffer,
            HAL_STORAGE_SIZE,
            FUNCTOR_BIND_MEMBER(&Storage::_flash_write_data, bool, uint8_t, uint32_t, const uint8_t *, uint16_t),
            FUNCTOR_BIND_MEMBER(&Storage::_flash_read_data, bool, uint8_t, uint32_t, uint8_t *, uint16_t),
            FUNCTOR_BIND_MEMBER(&Storage::_flash_erase_sector, bool, uint8_t),
            FUNCTOR_BIND_MEMBER(&Storage::_flash_erase_ok, bool)};
#endif
    
    void _flash_load(void);
    void _flash_write(uint16_t line);

#if STORAGE_USE_POSIX
    bool using_filesystem;
    int log_fd;
#endif
>>>>>>> upstream/master
};
