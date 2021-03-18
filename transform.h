#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "vec3.h"

class Transform
{
    public:
        vec3 position;
        vec3 rotation;
        vec3 scale;

        Transform();
};

#endif // TRANSFORM_H
