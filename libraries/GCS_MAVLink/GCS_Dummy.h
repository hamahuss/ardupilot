#include "GCS.h"
#include <AP_Common/AP_FWVersion.h>

const AP_FWVersion AP_FWVersion::fwver
{
    major: 3,
    minor: 1,
    patch: 4,
    fw_type: FIRMWARE_VERSION_TYPE_DEV,
    fw_string: "Dummy GCS",
    fw_hash_str: "",
    middleware_name: "",
    middleware_hash_str: "",
    os_name: "",
    os_hash_str: "",
    os_sw_version: 0
};

/*
 *  GCS backend used for many examples and tools
 */
class GCS_MAVLINK_Dummy : public GCS_MAVLINK
{
public:

    using GCS_MAVLINK::GCS_MAVLINK;

private:

    uint32_t telem_delay() const override { return 0; }
    void handleMessage(const mavlink_message_t &msg) override {}
    bool try_send_message(enum ap_message id) override { return true; }
    bool handle_guided_request(AP_Mission::Mission_Command &cmd) override { return true; }
    void handle_change_alt_request(AP_Mission::Mission_Command &cmd) override {}

protected:

    Compass *get_compass() const override { return nullptr; };
    AP_Mission *get_mission() override { return nullptr; }
    AP_Rally *get_rally() const override { return nullptr; };
    AP_Camera *get_camera() const override { return nullptr; };

    uint8_t sysid_my_gcs() const override { return 1; }
    bool set_mode(uint8_t mode) override { return false; };

    // dummy information:
    MAV_TYPE frame_type() const override { return MAV_TYPE_FIXED_WING; }
    MAV_MODE base_mode() const override { return (MAV_MODE)MAV_MODE_FLAG_CUSTOM_MODE_ENABLED; }
    uint32_t custom_mode() const override { return 3; } // magic number
    MAV_STATE system_status() const override { return MAV_STATE_CALIBRATING; }

};

/*
 * a GCS singleton used for many example sketches and tools
 */

extern const AP_HAL::HAL& hal;

class GCS_Dummy : public GCS
{
public:

    using GCS::GCS;

protected:

    GCS_MAVLINK_Dummy *new_gcs_mavlink_backend(GCS_MAVLINK_Parameters &params,
                                               AP_HAL::UARTDriver &uart) override {
        return new GCS_MAVLINK_Dummy(params, uart);
    }

private:
    GCS_MAVLINK_Dummy *chan(const uint8_t ofs) override {
        if (ofs > _num_gcs) {
            AP::internalerror().error(AP_InternalError::error_t::gcs_offset);
            return nullptr;
        }
        return (GCS_MAVLINK_Dummy *)_chan[ofs];
    };
    const GCS_MAVLINK_Dummy *chan(const uint8_t ofs) const override {
        if (ofs > _num_gcs) {
            AP::internalerror().error(AP_InternalError::error_t::gcs_offset);
            return nullptr;
        }
        return (GCS_MAVLINK_Dummy *)_chan[ofs];
    };

<<<<<<< HEAD
    void send_statustext(MAV_SEVERITY severity, uint8_t dest_bitmask, const char *text) { hal.console->printf("TOGCS: %s\n", text); }
=======
    void send_statustext(MAV_SEVERITY severity, uint8_t dest_bitmask, const char *text) override { hal.console->printf("TOGCS: %s\n", text); }

    MAV_TYPE frame_type() const override { return MAV_TYPE_FIXED_WING; }
    uint32_t custom_mode() const override { return 3; } // magic number
>>>>>>> upstream/master
};
