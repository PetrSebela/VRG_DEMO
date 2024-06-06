#ifndef _BALISTICS_H
#define _BALISTICS_H

#define GRAVITY_ACCELERATION 9.80665 // ms^2

/**
 * Returns elevation angle in degrees needed to hit target at coordinates (x, y) with specified initial velocity
 * Shooter is in (0, 0) and target should be in from of him [x in range (0, inf) ] 
 * @warning If initial velocity isnt enough to reach the target, NaN is returned
 * @param initial_velocity initial projectile velocity
 * @param x distance from target
 * @param y target height
 * @return needed elevation angle in degrees
 */
double ProjectileElevationWithTarget(double initial_velocity, double x, double y);

#endif