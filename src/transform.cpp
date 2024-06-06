#include "transform.hpp"
#include <format>
#include <cmath>

Transform::Transform() {}
Transform::~Transform() {}

Transform::Transform(Vector3 position, Vector3 initial_velocity, std::string name)
{
    this->position = position;
    this->velocity = initial_velocity;
    this->name = name;
}
Transform::Transform(Vector3 position, Vector3 initial_velocity, std::string name, bool isStatic)
{
    this->position = position;
    this->velocity = initial_velocity;
    this->name = name;
    this->isStatic = isStatic;
}

void Transform::Update(float time_step, float gravity)
{
    if (this->isStatic)
        return;
    this->position += this->velocity * time_step;
    this->velocity += Vector3(0, -gravity, 0) * time_step;
}

std::string Transform::ToString()
{
    return std::format("object {} at {} has velocity {}", name, position.ToString(), velocity.ToString());
}

std::string Transform::GetName()
{
    return this->name;
}

bool Transform::InHitRange(Vector3 target_position, float allowed_diviation)
{
    if (this->isStatic)
        return false;

    float target_distance = (target_position - this->position).GetMagnitude();
    return target_distance <= allowed_diviation;
}

Vector3 Transform::GetPosition()
{
    return this->position;
}