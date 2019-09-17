/// @file	RC_Channel.h
/// @brief	RC_Channel manager, with EEPROM-backed storage of constants.
#pragma once

#include <AP_Common/AP_Common.h>
#include <AP_Param/AP_Param.h>

#define RC_CHANNEL_TYPE_ANGLE       0
#define RC_CHANNEL_TYPE_RANGE       1

#define NUM_RC_CHANNELS 16

/// @class	RC_Channel
/// @brief	Object managing one RC channel
class RC_Channel {
public:
    friend class SRV_Channels;
    friend class RC_Channels;
    // Constructor
    RC_Channel(void);

<<<<<<< HEAD
    // used to get min/max/trim limit value based on _reverse
    enum LimitValue {
        RC_CHANNEL_LIMIT_TRIM,
        RC_CHANNEL_LIMIT_MIN,
        RC_CHANNEL_LIMIT_MAX
=======
    enum ChannelType {
        RC_CHANNEL_TYPE_ANGLE = 0,
        RC_CHANNEL_TYPE_RANGE = 1,
>>>>>>> upstream/master
    };

    // setup the control preferences
    void        set_range(uint16_t high);
    void        set_angle(uint16_t angle);
    bool        get_reverse(void) const;
    void        set_default_dead_zone(int16_t dzone);
    uint16_t    get_dead_zone(void) const { return dead_zone; }
    
    // get the center stick position expressed as a control_in value
    int16_t     get_control_mid() const;

    // read input from hal.rcin - create a control_in value
    void        set_pwm(int16_t pwm);
    void        recompute_pwm_no_deadzone();

    // calculate an angle given dead_zone and trim. This is used by the quadplane code
    // for hover throttle
    int16_t     pwm_to_angle_dz_trim(uint16_t dead_zone, uint16_t trim);

    /*
      return a normalised input for a channel, in range -1 to 1,
      centered around the channel trim. Ignore deadzone.
     */
    float       norm_input();

    /*
      return a normalised input for a channel, in range -1 to 1,
      centered around the channel trim. Take into account the deadzone
    */
    float       norm_input_dz();

    uint8_t     percent_input();
    int16_t     pwm_to_range();
    int16_t     pwm_to_range_dz(uint16_t dead_zone);

    // read the input value from hal.rcin for this channel
    uint16_t    read() const;

    static const struct AP_Param::GroupInfo var_info[];

    // return true if input is within deadzone of trim
    bool       in_trim_dz();

    int16_t    get_radio_in() const { return radio_in;}
    void       set_radio_in(int16_t val) {radio_in = val;}

    int16_t    get_control_in() const { return control_in;}
    void       set_control_in(int16_t val) { control_in = val;}

    void       clear_override();
    void       set_override(const uint16_t v, const uint32_t timestamp_us=0);
    bool       has_override() const;

    int16_t    stick_mixing(const int16_t servo_in);

    // get control input with zero deadzone
    int16_t     get_control_in_zero_dz(void);
    
    int16_t    get_radio_min() const {return radio_min.get();}
    void       set_radio_min(int16_t val) { radio_min = val;}

    int16_t    get_radio_max() const {return radio_max.get();}
    void       set_radio_max(int16_t val) {radio_max = val;}

    int16_t    get_radio_trim() const { return radio_trim.get();}
    void       set_radio_trim(int16_t val) { radio_trim.set(val);}
    void       save_radio_trim() { radio_trim.save();}

    void       set_and_save_trim() { radio_trim.set_and_save_ifchanged(radio_in);}

    // set and save trim if changed
    void       set_and_save_radio_trim(int16_t val) { radio_trim.set_and_save_ifchanged(val);}
<<<<<<< HEAD
    
    bool min_max_configured() const;
    
=======

    ChannelType get_type(void) const { return type_in; }

    AP_Int16    option; // e.g. activate EPM gripper / enable fence

    // auxillary switch support:
    void init_aux();
    bool read_aux();

