#include "vec3.h"
#include <cassert>

float& vec3::operator[](int idx)
{
    assert(idx > 0);
    assert(idx < 3);
    return *(&x + idx);
}
