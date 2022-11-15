

#pragma once

#include <cmath>
#include <stdexcept>

using namespace std;

/* Radius of the Earth (in km) */
const double RADIUS = 6378.1;

struct Coord {
    double lat_;
    double long_;

    double& operator[](int d);

    double at(int d) const;
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
double dist(Coord x, Coord y);

/**
 * Find the distance between two normalized coordinates
 *
 * @param x The first coordinate
 * @param y The second coordinate
 * @return The normalized distance
 */
double normalizedDist(Coord x, Coord y);