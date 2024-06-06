#ifndef _TRANSFORM_H
#define _TRANSFORM_H

#include "vector3.hpp"
#include <vector>
#include <string>

/**
 * Used as entity component, keeps track of entity position and velocity
 */
class Transform
{
private:
    Vector3 position;
    Vector3 velocity;
    bool isStatic = false;
    std::string name;

public:
    Transform(Vector3 position, Vector3 initial_velocity, std::string name);
    Transform(Vector3 position, Vector3 initial_velocity, std::string name, bool isStatic);
    Transform();
    ~Transform();
    
    /**
     * Updates entity position and velocity based on multiple factors;
     * @param time_step how many seconds pased from last update
     * @param gravity what acceleration should be used for gravity ( positive number )
     */
    void Update(float time_step, float gravity);

    /**
     * Checks if target position if withing allowed deviation radius
     */
    bool InHitRange(Vector3 target_position, float allowed_diviation);

    // returs string representation of transform
    std::string ToString();
    
    Vector3 GetPosition();
    std::string GetName();
};

#endif