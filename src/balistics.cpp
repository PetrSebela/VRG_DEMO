#include "balistics.hpp"
#include <cmath>
#include <stdexcept>

double ProjectileElevationWithTarget(double initial_velocity, double x, double y)
{    
    if (x <= 0.0 || initial_velocity <= 0.0)
        return NAN;

    double ivs = std::pow(initial_velocity, 2);
    double discriminant = std::pow(ivs, 2) - GRAVITY_ACCELERATION * (GRAVITY_ACCELERATION * std::pow(x, 2) + 2 * y * ivs);
    
    if (discriminant < 0) // projectile cannot read the target
        return NAN;

    // calculating both options and choosing the one with smaller peek
    double denominator = GRAVITY_ACCELERATION * x;
    double root1 = (ivs + std::sqrt(discriminant)) / denominator;
    double root2 = (ivs - std::sqrt(discriminant)) / denominator;

    double phi = std::atan(std::min(root1, root2)); //std::min cooses trajectory with shortest travel distance, std::max chooses trajectory with higher peek and hits target from above 
    return phi * 180 / M_PI ;      
}