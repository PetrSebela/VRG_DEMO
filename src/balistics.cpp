#include "balistics.hpp"
#include <cmath>
#include <stdexcept>

double ProjectileElevationWithTarget(double initial_velocity, double x, double y)
{
    if (x <= 0.0 || initial_velocity <= 0.0 || initial_velocity > 10000000.0)
        return NAN;

    double ivs = std::pow(initial_velocity, 2);
    double discriminant = std::pow(ivs, 2) - GRAVITY_ACCELERATION * (GRAVITY_ACCELERATION * std::pow(x, 2) + 2 * y * ivs);

    if (discriminant < 0) // projectile cannot reach the target
        return NAN;

    double denominator = GRAVITY_ACCELERATION * x;

    // double root = (ivs + std::sqrt(discriminant)) / denominator; // also hits the target but trajectory has higher peek
    double root = (ivs - std::sqrt(discriminant)) / denominator;

    double phi = std::atan(root);
    return phi * 180 / M_PI;
}