#ifndef OCTREE_H
#define OCTREE_H

#include "vec3.h"

#define X_PLUS 0b001
#define Y_PLUS 0b010
#define Z_PLUS 0b100

enum direction
{
    LEFT,
    RIGHT,
    DOWN,
    UP,
    BACK,
    FRONT
};

class Octree
{
    private:
        unsigned char level;
        Octree* childs[8];
        Octree* parent;
    public:
        vec3 pos;

    public:
        Octree();
        Octree(Octree* parent, vec3 pos);
        virtual ~Octree();

        unsigned char GetOctant();

        void NullChilds();
        void InitChilds();

        Octree* Find(vec3 pos);
        Octree* GetNext(direction dir);

        void Draw();
};

#endif // OCTREE_H
