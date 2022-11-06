#pragma once

#include <cmath>

using namespace std;

/* Radius of the Earth (in km) */
const double RADIUS = 6378.1;

struct Coord {
    double lat_;
    double long_;
};

/**
 * Find the distance between two coordinates
 *
 * Imagine two vectors starting at the center of the Earth and ending at the two coordinates.
 * First we find the angle between the vectors. Then we multiply the radius of the Earth to get
 * the actual distance.
 *
 * To calculate the angle between two vectors in spherical coordinates, we use the following
 * formula:
 *            cos(alp) = sin(phi1)sin(phi2)cos(the1 - the2) + cos(phi1)cos(phi2)
 * where `phi1`, `phi2` are the latitudes, `the1`, `the2` are the longitudes, and alp is the angle
 * we want to calculate.
 *
 * @param x The first coordinate
 * @param y The second coordinate
 * @return The distance in km
 */
double dist(Coord x, Coord y) {
    double phi1 = x.lat_ / 180.0 * M_PI;
    double phi2 = y.lat_ / 180.0 * M_PI;
    double thetaDiff = (x.long_ - y.long_) / 180.0 * M_PI;
    double cosAlp = sin(phi1) * sin(phi2) * cos(thetaDiff) + cos(phi1) * cos(phi2);
    return acos(cosAlp) * RADIUS;
}
