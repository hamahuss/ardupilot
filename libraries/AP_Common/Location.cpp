/*
 * Location.cpp
 */

#include "Location.h"

#include <AP_AHRS/AP_AHRS.h>
#include <AP_Terrain/AP_Terrain.h>

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
extern const AP_HAL::HAL& hal;

AP_Terrain *Location_Class::_terrain = nullptr;
=======
=======
>>>>>>> upstream/master
=======
>>>>>>> upstream/master
AP_Terrain *Location::_terrain = nullptr;
>>>>>>> upstream/master

/// constructors
Location_Class::Location_Class()
{
    zero();
}

Location_Class::Location_Class(int32_t latitude, int32_t longitude, int32_t alt_in_cm, ALT_FRAME frame)
{
    lat = latitude;
    lng = longitude;
    options = 0;
    set_alt_cm(alt_in_cm, frame);
}

Location_Class::Location_Class(const Location& loc)
{
    lat = loc.lat;
    lng = loc.lng;
    alt = loc.alt;
    options = loc.options;
}

Location_Class::Location_Class(const Vector3f &ekf_offset_neu)
{
    // store alt and alt frame
    set_alt_cm(ekf_offset_neu.z, ALT_FRAME_ABOVE_ORIGIN);

    // calculate lat, lon
    Location ekf_origin;
    if (AP::ahrs().get_origin(ekf_origin)) {
        lat = ekf_origin.lat;
        lng = ekf_origin.lng;
        offset(ekf_offset_neu.x / 100.0f, ekf_offset_neu.y / 100.0f);
    }
}

Location_Class& Location_Class::operator=(const struct Location &loc)
{
    lat = loc.lat;
    lng = loc.lng;
    alt = loc.alt;
    options = loc.options;
    return *this;
}

void Location_Class::set_alt_cm(int32_t alt_cm, ALT_FRAME frame)
{
    alt = alt_cm;
    flags.relative_alt = false;
    flags.terrain_alt = false;
    flags.origin_alt = false;
    switch (frame) {
        case ALT_FRAME_ABSOLUTE:
            // do nothing
            break;
        case ALT_FRAME_ABOVE_HOME:
            flags.relative_alt = true;
            break;
        case ALT_FRAME_ABOVE_ORIGIN:
            flags.origin_alt = true;
            break;
        case ALT_FRAME_ABOVE_TERRAIN:
            // we mark it as a relative altitude, as it doesn't have
            // home alt added
            flags.relative_alt = true;
            flags.terrain_alt = true;
            break;
    }
}

// converts altitude to new frame
bool Location_Class::change_alt_frame(ALT_FRAME desired_frame)
{
    int32_t new_alt_cm;
    if (!get_alt_cm(desired_frame, new_alt_cm)) {
        return false;
    }
    set_alt_cm(new_alt_cm, desired_frame);
    return true;
}

// get altitude frame
Location_Class::ALT_FRAME Location_Class::get_alt_frame() const
{
    if (flags.terrain_alt) {
        return ALT_FRAME_ABOVE_TERRAIN;
    }
    if (flags.origin_alt) {
        return ALT_FRAME_ABOVE_ORIGIN;
    }
    if (flags.relative_alt) {
        return ALT_FRAME_ABOVE_HOME;
    }
    return ALT_FRAME_ABSOLUTE;
}

