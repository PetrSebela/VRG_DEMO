#ifndef _VECTOR3_H
#define _VECTOR3_H
#include <string>


/**
 * Class representing 3D vector with axis x, y, z ( NOTE: y is upwards )
 */
class Vector3
{
public:
    float x = 0;
    float y = 0;
    float z = 0;
    Vector3(float x, float y, float z);
    Vector3();
    ~Vector3();

    /**
     * converts pitch and yaw (both in degrees) to coresponding unit vector
     */
    static Vector3 PitchYawToVector(double pitch, double yaw); 
    
    /**
     * return string representation of said vector
     */
    std::string ToString();
    
    /**
     * return vector magnitude
     */
    float GetMagnitude(); 

    bool operator != (const Vector3 &other);
    bool operator == (const Vector3 &other);

    void operator += (const Vector3 &other);
    Vector3 operator + (const Vector3 &other);

    void operator -= (const Vector3 &other);
    Vector3 operator - (const Vector3 &other);

    void operator *= (const float &other);
    Vector3 operator * (const float &other);

    float operator * (const Vector3 &other);

    void operator /= (const float &other);
    Vector3 operator / (const float &other);
};
#endif