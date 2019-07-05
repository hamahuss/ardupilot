#pragma once

#include <AP_HAL/AP_HAL.h>
#include "AP_HAL_SITL_Namespace.h"
#include "Semaphores.h"

class HALSITL::Util : public AP_HAL::Util {
public:
    Util(SITL_State *_sitlState) :
        sitlState(_sitlState) {}
    
    bool run_debug_shell(AP_HAL::BetterStream *stream) {
        return false;
    }

    /**
       how much free memory do we have in bytes. 
     */
    uint32_t available_memory(void) override {
        // SITL is assumed to always have plenty of memory. Return 128k for now
        return 0x20000;
    }

    // create a new semaphore
    AP_HAL::Semaphore *new_semaphore(void) override { return new HALSITL::Semaphore; }

    // get path to custom defaults file for AP_Param
    const char* get_custom_defaults_file() const override {
        return sitlState->defaults_path;
    }

    uint64_t get_hw_rtc() const override;

<<<<<<< HEAD
=======
    bool get_system_id(char buf[40]) override;
    bool get_system_id_unformatted(uint8_t buf[], uint8_t &len) override;
    void dump_stack_trace();

#ifdef ENABLE_HEAP
    // heap functions, note that a heap once alloc'd cannot be dealloc'd
    void *allocate_heap_memory(size_t size) override;
    void *heap_realloc(void *heap, void *ptr, size_t new_size) override;
#endif // ENABLE_HEAP

#ifdef WITH_SITL_TONEALARM
    bool toneAlarm_init() override { return _toneAlarm.init(); }
    void toneAlarm_set_buzzer_tone(float frequency, float volume, uint32_t duration_ms) override {
        _toneAlarm.set_buzzer_tone(frequency, volume, duration_ms);
    }
#endif

    // return true if the reason for the reboot was a watchdog reset
    bool was_watchdog_reset() const override { return getenv("SITL_WATCHDOG_RESET") != nullptr; }
    
<<<<<<< HEAD
>>>>>>> upstream/master
=======
>>>>>>> upstream/master
private:
    SITL_State *sitlState;
};
