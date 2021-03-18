#ifndef VECTOR3_H
#define VECTOR3_H


struct vec3
{
    float x;
    float y;
    float z;
    vec3(): x(0), y(0), z(0) {}
    vec3(float _x, float _y, float _z): x(_x), y(_y), z(_z) {}
    float& operator[](int idx);
};

#endif // VECTOR3_H