    // Aux Switch enumeration
    enum class AUX_FUNC {
        DO_NOTHING =           0, // aux switch disabled
        FLIP =                 2, // flip
        SIMPLE_MODE =          3, // change to simple mode
        RTL =                  4, // change to RTL flight mode
        SAVE_TRIM =            5, // save current position as level
        SAVE_WP =              7, // save mission waypoint or RTL if in auto mode
        CAMERA_TRIGGER =       9, // trigger camera servo or relay
        RANGEFINDER =         10, // allow enabling or disabling rangefinder in flight which helps avoid surface tracking when you are far above the ground
        FENCE =               11, // allow enabling or disabling fence in flight
        RESETTOARMEDYAW =     12, // UNUSED
        SUPERSIMPLE_MODE =    13, // change to simple mode in middle, super simple at top
        ACRO_TRAINER =        14, // low = disabled, middle = leveled, high = leveled and limited
        SPRAYER =             15, // enable/disable the crop sprayer
        AUTO =                16, // change to auto flight mode
        AUTOTUNE =            17, // auto tune
        LAND =                18, // change to LAND flight mode
        GRIPPER =             19, // Operate cargo grippers low=off, middle=neutral, high=on
        PARACHUTE_ENABLE  =   21, // Parachute enable/disable
        PARACHUTE_RELEASE =   22, // Parachute release
        PARACHUTE_3POS =      23, // Parachute disable, enable, release with 3 position switch
        MISSION_RESET =       24, // Reset auto mission to start from first command
        ATTCON_FEEDFWD =      25, // enable/disable the roll and pitch rate feed forward
        ATTCON_ACCEL_LIM =    26, // enable/disable the roll, pitch and yaw accel limiting
        RETRACT_MOUNT =       27, // Retract Mount
        RELAY =               28, // Relay pin on/off (only supports first relay)
        LANDING_GEAR =        29, // Landing gear controller
        LOST_VEHICLE_SOUND =  30, // Play lost vehicle sound
        MOTOR_ESTOP =         31, // Emergency Stop Switch
        MOTOR_INTERLOCK =     32, // Motor On/Off switch
        BRAKE =               33, // Brake flight mode
        RELAY2 =              34, // Relay2 pin on/off
        RELAY3 =              35, // Relay3 pin on/off
        RELAY4 =              36, // Relay4 pin on/off
        THROW =               37, // change to THROW flight mode
        AVOID_ADSB =          38, // enable AP_Avoidance library
        PRECISION_LOITER =    39, // enable precision loiter
        AVOID_PROXIMITY =     40, // enable object avoidance using proximity sensors (ie. horizontal lidar)
        ARMDISARM =           41, // arm or disarm vehicle
        SMART_RTL =           42, // change to SmartRTL flight mode
        INVERTED  =           43, // enable inverted flight
        WINCH_ENABLE =        44, // winch enable/disable
        WINCH_CONTROL =       45, // winch control
        RC_OVERRIDE_ENABLE =  46, // enable RC Override
        USER_FUNC1 =          47, // user function #1
        USER_FUNC2 =          48, // user function #2
        USER_FUNC3 =          49, // user function #3
        LEARN_CRUISE =        50, // learn cruise throttle (Rover)
        MANUAL       =        51, // manual mode
        ACRO         =        52, // acro mode
        STEERING     =        53, // steering mode
        HOLD         =        54, // hold mode
        GUIDED       =        55, // guided mode
        LOITER       =        56, // loiter mode
        FOLLOW       =        57, // follow mode
        CLEAR_WP     =        58, // clear waypoints
        SIMPLE       =        59, // simple mode
        ZIGZAG       =        60, // zigzag mode
        ZIGZAG_SaveWP =       61, // zigzag save waypoint
        COMPASS_LEARN =       62, // learn compass offsets
        SAILBOAT_TACK =       63, // rover sailboat tack
        REVERSE_THROTTLE =    64, // reverse throttle input
        GPS_DISABLE  =        65, // disable GPS for testing
        RELAY5 =              66, // Relay5 pin on/off
        RELAY6 =              67, // Relay6 pin on/off
        STABILIZE =           68, // stabilize mode
        POSHOLD   =           69, // poshold mode
        ALTHOLD   =           70, // althold mode
        FLOWHOLD  =           71, // flowhold mode
        CIRCLE    =           72, // circle mode
        DRIFT     =           73  // drift mode
        // if you add something here, make sure to update the documentation of the parameter in RC_Channel.cpp!
        // also, if you add an option >255, you will need to fix duplicate_options_exist
    };
    typedef enum AUX_FUNC aux_func_t;

protected:

    // auxillary switch handling (n.b.: we store this as 2-bits!):
    enum aux_switch_pos_t : uint8_t {
        LOW,       // indicates auxiliary switch is in the low position (pwm <1200)
        MIDDLE,    // indicates auxiliary switch is in the middle position (pwm >1200, <1800)
        HIGH       // indicates auxiliary switch is in the high position (pwm >1800)
    };

    virtual void init_aux_function(aux_func_t ch_option, aux_switch_pos_t);
    virtual void do_aux_function(aux_func_t ch_option, aux_switch_pos_t);