/// get altitude in desired frame
bool Location_Class::get_alt_cm(ALT_FRAME desired_frame, int32_t &ret_alt_cm) const
{
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
    Location_Class::ALT_FRAME frame = get_alt_frame();
=======
=======
>>>>>>> upstream/master
=======
>>>>>>> upstream/master
#if CONFIG_HAL_BOARD == HAL_BOARD_SITL
    if (!initialised()) {
        AP_HAL::panic("Should not be called on invalid location");
    }
#endif
    Location::AltFrame frame = get_alt_frame();
>>>>>>> upstream/master

    // shortcut if desired and underlying frame are the same
    if (desired_frame == frame) {
        ret_alt_cm = alt;
        return true;
    }

    // check for terrain altitude
    float alt_terr_cm = 0;
    if (frame == ALT_FRAME_ABOVE_TERRAIN || desired_frame == ALT_FRAME_ABOVE_TERRAIN) {
#if AP_TERRAIN_AVAILABLE
        if (_terrain == nullptr || !_terrain->height_amsl(*(Location *)this, alt_terr_cm, true)) {
            return false;
        }
        // convert terrain alt to cm
        alt_terr_cm *= 100.0f;
#else
        return false;
#endif
    }

    // convert alt to absolute
    int32_t alt_abs;
    switch (frame) {
        case ALT_FRAME_ABSOLUTE:
            alt_abs = alt;
            break;
        case ALT_FRAME_ABOVE_HOME:
            if (!AP::ahrs().home_is_set()) {
                return false;
            }
            alt_abs = alt + AP::ahrs().get_home().alt;
            break;
        case ALT_FRAME_ABOVE_ORIGIN:
            {
                // fail if we cannot get ekf origin
                Location ekf_origin;
                if (!AP::ahrs().get_origin(ekf_origin)) {
                    return false;
                }
                alt_abs = alt + ekf_origin.alt;
            }
            break;
        case ALT_FRAME_ABOVE_TERRAIN:
            alt_abs = alt + alt_terr_cm;
            break;
        default:
            // unknown conversion to absolute alt, this should never happen
            return false;
    }

    // convert absolute to desired frame
    switch (desired_frame) {
        case ALT_FRAME_ABSOLUTE:
            ret_alt_cm = alt_abs;
            return true;
        case ALT_FRAME_ABOVE_HOME:
            if (!AP::ahrs().home_is_set()) {
                return false;
            }
            ret_alt_cm = alt_abs - AP::ahrs().get_home().alt;
            return true;
        case ALT_FRAME_ABOVE_ORIGIN:
            {
                // fail if we cannot get ekf origin
                Location ekf_origin;
                if (!AP::ahrs().get_origin(ekf_origin)) {
                    return false;
                }
                ret_alt_cm = alt_abs - ekf_origin.alt;
                return true;
            }
        case ALT_FRAME_ABOVE_TERRAIN:
            ret_alt_cm = alt_abs - alt_terr_cm;
            return true;
        default:
            // should never happen
            return false;
    }
}

bool Location_Class::get_vector_xy_from_origin_NE(Vector2f &vec_ne) const
{
    Location ekf_origin;
    if (!AP::ahrs().get_origin(ekf_origin)) {
        return false;
    }
    vec_ne.x = (lat-ekf_origin.lat) * LATLON_TO_CM;
    vec_ne.y = (lng-ekf_origin.lng) * LATLON_TO_CM * longitude_scale(ekf_origin);
    return true;
}

bool Location_Class::get_vector_from_origin_NEU(Vector3f &vec_neu) const
{
    // convert lat, lon
    Vector2f vec_ne;
    if (!get_vector_xy_from_origin_NE(vec_ne)) {
        return false;
    }
    vec_neu.x = vec_ne.x;
    vec_neu.y = vec_ne.y;

    // convert altitude
    int32_t alt_above_origin_cm = 0;
    if (!get_alt_cm(ALT_FRAME_ABOVE_ORIGIN, alt_above_origin_cm)) {
        return false;
    }
    vec_neu.z = alt_above_origin_cm;

    return true;
}

// return distance in meters between two locations
float Location_Class::get_distance(const struct Location &loc2) const
{
    float dlat = (float)(loc2.lat - lat);
    float dlng = ((float)(loc2.lng - lng)) * longitude_scale(loc2);
    return norm(dlat, dlng) * LOCATION_SCALING_FACTOR;
}


/*
  return the distance in meters in North/East plane as a N/E vector
  from loc1 to loc2
 */
Vector2f Location::get_distance_NE(const Location &loc2) const
{
    return Vector2f((loc2.lat - lat) * LOCATION_SCALING_FACTOR,
                    (loc2.lng - lng) * LOCATION_SCALING_FACTOR * longitude_scale());
}

// return the distance in meters in North/East/Down plane as a N/E/D vector to loc2
Vector3f Location::get_distance_NED(const Location &loc2) const
{
    return Vector3f((loc2.lat - lat) * LOCATION_SCALING_FACTOR,
                    (loc2.lng - lng) * LOCATION_SCALING_FACTOR * longitude_scale(),
                    (alt - loc2.alt) * 0.01f);
}

// extrapolate latitude/longitude given distances (in meters) north and east
void Location_Class::offset(float ofs_north, float ofs_east)
{
    // use is_equal() because is_zero() is a local class conflict and is_zero() in AP_Math does not belong to a class
    if (!is_equal(ofs_north, 0.0f) || !is_equal(ofs_east, 0.0f)) {
        int32_t dlat = ofs_north * LOCATION_SCALING_FACTOR_INV;
        int32_t dlng = (ofs_east * LOCATION_SCALING_FACTOR_INV) / longitude_scale(*this);
        lat += dlat;
        lng += dlng;
    }
}
<<<<<<< HEAD
=======

