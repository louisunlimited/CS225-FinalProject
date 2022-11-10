#pragma once

#include <cmath>
#include <stdexcept>

using namespace std;

/* Radius of the Earth (in km) */
const double RADIUS = 6378.1;

struct Coord {
    double lat_;
    double long_;

    double& operator[](int d) {
        switch (d) {
            case 0:
                return lat_;
            case 1:
                return long_;
            default:
                throw out_of_range("Index out of range");
        }
    }

    double at(int d) const {
        switch (d) {
            case 0:
                return lat_;
            case 1:
                return long_;
            default:
                throw out_of_range("Index out of range");
        }
    }
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
double dist(const Coord& x, const Coord& y) {
    double phi1 = x.lat_ / 180.0 * M_PI;
    double phi2 = y.lat_ / 180.0 * M_PI;
    double thetaDiff = (x.long_ - y.long_) / 180.0 * M_PI;
    double cosAlp = sin(phi1) * sin(phi2) * cos(thetaDiff) + cos(phi1) * cos(phi2);
    return acos(cosAlp) * RADIUS;
}

/**
 * Find the distance between two normalized coordinates
 *
 * @param x The first coordinate
 * @param y The second coordinate
 * @return The normalized distance
 */
double normalizedDist(const Coord& x, const Coord& y) {
    double latDiff = x.lat_ - y.lat_;
    double longDiff = x.long_ - y.long_;
    return sqrt(latDiff * latDiff + longDiff * longDiff);
}
