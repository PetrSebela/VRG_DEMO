#ifndef _SIM_SETTINGS_H
#define _SIM_SETTINGS_H

#include "vector3.hpp"

// struct for holding simulation settings
struct sim_settings
{
    Vector3 shooter_position;
    Vector3 target_position;
    double projectile_velocity;
    float time_step;
    float projectile_mass; //unused in current simulation since mass doesnt effect freefall acceleration and this simulation doesnt include air drag 
};
#endif