/*
 *  extrapolate latitude/longitude given bearing and distance
 * Note that this function is accurate to about 1mm at a distance of
 * 100m. This function has the advantage that it works in relative
 * positions, so it keeps the accuracy even when dealing with small
 * distances and floating point numbers
 */
void Location::offset_bearing(float bearing, float distance)
{
    const float ofs_north = cosf(radians(bearing)) * distance;
    const float ofs_east  = sinf(radians(bearing)) * distance;
    offset(ofs_north, ofs_east);
}

/*
 *  extrapolate latitude/longitude given bearing and distance
 * Note that this function is accurate to about 1mm at a distance of
 * 100m. This function has the advantage that it works in relative
 * positions, so it keeps the accuracy even when dealing with small
 * distances and floating point numbers
 */
void Location::offset_bearing(float bearing, float distance)
{
    const float ofs_north = cosf(radians(bearing)) * distance;
    const float ofs_east  = sinf(radians(bearing)) * distance;
    offset(ofs_north, ofs_east);
}

/*
 *  extrapolate latitude/longitude given bearing and distance
 * Note that this function is accurate to about 1mm at a distance of
 * 100m. This function has the advantage that it works in relative
 * positions, so it keeps the accuracy even when dealing with small
 * distances and floating point numbers
 */
void Location::offset_bearing(float bearing, float distance)
{
    const float ofs_north = cosf(radians(bearing)) * distance;
    const float ofs_east  = sinf(radians(bearing)) * distance;
    offset(ofs_north, ofs_east);
}

float Location::longitude_scale() const
{
    float scale = cosf(lat * (1.0e-7f * DEG_TO_RAD));
    return MAX(scale, 0.01f);
}

/*
 * convert invalid waypoint with useful data. return true if location changed
 */
bool Location::sanitize(const Location &defaultLoc)
{
    bool has_changed = false;
    // convert lat/lng=0 to mean current point
    if (lat == 0 && lng == 0) {
        lat = defaultLoc.lat;
        lng = defaultLoc.lng;
        has_changed = true;
    }

    // convert relative alt=0 to mean current alt
    if (alt == 0 && relative_alt) {
        relative_alt = false;
        alt = defaultLoc.alt;
        has_changed = true;
    }

    // limit lat/lng to appropriate ranges
    if (!check_latlng()) {
        lat = defaultLoc.lat;
        lng = defaultLoc.lng;
        has_changed = true;
    }

    return has_changed;
}

// make sure we know what size the Location object is:
assert_storage_size<Location, 16> _assert_storage_size_Location;


// return bearing in centi-degrees from location to loc2
int32_t Location::get_bearing_to(const struct Location &loc2) const
{
    const int32_t off_x = loc2.lng - lng;
    const int32_t off_y = (loc2.lat - lat) / loc2.longitude_scale();
    int32_t bearing = 9000 + atan2f(-off_y, off_x) * DEGX100;
    if (bearing < 0) {
        bearing += 36000;
    }
    return bearing;
}

/*
  return true if lat and lng match. Ignores altitude and options
 */
bool Location::same_latlon_as(const Location &loc2) const
{
    return (lat == loc2.lat) && (lng == loc2.lng);
}

// return true when lat and lng are within range
bool Location::check_latlng() const
{
    return check_lat(lat) && check_lng(lng);
}

// see if location is past a line perpendicular to
// the line between point1 and point2 and passing through point2.
// If point1 is our previous waypoint and point2 is our target waypoint
// then this function returns true if we have flown past
// the target waypoint
bool Location::past_interval_finish_line(const Location &point1, const Location &point2) const
{
    return this->line_path_proportion(point1, point2) >= 1.0f;
}

/*
  return the proportion we are along the path from point1 to
  point2, along a line parallel to point1<->point2.

  This will be more than 1 if we have passed point2
 */
float Location::line_path_proportion(const Location &point1, const Location &point2) const
{
    const Vector2f vec1 = point1.get_distance_NE(point2);
    const Vector2f vec2 = point1.get_distance_NE(*this);
    const float dsquared = sq(vec1.x) + sq(vec1.y);
    if (dsquared < 0.001f) {
        // the two points are very close together
        return 1.0f;
    }
    return (vec1 * vec2) / dsquared;
}
<<<<<<< HEAD
<<<<<<< HEAD
>>>>>>> upstream/master
=======
>>>>>>> upstream/master
=======
>>>>>>> upstream/master
