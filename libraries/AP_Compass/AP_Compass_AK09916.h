/*
 * This file is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This file is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once

#include <AP_Common/AP_Common.h>
#include <AP_HAL/AP_HAL.h>
#include <AP_HAL/I2CDevice.h>
#include <AP_Math/AP_Math.h>

#include "AP_Compass.h"
#include "AP_Compass_Backend.h"

#ifndef HAL_COMPASS_AK09916_I2C_ADDR
# define HAL_COMPASS_AK09916_I2C_ADDR 0x0C
#endif

// the AK09916 can be connected via an ICM20948
#ifndef HAL_COMPASS_ICM20948_I2C_ADDR
# define HAL_COMPASS_ICM20948_I2C_ADDR 0x69
#endif

class AP_Compass_AK09916 : public AP_Compass_Backend
{
public:
<<<<<<< HEAD
    static AP_Compass_Backend *probe(Compass &compass,
                                     AP_HAL::OwnPtr<AP_HAL::I2CDevice> dev,
                                     bool force_external = false,
                                     enum Rotation rotation = ROTATION_NONE);

    // separate probe function for when behind a ICM20948 IMU
    static AP_Compass_Backend *probe_ICM20948(Compass &compass,
                                              AP_HAL::OwnPtr<AP_HAL::I2CDevice> dev,
                                              AP_HAL::OwnPtr<AP_HAL::I2CDevice> dev_icm,
                                              bool force_external = false,
                                              enum Rotation rotation = ROTATION_NONE);
    
    void read() override;
=======
    /* Probe for AK09916 standalone on I2C bus */
    static AP_Compass_Backend *probe(AP_HAL::OwnPtr<AP_HAL::I2CDevice> dev,
                                     bool force_external,
                                     enum Rotation rotation);

    /* Probe for AK09916 on auxiliary bus of ICM20948, connected through I2C */
    static AP_Compass_Backend *probe_ICM20948(AP_HAL::OwnPtr<AP_HAL::I2CDevice> dev,
                                             AP_HAL::OwnPtr<AP_HAL::I2CDevice> dev_icm,
                                             bool force_external,
                                             enum Rotation rotation);

    /* Probe for AK09916 on auxiliary bus of ICM20948, connected through SPI */
    static AP_Compass_Backend *probe_ICM20948(uint8_t mpu9250_instance,
                                             enum Rotation rotation);
>>>>>>> upstream/master

    static constexpr const char *name = "AK09916";

private:
<<<<<<< HEAD
    enum bus_type {
        AK09916_I2C=0,
        AK09916_ICM20948_I2C,
    } bus_type;
=======
    AP_Compass_AK09916(AP_AK09916_BusDriver *bus, bool force_external,
                       enum Rotation rotation);

    bool init();
    void _make_factory_sensitivity_adjustment(Vector3f &field) const;
    void _make_adc_sensitivity_adjustment(Vector3f &field) const;

    bool _reset();
    bool _setup_mode();
    bool _check_id();
    bool _calibrate();

    void _update();

    AP_AK09916_BusDriver *_bus;

    float _magnetometer_ASA[3] {0, 0, 0};
    bool _force_external;
    uint8_t _compass_instance;
    bool _initialized;
    enum Rotation _rotation;
};


class AP_AK09916_BusDriver
{
public:
    virtual ~AP_AK09916_BusDriver() { }

    virtual bool block_read(uint8_t reg, uint8_t *buf, uint32_t size) = 0;
    virtual bool register_read(uint8_t reg, uint8_t *val) = 0;
    virtual bool register_write(uint8_t reg, uint8_t val) = 0;

    virtual AP_HAL::Semaphore  *get_semaphore() = 0;

    virtual bool configure() { return true; }
    virtual bool start_measurements() { return true; }
    virtual AP_HAL::Device::PeriodicHandle register_periodic_callback(uint32_t, AP_HAL::Device::PeriodicCb) = 0;

    // set device type within a device class
    virtual void set_device_type(uint8_t devtype) = 0;

    // return 24 bit bus identifier
    virtual uint32_t get_bus_id(void) const = 0;
};

class AP_AK09916_BusDriver_HALDevice: public AP_AK09916_BusDriver
{
public:
    AP_AK09916_BusDriver_HALDevice(AP_HAL::OwnPtr<AP_HAL::I2CDevice> dev);

    virtual bool block_read(uint8_t reg, uint8_t *buf, uint32_t size) override;
    virtual bool register_read(uint8_t reg, uint8_t *val) override;
    virtual bool register_write(uint8_t reg, uint8_t val) override;

    virtual AP_HAL::Semaphore  *get_semaphore() override;
    AP_HAL::Device::PeriodicHandle register_periodic_callback(uint32_t period_usec, AP_HAL::Device::PeriodicCb) override;

    // set device type within a device class
    void set_device_type(uint8_t devtype) override {
        _dev->set_device_type(devtype);
    }

    // return 24 bit bus identifier
    uint32_t get_bus_id(void) const override {
        return _dev->get_bus_id();
    }
>>>>>>> upstream/master
    
    AP_Compass_AK09916(Compass &compass,
                       AP_HAL::OwnPtr<AP_HAL::Device> dev,
                       AP_HAL::OwnPtr<AP_HAL::Device> dev_icm,
                       bool force_external,
                       enum Rotation rotation,
                       enum bus_type bus_type);

    AP_HAL::OwnPtr<AP_HAL::Device> dev;
    AP_HAL::OwnPtr<AP_HAL::Device> dev_icm;
    
    /**
     * Device periodic callback to read data from the sensor.
     */
    bool init();
    void timer();

    uint8_t compass_instance;
    Vector3f accum;
    uint16_t accum_count;
    bool force_external;
    enum Rotation rotation;
};
