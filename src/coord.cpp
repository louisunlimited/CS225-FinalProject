#include "coord.h"

double dist(Coord x, Coord y) {
    double phi1 = x.lat_ / 180.0 * M_PI;
    double phi2 = y.lat_ / 180.0 * M_PI;
    double thetaDiff = (x.long_ - y.long_) / 180.0 * M_PI;
    double cosAlp = sin(phi1) * sin(phi2) * cos(thetaDiff) + cos(phi1) * cos(phi2);
    return acos(cosAlp) * RADIUS;
}

double normalizedDist(Coord x, Coord y) {
    double latDiff = x.lat_ - y.lat_;
    double longDiff = x.long_ - y.long_;
    return sqrt(latDiff * latDiff + longDiff * longDiff);
}