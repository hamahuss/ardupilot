#pragma once

#include <AP_Math/AP_Math.h>

class AP_Terrain;

class Location_Class : public Location
{
public:

    /// enumeration of possible altitude types
    enum ALT_FRAME {
        ALT_FRAME_ABSOLUTE = 0,
        ALT_FRAME_ABOVE_HOME = 1,
        ALT_FRAME_ABOVE_ORIGIN = 2,
        ALT_FRAME_ABOVE_TERRAIN = 3
    };

    /// constructors
    Location_Class();
    Location_Class(int32_t latitude, int32_t longitude, int32_t alt_in_cm, ALT_FRAME frame);
    Location_Class(const Location& loc);
    Location_Class(const Vector3f &ekf_offset_neu);

    static void set_terrain(AP_Terrain* terrain) { _terrain = terrain; }

    // operators
    Location_Class& operator=(const struct Location &loc);

    // set altitude
    void set_alt_cm(int32_t alt_cm, ALT_FRAME frame);

    // get altitude (in cm) in the desired frame
    // returns false on failure to get altitude in the desired frame which
    // can only happen if the original frame or desired frame is above-terrain
<<<<<<< HEAD
<<<<<<< HEAD
    bool get_alt_cm(ALT_FRAME desired_frame, int32_t &ret_alt_cm) const;
=======
    bool get_alt_cm(AltFrame desired_frame, int32_t &ret_alt_cm) const WARN_IF_UNUSED;
>>>>>>> upstream/master
=======
    bool get_alt_cm(AltFrame desired_frame, int32_t &ret_alt_cm) const WARN_IF_UNUSED;
>>>>>>> upstream/master

    // get altitude frame
    ALT_FRAME get_alt_frame() const;

    // converts altitude to new frame
    // returns false on failure to convert which can only happen if
    // the original frame or desired frame is above-terrain
    bool change_alt_frame(ALT_FRAME desired_frame);

    // get position as a vector from origin (x,y only or x,y,z)
    // return false on failure to get the vector which can only
    // happen if the EKF origin has not been set yet
    // x, y and z are in centimetres
    bool get_vector_xy_from_origin_NE(Vector2f &vec_ne) const WARN_IF_UNUSED;
    bool get_vector_from_origin_NEU(Vector3f &vec_neu) const WARN_IF_UNUSED;

    // return distance in meters between two locations
    float get_distance(const struct Location &loc2) const;

    // return the distance in meters in North/East/Down plane as a N/E/D vector to loc2
    Vector3f get_distance_NED(const Location &loc2) const;

    // return the distance in meters in North/East plane as a N/E vector to loc2
    Vector2f get_distance_NE(const Location &loc2) const;

    // extrapolate latitude/longitude given distances (in meters) north and east
    void offset(float ofs_north, float ofs_east);

<<<<<<< HEAD
<<<<<<< HEAD
    bool is_zero(void) { return (lat == 0 && lng == 0 && alt == 0 && options == 0); }
=======
=======
>>>>>>> upstream/master
    // extrapolate latitude/longitude given bearing and distance
    void offset_bearing(float bearing, float distance);

    // longitude_scale - returns the scaler to compensate for
    // shrinking longitude as you move north or south from the equator
    // Note: this does not include the scaling to convert
    // longitude/latitude points to meters or centimeters
    float longitude_scale() const;

    bool is_zero(void) const WARN_IF_UNUSED;
<<<<<<< HEAD
>>>>>>> upstream/master
=======
>>>>>>> upstream/master

    void zero(void) { lat = lng = alt = 0; options = 0; }

    // return bearing in centi-degrees from location to loc2
    int32_t get_bearing_to(const struct Location &loc2) const;

    // check if lat and lng match. Ignore altitude and options
    bool same_latlon_as(const Location &loc2) const;

    /*
     * convert invalid waypoint with useful data. return true if location changed
     */
    bool sanitize(const struct Location &defaultLoc);

    // return true when lat and lng are within range
    bool check_latlng() const;

    // see if location is past a line perpendicular to
    // the line between point1 and point2 and passing through point2.
    // If point1 is our previous waypoint and point2 is our target waypoint
    // then this function returns true if we have flown past
    // the target waypoint
    bool past_interval_finish_line(const Location &point1, const Location &point2) const;

    /*
      return the proportion we are along the path from point1 to
      point2, along a line parallel to point1<->point2.
      This will be more than 1 if we have passed point2
     */
    float line_path_proportion(const Location &point1, const Location &point2) const;

    bool initialised() const { return (lat !=0 || lng != 0); }

    // return bearing in centi-degrees from location to loc2
    int32_t get_bearing_to(const struct Location &loc2) const;

    // check if lat and lng match. Ignore altitude and options
    bool same_latlon_as(const Location &loc2) const;

    /*
     * convert invalid waypoint with useful data. return true if location changed
     */
    bool sanitize(const struct Location &defaultLoc);

    // return true when lat and lng are within range
    bool check_latlng() const;

    // see if location is past a line perpendicular to
    // the line between point1 and point2 and passing through point2.
    // If point1 is our previous waypoint and point2 is our target waypoint
    // then this function returns true if we have flown past
    // the target waypoint
    bool past_interval_finish_line(const Location &point1, const Location &point2) const;

    /*
      return the proportion we are along the path from point1 to
      point2, along a line parallel to point1<->point2.
      This will be more than 1 if we have passed point2
     */
    float line_path_proportion(const Location &point1, const Location &point2) const;

    bool initialised() const { return (lat !=0 || lng != 0); }

private:
    static AP_Terrain *_terrain;

    // scaling factor from 1e-7 degrees to meters at equator
    // == 1.0e-7 * DEG_TO_RAD * RADIUS_OF_EARTH
    static constexpr float LOCATION_SCALING_FACTOR = 0.011131884502145034f;
    // inverse of LOCATION_SCALING_FACTOR
    static constexpr float LOCATION_SCALING_FACTOR_INV = 89.83204953368922f;
};
