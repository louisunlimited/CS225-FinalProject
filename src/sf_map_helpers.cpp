/**
 * Implementation of sf_map.h
 *
 * implements helper methods as well as methods used exclusively for testing.
 */

#include "sf_map.h"

void SFMap::setScale(double scale) {
    if (scale <= 0) {
        throw invalid_argument("Scaling factor must be positive");
    }
    SCALE = scale;
}