    void do_aux_function_avoid_proximity(const aux_switch_pos_t ch_flag);
    void do_aux_function_camera_trigger(const aux_switch_pos_t ch_flag);
    void do_aux_function_clear_wp(const aux_switch_pos_t ch_flag);
    void do_aux_function_gripper(const aux_switch_pos_t ch_flag);
    void do_aux_function_lost_vehicle_sound(const aux_switch_pos_t ch_flag);
    void do_aux_function_rc_override_enable(const aux_switch_pos_t ch_flag);
    void do_aux_function_relay(uint8_t relay, bool val);
    void do_aux_function_sprayer(const aux_switch_pos_t ch_flag);

    typedef int8_t modeswitch_pos_t;
    virtual void mode_switch_changed(modeswitch_pos_t new_pos) {
        // no action by default (e.g. Tracker, Sub, who do their own thing)
    };


>>>>>>> upstream/master
private:

    // pwm is stored here
    int16_t     radio_in;

    // value generated from PWM normalised to configured scale
    int16_t    control_in;
    
    AP_Int16    radio_min;
    AP_Int16    radio_trim;
    AP_Int16    radio_max;

    AP_Int8     reversed;
    AP_Int16    dead_zone;

    uint8_t     type_in;
    int16_t     high_in;

    // the input channel this corresponds to
    uint8_t     ch_in;

    // overrides
    uint16_t override_value;
    uint32_t last_override_time;

    // bits set when channel has been identified as configured
    static uint32_t configured_mask;

    int16_t pwm_to_angle();
    int16_t pwm_to_angle_dz(uint16_t dead_zone);
};


/*
  class	RC_Channels. Hold the full set of RC_Channel objects
*/
class RC_Channels {
public:
    friend class SRV_Channels;
    friend class RC_Channel;
    // constructor
    RC_Channels(void);

    static const struct AP_Param::GroupInfo var_info[];

    static RC_Channel *rc_channel(uint8_t chan) {
        return (chan < NUM_RC_CHANNELS)?&channels[chan]:nullptr;
    }

    static uint16_t get_radio_in(const uint8_t chan); // returns the last read radio_in value from a chan, 0 if the channel is out of range
    static uint8_t get_radio_in(uint16_t *chans, const uint8_t num_channels); // reads a block of chanel radio_in values starting from channel 0
                                                                              // returns the number of valid channels

    static uint8_t get_valid_channel_count(void);                      // returns the number of valid channels in the last read
    static int16_t get_receiver_rssi(void);                            // returns [0, 255] for receiver RSSI (0 is no link) if present, otherwise -1
    static bool read_input(void);                                      // returns true if new input has been read in
    static void clear_overrides(void);                                 // clears any active overrides
    static bool receiver_bind(const int dsmMode);                      // puts the reciever in bind mode if present, returns true if success
    static void set_override(const uint8_t chan, const int16_t value, const uint32_t timestamp_ms = 0); // set a channels override value
    static bool has_active_overrides(void);                            // returns true if there are overrides applied that are valid

<<<<<<< HEAD
=======
    class RC_Channel *find_channel_for_option(const RC_Channel::aux_func_t option);
    bool duplicate_options_exist();

    void init_aux_all();
    void read_aux_all();

    // mode switch handling
    void reset_mode_switch();
    virtual void read_mode_switch();

    // has_valid_input should be pure-virtual when Plane is converted
    virtual bool has_valid_input() const { return false; };

    bool gcs_overrides_enabled() const { return _gcs_overrides_enabled; }
    void set_gcs_overrides_enabled(bool enable) {
        _gcs_overrides_enabled = enable;
        if (!_gcs_overrides_enabled) {
            clear_overrides();
        }
    }

    // should we ignore RC failsafe bits from receivers?
    bool ignore_rc_failsafe(void) const {
        return get_singleton() != nullptr && (_options & uint32_t(Option::IGNORE_FAILSAFE));
    }

    bool ignore_overrides() const {
        return _options & uint32_t(Option::IGNORE_OVERRIDES);
    }

    bool ignore_receiver() const {
        return _options & uint32_t(Option::IGNORE_RECEIVER);
    }

    float override_timeout_ms() const {
        return _override_timeout.get() * 1e3f;
    }

protected:

    enum class Option {
        IGNORE_RECEIVER  = (1 << 0), // RC receiver modules
        IGNORE_OVERRIDES = (1 << 1), // MAVLink overrides
        IGNORE_FAILSAFE  = (1 << 2), // ignore RC failsafe bits
    };

    void new_override_received() {
        has_new_overrides = true;
    }

>>>>>>> upstream/master
private:
    // this static arrangement is to avoid static pointers in AP_Param tables
    static RC_Channel *channels;
<<<<<<< HEAD
    static bool has_new_overrides;
    static AP_Float *override_timeout;
    RC_Channel obj_channels[NUM_RC_CHANNELS];
=======

    bool has_new_overrides;

>>>>>>> upstream/master
    AP_Float _override_timeout;
};
