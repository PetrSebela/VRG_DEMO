#include "vector3.hpp"
#include <cmath>
#include <format>

Vector3::Vector3(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}
Vector3::Vector3() {}
Vector3::~Vector3() {}

Vector3 Vector3::PitchYawToVector(double pitch, double yaw)
{
    double pitchRad = pitch * M_PI / 180;
    double yawRad = yaw * M_PI / 180;

    double x = std::cos(yawRad) * std::cos(pitchRad);
    double y = std::sin(pitchRad);
    double z = std::sin(yawRad) * std::cos(pitchRad);
    return Vector3(x, y, z);
}

std::string Vector3::ToString()
{
    return std::format("(\t{},\t{},\t{})", this->x, this->y, this->z);
}

float Vector3::GetMagnitude()
{
    return std::sqrt(std::pow(this->x,2) + std::pow(this->y,2) + std::pow(this->z,2));
}

bool Vector3::operator!=(const Vector3 &other) { return this->x != other.x || this->y != other.y || this->z != other.z; }
bool Vector3::operator==(const Vector3 &other) { return this->x == other.x && this->y == other.y && this->z == other.z; }

void Vector3::operator+=(const Vector3 &other)
{
    this->x += other.x;
    this->y += other.y;
    this->z += other.z;
}

Vector3 Vector3::operator+(const Vector3 &other)
{
    return Vector3(this->x + other.x, this->y + other.y, this->z + other.z);
}

void Vector3::operator-=(const Vector3 &other)
{
    this->x -= other.x;
    this->y -= other.y;
    this->z -= other.z;
}

Vector3 Vector3::operator-(const Vector3 &other)
{
    return Vector3(this->x - other.x, this->y - other.y, this->z - other.z);
}

void Vector3::operator*=(const float &other)
{
    this->x *= other;
    this->y *= other;
    this->z *= other;
}

Vector3 Vector3::operator*(const float &other)
{
    return Vector3(this->x * other, this->y * other, this->z * other);
}

void Vector3::operator/=(const float &other)
{
    this->x /= other;
    this->y /= other;
    this->z /= other;
}

Vector3 Vector3::operator/(const float &other)
{
    return Vector3(this->x / other, this->y / other, this->z / other);
}